// Project: libv.meta, File: src/libv/meta/uninitialized.hpp

#pragma once

//#include <memory>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct uninitialized_t {};

constexpr inline uninitialized_t uninitialized{};

// -------------------------------------------------------------------------------------------------

//template <class T>
//struct uninitialized_allocator {
//	using value_type = T;
//
//public:
//	uninitialized_allocator() = default;
//
//	template <class U>
//	explicit constexpr uninitialized_allocator(const uninitialized_allocator<U>&) noexcept {}
//
//	[[nodiscard]] T* allocate(std::size_t n) {
//		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
//			throw std::bad_array_new_length();
//
//		if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
//			return p;
//		}
//
//		throw std::bad_alloc();
//	}
//
//	void deallocate(T* p, std::size_t n) noexcept {
//		std::free(p);
//	}
//};
//
//template <class T, class U>
//bool operator==(const uninitialized_allocator<T>&, const uninitialized_allocator<U>&) { return true; }
//
//template <class T, class U>
//bool operator!=(const uninitialized_allocator<T>&, const uninitialized_allocator<U>&) { return false; }

// -------------------------------------------------------------------------------------------------

} // namespace libv
