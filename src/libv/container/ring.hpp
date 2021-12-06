// Project: libv.container, File: src/libv/container/ring.hpp

#pragma once

// std
#include <exception>
//#include <vector>
// pro
#include <libv/container/dyn_array.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class ring {
private:
	std::size_t start = 0;
	std::size_t end = 0;
	libv::dyn_array<T> store;

public:
	explicit inline ring(std::size_t size) : store(size) {}

	inline ring(std::size_t size, T&& init) : store(size, std::move(init)) {}
	inline ring(std::size_t size, const T& init) : store(size, init) {}

	inline ring(const ring&) = default;
	inline ring& operator=(const ring&) & = default;
	inline ring(ring&&) noexcept = default;
	inline ring& operator=(ring&&) & noexcept = default;

public:
	// iterators
	// range interface

	[[nodiscard]] constexpr inline std::size_t size() const noexcept {
		return store.size();
	}
};

// =================================================================================================

template <typename T>
struct ring {
	typedef T value_type;

private:
	std::vector<T> store;
//	std::unique_ptr<T[]> store;
	std::size_t start;
	std::size_t count;

//	// helper functions:
//	void check(std::size_t n) {
//		if (n >= count)
//			throw std::out_of_range("ring");
//	}
//	T* alloc(std::size_t n) {
//		return reinterpret_cast<T*> (new char[ n * sizeof (T) ]);
//	}

public:
	// construct and destruct:
//	ring() = delete;
//	const ring operator=(const ring&) = delete;
//	explicit ring(std::size_t c)
//		: store(alloc(c)), count(c) {
//		std::size_t i;
//		try {
//			for (i = 0; i < count; ++i)
//				new (store + i) T;
//		} catch (...) {
//			for (; i > 0; --i)
//				(store + (i - 1))->~T();
//			throw;
//		}
//	}
//	ring(const ring& d)
//		: store(alloc(d.count)), count(d.count) {
//		try {
//			std::uninitialized_copy(d.begin(), d.end(), begin());
//		} catch (...) {
//			delete store;
//			throw;
//		}
//	}
//	~ring() {
//		for (std::size_t i = 0; i < count; ++i)
//			(store + i)->~T();
//		delete[] store;
//	}

	// capacity:
	std::size_t size() const {
		return count;
	}
	std::size_t max_size() const {
		return count;
	}
	bool empty() const {
		return false;
	}

	// element access:
	T& operator[](std::size_t n) {
		return store[n];
	}
	const T& operator[](std::size_t n) const {
		return store[n];
	}
	T& front() {
		return store[start];
	}
	const T& front() const {
		return store[start];
	}
	T& back() {
		return store[start + count - 1];
	}
	const T& back() const {
		return store[start + count - 1];
	}
	T& at(std::size_t n) {
		n += start;
		check(n);
		return store[count > n ? count : n - store.size()];
	}
	const T& at(std::size_t n) const {
		n += start;
		check(n);
		return store[count > n ? count : n - store.size()];
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
