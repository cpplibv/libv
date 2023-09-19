// Project: libv.utility, File: src/libv/utility/memory/unique.hpp

#pragma once

#include <libv/meta/uninitialized.hpp>

#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
inline std::unique_ptr<T> make_unique_uninitialized(std::size_t size) {
	static_assert(std::is_array_v<T>);
	using value_type = std::remove_extent_t<T>;
	static constexpr bool supports_libv_uninitialized = (requires { value_type(libv::uninitialized); });

	if constexpr (!std::is_trivially_constructible_v<value_type> && supports_libv_uninitialized) {
		// Not trivially constructible, but supports libv::uninitialized, use it
		std::unique_ptr<T> result(reinterpret_cast<value_type*>(::operator new[](sizeof(value_type) * size)));
		for (std::size_t i = 0; i < size; ++i)
			::new (&result[i]) value_type(libv::uninitialized);
		return result;
	} else {
		// Otherwise just use make_unique_for_overwrite
		return std::make_unique_for_overwrite<T>(size);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
