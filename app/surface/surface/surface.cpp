// Created by dbobula on 4/19/2022.

// hpp
#include <surface/surface/surface.hpp>
// libv
#include <libv/utility/index_spiral.hpp>
#include <libv/utility/timer.hpp>
// pro
#include <surface/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

//void SurfaceGen::buildChunk(size_t index, const Config& config) {
//	const auto chunkIndex = libv::index_spiral(index).cast<int32_t>();
//	std::unique_ptr<Chunk> chunk = chunkGen.generateChunk(config, chunkIndex);
//	chunkGen.placeVegetation(chunk, config);
//	// more log needed
////		fmt::print("TimerChunkGen: {:8.4f} ms", timerChunkGen.timed_ms().count());
////		std::cout << std::endl;
//
//	auto chunksLock = std::unique_lock(chunksGuard);
//	readyChunks.emplace_back(std::move(chunk));
//}

void SurfaceGen::buildChunks(const std::shared_ptr<const Config>& config_sp) {
	stopSource.request_stop();
	stopSource = std::stop_source{};

	readyChunks = std::make_shared<std::queue<std::shared_ptr<Chunk>>>();

	control.execute_async([this, config_sp, resultQueue = readyChunks, token = stopSource.get_token()]() {
		const auto& config = *config_sp;

		for (size_t i = 0; i < config.numChunks && !token.stop_requested(); ++i) {
			libv::Timer timer;

			const auto chunkIndex = libv::index_spiral(i);

			std::shared_ptr<Chunk> chunk = chunkGen.generateChunk(config, chunkIndex);
			chunkGen.placeVegetation(*chunk, config);

			auto chunksLock = std::unique_lock(chunksGuard);
			resultQueue->emplace(std::move(chunk));

			log_surface.trace("TimerChunkGen{:>8}: {:8.4f} ms", chunkIndex, timer.timed_ms().count());
		}
	});
}

std::shared_ptr<Chunk> SurfaceGen::tryPopReadyChunk() {
	auto chunksLock = std::unique_lock(chunksGuard);
	if (!readyChunks || readyChunks->empty())
		return nullptr;

	auto first = std::move(readyChunks->front());
	readyChunks->pop();
	return first;
}


// -------------------------------------------------------------------------------------------------






} // namespace surface
