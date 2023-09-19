// Project: libv.rev, File: src/libv/rev/resource/basic_resource.hpp

#pragma once

#include <libv/re/resource/common_resource.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>

#include <cassert>
#include <cstdint>
#include <mutex>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class LoadingState : int32_t {
	standby = 0,
	delayed,
	loading,
	loadingRepeat,
};

// (Incomplete:)
// template <typename T>
// concept Resource = requires (T& res) {
// 	std::derived_from<T, libv::ref_count_base>;
// 	{ res.cold->mutex } -> std::mutex;
// 	{ res.cold->loadingState } -> LoadingState;
// 	{ res.cold->cached } -> bool;
// 	{ res.cold->loadVersion } -> int;
// 	{ res.cold->currentVersion } -> int;
// };

struct BasicResource {
private:
	template <typename Resource, typename ResourceLoader>
	static inline void _implRequestLoad(Resource* resource, std::unique_lock<std::mutex>& lock, ResourceLoader& loader, LoadingMode mode) noexcept {
		switch (resource->cold->loadingState) {
		case LoadingState::delayed:
			// Noop, waiting for delayed guard release
			break;
		case LoadingState::standby:
			resource->cold->loadingState = LoadingState::loading;
			lock.unlock();
			loader.executeLoad(libv::intrusive2_ptr<Resource>(resource), mode);
			break;
		case LoadingState::loading:
			resource->cold->loadingState = LoadingState::loadingRepeat;
			break;
		case LoadingState::loadingRepeat:
			// Noop
			break;
		}
	}
	template <typename Resource, typename ResourceLoader>
	static inline void _loadFinish(Resource* resource, std::unique_lock<std::mutex>& lock, ResourceLoader& loader) noexcept {
		switch (resource->cold->loadingState) {
		case LoadingState::delayed:
			assert(false && "Internal error");
			break;
		case LoadingState::standby:
			assert(false && "Internal error");
			break;
		case LoadingState::loading:
			resource->cold->loadingState = LoadingState::standby;
			break;
		case LoadingState::loadingRepeat:
			resource->cold->loadingState = LoadingState::loading;
			lock.unlock();
			loader.executeLoad(libv::intrusive2_ptr<Resource>(resource), LoadingMode::async);
			break;
		}
	}

public:
	template <typename Resource, typename ResourceLoader>
	static inline void implRequestLoad(Resource* resource, ResourceLoader& loader, LoadingMode mode) noexcept {
		auto lock = std::unique_lock(resource->cold->mutex);
		_implRequestLoad(resource, lock, loader, mode);
	}

	template <typename Resource, typename ResourceLoader>
	static inline void implRequestActivatingLoad(Resource* resource, ResourceLoader& loader, LoadingMode mode) noexcept {
		auto lock = std::unique_lock(resource->cold->mutex);
		resource->cold->loadingState = LoadingState::standby;
		_implRequestLoad(resource, lock, loader, mode);
	}

	template <typename Resource, typename ResourceLoader>
	static inline void implRecordLoadFailure(Resource* resource, ResourceLoader& loader) noexcept {
		auto lock = std::unique_lock(resource->cold->mutex);
		++resource->cold->loadVersion;
		_loadFinish(resource, lock, loader);
	}

	template <typename Resource, typename ResourceLoader>
	static inline void implRecordLoadSuccess(Resource* resource, ResourceLoader& loader) noexcept {
		auto lock = std::unique_lock(resource->cold->mutex);
		++resource->cold->loadVersion;
		resource->cold->currentVersion = resource->cold->loadVersion;
		resource->cold->fallback = nullptr;
		_loadFinish(resource, lock, loader);
	}

	template <typename Resource>
	static inline void implRecordUnloadFromCache(Resource* resource) noexcept {
		// Called from the loader from inside the impl_ref_count_one function if cache unloads the resource
		resource->cold->cached = false;
	}

	template <typename Resource, typename ResourceLoader>
	static inline void impl_ref_count_one(Resource* resource, ResourceLoader& loader) noexcept {
		if (resource->cold->cached)
			// Loader has the last reference, signal potential unload
			loader.unload(resource);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
