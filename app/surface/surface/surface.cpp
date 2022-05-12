// Created by dbobula on 4/19/2022.

// hpp
#include <surface/surface/surface.hpp>
// libv
#include <libv/algo/erase_unstable.hpp>
#include <libv/utility/index_spiral.hpp>
#include <libv/utility/timer.hpp>
// std
#include <algorithm>
// pro
#include <surface/log.hpp>
#include <surface/surface/chunk.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

SurfaceGenerationTask::SurfaceGenerationTask(std::stop_token&& stopToken, std::shared_ptr<const Config>&& config, const std::shared_ptr<ChunkGen>& chunkGen) :
	stopToken(std::move(stopToken)), config(config), chunkGen(chunkGen) {}

void SurfaceGenerationTask::run() {
	auto lock = std::unique_lock(mutex);

	//	const auto chunkSize = todoSize;
	//	const auto chunkIndex = libv::index_spiral(i);
	//	const auto focalChunk = ;

	while (true) {
		std::shared_ptr<Chunk> nextChunk;

		queuePending_cv.wait(lock, stopToken, [&]{ return !queuePending.empty(); });
		if (stopToken.stop_requested()) {
			log_surface.trace("Surface generation task is cancelled");
			return; // Generation Task is cancelled
		}

		nextChunk = queuePending.front().lock();
		queuePending.pop_front();

		if (!nextChunk)
			continue;

		{
			lock.unlock();

			libv::Timer timer;
			chunkGen->generateChunk(*config, *nextChunk);
			chunkGen->placeVegetation(*config, *nextChunk);

			log_surface.trace("TimerChunkGen{:>8}: {:8.4f} ms", nextChunk->index, timer.timed_ms().count());

			lock.lock();
		}
		queueReady.emplace_back(std::move(nextChunk));
	}
}

// -------------------------------------------------------------------------------------------------

Surface::Surface() :
	chunkGen(std::make_shared<ChunkGen>()) {
}

Surface::~Surface() {
	stopSource.request_stop();
}

// -------------------------------------------------------------------------------------------------

int Surface::gen(std::shared_ptr<const Config>&& config_) {
	chunks.clear();

	stopSource.request_stop();
	stopSource = std::stop_source{};

	currentTask.reset();
	currentTask = std::make_shared<SurfaceGenerationTask>(stopSource.get_token(), std::move(config_), chunkGen);

	control.execute_async([task = currentTask]() {
		task->run();
	});

	return ++generation;
}

bool Surface::update(libv::vec3f newFocalPosition, libv::vec3f newFocalDirection) {
	bool changed = false;

	if (!currentTask)
		return changed;

	auto lock = std::unique_lock(currentTask->mutex);
	libv::Timer timer;

	const auto oldFocalPosition = currentTask->focalPosition;
	const auto oldFocalDirection = currentTask->focalDirection;

	const auto size = libv::vec2f{32, 32};

	const auto distanceThresholdPrefetch = size.x * 3.f;
	const auto distanceThresholdExpire = distanceThresholdPrefetch * 3.f;
	const auto distanceThresholdInit = distanceThresholdPrefetch * 1.5f;

	const auto prefetchIndexCount = static_cast<int32_t>(distanceThresholdPrefetch / size.x) + 1;

	const auto oldFocalOriginIndex = libv::lround(xy(oldFocalPosition) / size).cast<int32_t>();
	const auto newFocalOriginIndex = libv::lround(xy(newFocalPosition) / size).cast<int32_t>();

	if (oldFocalOriginIndex != newFocalOriginIndex) {
		log_surface.trace("BORDER");

		// Expire distant chunks
		// Detect chunks out of range
		currentTask->chunks.slide(newFocalOriginIndex - oldFocalOriginIndex, [&](libv::vec2i index, const auto& chunk) {
			(void) index;
			if (chunk) {
				log_surface.trace("Destroy CHUNKYYY {}", chunk->index);
				libv::erase_unstable(chunks, chunk);
			}
			return nullptr;
		});
//		std::erase_if(currentTask->chunks, [&](const auto& pair) {
//			const auto& chunk = pair.second;
//			if ((chunk->position - xy(newFocalPosition)).length_sq() > distanceThresholdExpire * distanceThresholdExpire) {
//				log_surface.trace("Killed CHUNKYYY {}", chunk->index);
//				currentTask->queueExpired.emplace_back(chunk);
//				chunk->state = ChunkState::expired;
//				libv::erase_unstable(currentTask->queuePending, chunk);
//				libv::erase_unstable(currentTask->queueReady, chunk);
//				return true;
//			}
//			return false;
//		});

		auto t2 = timer.timef_ms().count();
		log_surface.trace_if(t2 > 0.01f, "Focus update took: Slide {:8.4f} ms", t2);
		timer.reset();

		// Detect chunks in range
		// Add new chunks into the pool
		for (int y = -prefetchIndexCount; y <= prefetchIndexCount; ++y) {
			for (int x = -prefetchIndexCount; x <= prefetchIndexCount; ++x) {
				const auto index = newFocalOriginIndex + libv::vec2i{x, y};
				const auto position = index.cast<float>() * size;
//				log_surface.trace("Check on {:8.4f} ms", t3);

				if (!currentTask->chunks.contains(index))
					continue; // I don't think it should happen

				auto& chunk = currentTask->chunks(index);
				if (chunk != nullptr)
					// ??? Unless generation change?
					continue;

				log_surface.trace("Queue new chunk {}", index);
				chunk = std::make_shared<Chunk>(
						index,
						position,
						size,
						currentTask->config->resolution + 1,
						currentTask->config->globalSeed
				);
				currentTask->queuePending.emplace_back(chunk);
				currentTask->queuePending_cv.notify_all();
			}
		}
	}

	auto t3 = timer.timef_ms().count();
	log_surface.trace_if(t3 > 0.01f, "Focus update took: Scan {:8.4f} ms", t3);
	timer.reset();

//	for (auto& chunk : currentTask->queueReady) {
//		chunks.emplace_back(std::move(chunk));
//		changed = true;
//	}
	if (!currentTask->queueReady.empty()) {
		auto& chunk = currentTask->queueReady.back();
		chunks.emplace_back(std::move(chunk));
		currentTask->queueReady.pop_back();
		changed = true;
	}
//	currentTask->queueReady.clear();

	currentTask->focalPosition = newFocalPosition;
	currentTask->focalDirection = newFocalDirection;

	auto t = timer.timef_ms().count();
	log_surface.trace_if(t > 0.01f, "Focus update took {:8.4f} ms", t);

	return changed;
}

//std::shared_ptr<Chunk> Surface::tryPopReadyChunk() {
//	if (!currentTask)
//		return nullptr;
//
//	auto lock = std::unique_lock(currentTask->mutex);
//	if (currentTask->queueReady.empty())
//		return nullptr;
//
//	auto first = std::move(currentTask->queueReady.back());
//	currentTask->queueReady.pop_back();
//	return first;
//}
//
//std::shared_ptr<Chunk> Surface::tryPopExpiredChunk() {
//	if (!currentTask)
//		return nullptr;
//
//	auto lock = std::unique_lock(currentTask->mutex);
//	if (currentTask->queueExpired.empty())
//		return nullptr;
//
//	auto first = std::move(currentTask->queueExpired.back());
//	currentTask->queueExpired.pop_back();
//	return first;
//}

// -------------------------------------------------------------------------------------------------

} // namespace surface
