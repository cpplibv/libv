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
