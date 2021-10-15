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
	size_t start = 0;
	size_t end = 0;
	libv::dyn_array<T> store;

public:
	explicit inline ring(size_t size) : store(size) {}

	inline ring(size_t size, T&& init) : store(size, std::move(init)) {}
	inline ring(size_t size, const T& init) : store(size, init) {}

	inline ring(const ring&) = default;
	inline ring& operator=(const ring&) & = default;
	inline ring(ring&&) noexcept = default;
	inline ring& operator=(ring&&) & noexcept = default;

public:
	// iterators
	// range interface

	[[nodiscard]] constexpr inline size_t size() const noexcept {
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
	size_t start;
	size_t count;

//	// helper functions:
//	void check(size_t n) {
//		if (n >= count)
//			throw std::out_of_range("ring");
//	}
//	T* alloc(size_t n) {
//		return reinterpret_cast<T*> (new char[ n * sizeof (T) ]);
//	}

public:
	// construct and destruct:
//	ring() = delete;
//	const ring operator=(const ring&) = delete;
//	explicit ring(size_t c)
//		: store(alloc(c)), count(c) {
//		size_t i;
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
//		for (size_t i = 0; i < count; ++i)
//			(store + i)->~T();
//		delete[] store;
//	}

	// capacity:
	size_t size() const {
		return count;
	}
	size_t max_size() const {
		return count;
	}
	bool empty() const {
		return false;
	}

	// element access:
	T& operator[](size_t n) {
		return store[n];
	}
	const T& operator[](size_t n) const {
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
	T& at(size_t n) {
		n += start;
		check(n);
		return store[count > n ? count : n - store.size()];
	}
	const T& at(size_t n) const {
		n += start;
		check(n);
		return store[count > n ? count : n - store.size()];
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
