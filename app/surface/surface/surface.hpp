// Created by dbobula on 4/19/2022.

#pragma once

#include <libv/mt/worker_thread.hpp>

#include <vector>
#include <queue>

#include <surface/surface/chunk.hpp>


namespace surface {

class SurfaceGen {
private:
//	std::atomic<bool> cancelled = false;
	std::stop_source stopSource;
	libv::mt::worker_thread control{"gen_control"};
	std::mutex chunksGuard;
	std::shared_ptr<std::queue<std::shared_ptr<Chunk>>> readyChunks;
	ChunkGen chunkGen;


public:
	SurfaceGen() {}

//	void buildChunk(size_t index, const Config& config);
	void buildChunks(const std::shared_ptr<const Config>& config);
	std::shared_ptr<Chunk> tryPopReadyChunk();
};

class Surface {
private:
	int genCnt = 0;
	std::vector<std::shared_ptr<Chunk>> chunks;
//	std::mutex chunksGuard;
//	std::shared_ptr<const Config> config = nullptr;
	SurfaceGen surfaceGen;

public:
//	explicit Surface(Config&& config_) : config(std::move(config_)) {
//	}

	inline const std::vector<std::shared_ptr<Chunk>>& getChunks() {
		return chunks;
	}

	inline int gen(std::shared_ptr<const Config>&& config) {
		//void cancel
//		std::cout << "surface.gen() " << std::endl;
		chunks.clear();
		surfaceGen.buildChunks(std::move(config));
		return ++genCnt;
	}

	inline bool update() {
		bool changed = false;
		while (auto chunk = surfaceGen.tryPopReadyChunk()) {
			chunks.emplace_back(std::move(chunk));
			changed = true;
		}
		return changed;
	}

	[[nodiscard]] inline int getGenCnt() {
		return genCnt;
	}
};



// =================================================================================================

//	struct What {
//		// inheritance ?
//		// bunch of enums ?
//	};
//
//	struct When {
//		Where where;
//
//		void foo() {
//			where.please_start_new_update(What);
//		}
//	};
//
//	struct Where {
//		libv::mt::worker_thread control;
//		How how;
//
//		void foo() {
//			std::stop_token stop;
//			cancel_last();
//			control.execute_async([stop] {
//				what.how.generate(stop);
//			});
//		}
//	};
//
//	struct How {
//		Genator gen;
//
//		void foo() {
//			generate_surface_and_give_it_to(Yield);
//		}
//	};
//
//	struct Yield {
//		std::mutex mutex;
//		std::vector<finished_What> finished;
//
//		void finish(finished_What);
//		std::vector<finished_What> getFinisheds();
//	};

// =================================================================================================




} // namespace surface


