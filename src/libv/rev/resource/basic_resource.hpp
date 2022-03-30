// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// std
#include <algorithm>
#include <mutex>
#include <vector>
// libv
#include <libv/algo/erase_stable.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
//// pro
//#include <libv/rev/log.hpp>


namespace libv {
namespace rev {

// =================================================================================================
// =================================================================================================
// =================================================================================================

//libv.res: Resource - New library for resource and asset loading, caching and tracking
//		Based on the libv.rev's Shader and ShaderLoad the same concept can be generalized for
//		assets, audio, textures, mods/scripts and models
//
//		Requirements:
//			cache
//			resolve / include directory / loader mapping
//			load and inclusion/extension of tracking set (recursion should be possible)
//			filesystem resources
//			filesystem resource fragments (inclusion subjects)
//			virtual resources
//			virtual generated resources
//			tracking of virtual resources
//			tracking of filesystem resources
//			load/reload/unload/tracking events
//			excellent compile time: runtime polymorphism or easy pimpl (2 hook class, 1 manager, 1 resource that are type erased)

// -------------------------------------------------------------------------------------------------

template <typename CRTP, typename T>
class BasicResourceLoader : public libv::ref_count_base<CRTP> {
	std::mutex mutex;
	std::vector<T*> cache; // Ordered

public:
	template <typename... Args>
	libv::intrusive2_ptr<T> lookup(Args&&... args) {
		auto lock = std::unique_lock(mutex);

		const auto comp = [&](const T* resource) { return resource->compare(args...); };
		const auto it = std::ranges::lower_bound(cache, 0, {}, comp);

		if (it != cache.end() && 0 == (*it)->compare(args...))
			return libv::intrusive2_ptr<T>(*it); // (it)->intrusive_from_this();

		auto result = static_cast<CRTP&>(*this).create_resource(std::forward<Args>(args)...);
		cache.emplace(it, result.get());
		return result;
	}

	void unload(T* resource) {
		auto lock = std::unique_lock(mutex);

		libv::erase_stable(cache, resource);
	}

	// void update(libv::gl::GL& gl);
	// void attach_libv_ui_hub(Hub&& hub);
	// add_include_directory
	// tracking
	// foreach
	// on_create
	// on_update
	// on_destroy
};

template <typename CRTP, typename LoaderT>
class BasicResource : public libv::ref_count_base<CRTP> {
	friend libv::ref_count_access;

protected:
	using Base = BasicResource;
	using Loader = LoaderT;

private:
//	std::atomic<Loader*> loader;
	Loader* loader;
//	libv::intrusive2_ptr<Loader> loader;
//	libv::intrusive_ptr<Loader> loader;
//	std::weak_ptr<Loader> loader;

public:
	explicit BasicResource(Loader* loader) : loader(loader) {}

private:
	constexpr inline void ref_count_zero() noexcept {
		// !!! Loader lifetime synchronization without a weak_ptr is not solved yet
		if (loader)
			loader->unload(static_cast<CRTP*>(this));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
