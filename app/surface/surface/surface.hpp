// Project: libv, File: app/surface/surface/surface.hpp

#pragma once

// libv
#include <libv/container/sliding_window_2d.hpp>
#include <libv/math/vec.hpp>
#include <libv/mt/queue_concurrent.hpp>
#include <libv/mt/worker_thread.hpp>
// std
#include <condition_variable>
#include <deque>
#include <mutex>
#include <stop_token>
#include <vector>
// pro
#include <surface/surface/fwd.hpp>
#include <surface/surface/surface_constants.hpp>
#include <surface/view/vec_hash.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct SurfaceGenerationTask {
	// Immutable / thread-safe access:
	std::stop_token stopToken;
	const std::shared_ptr<const Config> config;

	// Exclusive access:
	const std::shared_ptr<ChunkGen> chunkGen;

	// (Thread-safe) Concurrent access with Surface:
	libv::mt::queue_concurrent<std::weak_ptr<Chunk>> queuePending;
	libv::mt::queue_concurrent<std::weak_ptr<Chunk>> queueReady;

public:
	SurfaceGenerationTask(std::stop_token&& stopToken, std::shared_ptr<const Config>&& config, const std::shared_ptr<ChunkGen>& chunkGen);

	void run();
};

// -------------------------------------------------------------------------------------------------

class Surface {
private:
	std::shared_ptr<ChunkGen> chunkGen;

	std::stop_source stopSource;
	std::shared_ptr<SurfaceGenerationTask> currentTask;

	int generation = 0;
	std::vector<std::shared_ptr<Chunk>> activeChunks;
	libv::sliding_window_2D<std::shared_ptr<Chunk>> chunks{
			-chunkRangeWindowSize,
			-chunkRangeWindowSize,
			chunkRangeWindowSize * 2 + 1,
			chunkRangeWindowSize * 2 + 1,
			nullptr};

	libv::vec3f focalPosition;
	libv::vec3f focalDirection;

	libv::mt::worker_thread control{"gen/control"}; // Control thread is last member to be the first to join

public:
	Surface();
	~Surface();

private:
	void scanSpiral(libv::vec2i center, int32_t scanRange);
	void scan(libv::vec2i center, int32_t scanRange);

public:
	int gen(std::shared_ptr<const Config>&& config_);
	bool update(libv::vec3f focalPosition, libv::vec3f focalDirection);

public:
	[[nodiscard]] inline const std::vector<std::shared_ptr<Chunk>>& getActiveChunks() const noexcept {
		return activeChunks;
	}

	[[nodiscard]] inline int getGeneration() const noexcept {
		return generation;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
