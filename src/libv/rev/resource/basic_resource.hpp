// Project: libv.rev, File: src/libv/rev/resource/basic_resource.hpp

#pragma once

// std
#include <algorithm>
#include <vector>
// libv
#include <libv/algo/erase_stable.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

// libv.res: Resource - New library for resource and asset loading, caching and tracking
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

template <typename T>
class BasicResourceLoaderCache {
public:
	std::vector<libv::intrusive2_ptr<T>> storage; // Ordered by compare

	template <typename... Args>
	libv::intrusive2_ptr<T>& lookup(Args&&... args) {
		// O( N*log(N) )
		const auto comp = [&](const libv::intrusive2_ptr<T>& resource) { return resource->compare(args...); };
		const auto it = std::ranges::lower_bound(storage, 0, {}, comp);

		if (it != storage.end() && 0 == (*it)->compare(args...))
			return *it;

		return *storage.emplace(it, nullptr);
	}

	void remove(T* resource) {
		// O( N^2 )
		libv::erase_stable(storage, resource, [](const libv::intrusive2_ptr<T>& ptr) {
			return ptr.get();
		});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
