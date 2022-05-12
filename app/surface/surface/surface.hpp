// Created by dbobula on 4/19/2022.

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/mt/worker_thread.hpp>
#include <libv/container/sliding_window_2d.hpp>
// std
#include <condition_variable>
#include <deque>
#include <mutex>
#include <stop_token>
#include <unordered_set>
#include <vector>
//#include <queue>
// pro
#include <surface/view/vec_hash.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class Chunk;
class ChunkGen;
class Config;

// -------------------------------------------------------------------------------------------------

struct SurfaceGenerationTask {
	// Immutable / thread-safe access:
	std::stop_token stopToken;
	const std::shared_ptr<const Config> config;

	// Exclusive access:
	const std::shared_ptr<ChunkGen> chunkGen;

	// Concurrent access with Surface:
	std::mutex mutex;
	std::condition_variable_any queuePending_cv;

	std::deque<std::weak_ptr<Chunk>> queuePending;
	std::vector<std::shared_ptr<Chunk>> queueReady;
	libv::sliding_window_2D<std::shared_ptr<Chunk>> chunks{-12, -12, 25, 25, nullptr};

//	std::vector<std::shared_ptr<Chunk>> queuePending;
//	std::vector<std::shared_ptr<Chunk>> chunksActive;
//	std::vector<std::shared_ptr<Chunk>> queueExpired;
//	std::unordered_set<std::shared_ptr<Chunk>, VecHash> chunks;
//	std::unordered_map<libv::vec2i, std::shared_ptr<Chunk>, VecHash> chunks;

	libv::vec3f focalPosition;
	libv::vec3f focalDirection;

public:
	SurfaceGenerationTask(std::stop_token&& stopToken, std::shared_ptr<const Config>&& config, const std::shared_ptr<ChunkGen>& chunkGen);

	void run();
};

// -------------------------------------------------------------------------------------------------

class Surface {
//	std::stop_source stopSource;
//	libv::flat_set<libv::vec2i, >
//	std::atomic<bool> cancelled = false;
//	std::mutex chunksGuard;
//	std::shared_ptr<std::queue<std::shared_ptr<Chunk>>> readyChunks;

private:
	std::shared_ptr<ChunkGen> chunkGen;

	std::stop_source stopSource;
	std::shared_ptr<SurfaceGenerationTask> currentTask;

	int generation = 0;
	std::vector<std::shared_ptr<Chunk>> chunks;

	libv::mt::worker_thread control{"gen/control"}; // Control thread is last member to be the first to join

public:
	Surface();
	~Surface();

public:
	//void changeConfig(std::shared_ptr<const Config>&& config);
//	void setFocus(libv::vec3f focalPosition, libv::vec3f focalDirection);
//	std::shared_ptr<Chunk> tryPopReadyChunk();
//	std::shared_ptr<Chunk> tryPopExpiredChunk();

	int gen(std::shared_ptr<const Config>&& config_);
	bool update(libv::vec3f focalPosition, libv::vec3f focalDirection);

public:
	[[nodiscard]] inline const std::vector<std::shared_ptr<Chunk>>& getChunks() const noexcept {
		return chunks;
	}

	[[nodiscard]] inline int getGeneration() const noexcept {
		return generation;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
