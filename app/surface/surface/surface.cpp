// Created by dbobula on 4/19/2022.

// hpp
#include <surface/surface/surface.hpp>
// libv
#include <libv/algo/erase_unstable.hpp>
#include <libv/utility/index_spiral.hpp>
#include <libv/utility/timer.hpp>
// std
//#include <algorithm>
// pro
#include <surface/log.hpp>
#include <surface/surface/chunk.hpp>
#include <surface/surface/config.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

SurfaceGenerationTask::SurfaceGenerationTask(std::stop_token&& stopToken, std::shared_ptr<const Config>&& config, const std::shared_ptr<ChunkGen>& chunkGen) :
	stopToken(std::move(stopToken)), config(config), chunkGen(chunkGen) {}

void SurfaceGenerationTask::run() {
	while (true) {
		auto nextChunkOpt = queuePending.pop(stopToken);
		if (!nextChunkOpt) { // Generation Task is cancelled via stop token
			log_surface.trace("Surface generation task is cancelled");
			return;
		}

		auto nextChunk = nextChunkOpt->lock();
		if (!nextChunk) // This chunk's generation is cancelled via weak_ptr expire
			continue;

		libv::Timer timer;
		chunkGen->generateChunk(*config, *nextChunk);
		chunkGen->placeVegetation(*config, *nextChunk);
		log_surface.trace("TimerChunkGen{:>8}: {:8.4f} ms", nextChunk->index, timer.timed_ms().count());

		queueReady.emplace(std::move(nextChunk));

//		if (focalPointChanged)
//			queuePending.sort(std::less<>{}, [](const auto& entry) { return length_sq(entry->position - focalPosition); });
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

void Surface::scanSpiral(libv::vec2i center, int32_t scanRange) {
	libv::index_spiral_loop(scanRange, [&](const auto spiral_index) {
		const auto index = center + spiral_index;
		const auto position = index.template cast<float>() * chunkSize;
//			log_surface.trace("Check on {:8.4f} ms", t3);

		if (!chunks.contains(index))
			return; // (Bad configuration)

		auto& chunk = chunks(index);
		if (chunk != nullptr)
			return;

		log_surface.trace("Queue new chunk {}", index);
		chunk = std::make_shared<Chunk>(
				index,
				position,
				chunkSize,
				currentTask->config->resolution + 1,
				currentTask->config->globalSeed
		);

		currentTask->queuePending.emplace(chunk);
	});
}

void Surface::scan(libv::vec2i center, int32_t scanRange) {
	for (int y = -scanRange; y <= scanRange; ++y) {
		for (int x = -scanRange; x <= scanRange; ++x) {
			const auto index = center + libv::vec2i{x, y};
			const auto position = index.cast<float>() * chunkSize;
//			log_surface.trace("Check on {:8.4f} ms", t3);

			if (!chunks.contains(index))
				continue; // (Bad configuration)

			auto& chunk = chunks(index);
			if (chunk != nullptr)
				continue;

			log_surface.trace("Queue new chunk {}", index);
			chunk = std::make_shared<Chunk>(
					index,
					position,
					chunkSize,
					currentTask->config->resolution + 1,
					currentTask->config->globalSeed
			);

			currentTask->queuePending.emplace(chunk);
		}
	}
}

// -------------------------------------------------------------------------------------------------

int Surface::gen(std::shared_ptr<const Config>&& config_) {
	chunks.fill(nullptr);
	activeChunks.clear();

	++generation;

	stopSource.request_stop();
	stopSource = std::stop_source{};

	currentTask.reset();
	currentTask = std::make_shared<SurfaceGenerationTask>(stopSource.get_token(), std::move(config_), chunkGen);

	control.execute_async([task = currentTask]() {
		task->run();
	});

	const auto oldFocalPosition = focalPosition;
	const auto oldFocalOriginIndex = libv::lround(xy(oldFocalPosition) / chunkSize).cast<int32_t>();

	scanSpiral(oldFocalOriginIndex, chunkRangeScanInitialization);

	return generation;
}

bool Surface::update(libv::vec3f newFocalPosition, libv::vec3f newFocalDirection) {
	bool changed = false;

	if (!currentTask)
		return changed;

	libv::Timer timer;

	const auto oldFocalPosition = focalPosition;
//	const auto oldFocalDirection = focalDirection;

	const auto oldFocalOriginIndex = libv::lround(xy(oldFocalPosition) / chunkSize).cast<int32_t>();
	const auto newFocalOriginIndex = libv::lround(xy(newFocalPosition) / chunkSize).cast<int32_t>();

	if (oldFocalOriginIndex != newFocalOriginIndex) {
		log_surface.trace("BORDER");

		// Detect chunks out of range
		chunks.slide(newFocalOriginIndex - oldFocalOriginIndex, [&](libv::vec2i index, const auto& chunk) {
			(void) index;
			if (chunk) {
				log_surface.trace("Destroy CHUNKYYY {}", chunk->index);
				libv::erase_unstable(activeChunks, chunk);
			}
			return nullptr;
		});

		auto t2 = timer.timef_ms().count();
		log_surface.trace_if(t2 > 0.01f, "Focus update took: Slide {:8.4f} ms", t2);
		timer.reset();

		// Detect chunks in range
		scan(newFocalOriginIndex, chunkRangeScanUpdate);
	}

	auto t3 = timer.timef_ms().count();
	log_surface.trace_if(t3 > 0.01f, "Focus update took: Scan {:8.4f} ms", t3);
	timer.reset();

//	while (auto chunk = currentTask->queueReady.try_pop()) {
	if (auto chunk = currentTask->queueReady.try_pop()) {
		if (auto chunkSP = chunk->lock()) {
			// Check if the ready chunk is still something that we want
			activeChunks.emplace_back(std::move(chunkSP));
			changed = true;
		}
	}

	focalPosition = newFocalPosition;
	focalDirection = newFocalDirection;

	auto t = timer.timef_ms().count();
	log_surface.trace_if(t > 0.01f, "Focus update took {:8.4f} ms", t);

	return changed;
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
