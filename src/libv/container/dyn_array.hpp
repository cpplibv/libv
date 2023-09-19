// Project: libv.container, File: src/libv/container/dyn_array.hpp

#pragma once

#include <libv/meta/uninitialized.hpp>
#include <libv/utility/memory/unique.hpp>

#include <cassert>
#include <cstring>
#include <memory>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class dyn_array {
public:
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef std::size_t size_type;
	typedef ptrdiff_t difference_type;

private:
	std::unique_ptr<T[]> store;
	size_type count = 0;

public:
	dyn_array() = default;

	explicit dyn_array(size_type count) :
		store(std::make_unique<T[]>(count)),
		count(count) {
	}

	dyn_array(size_type count, libv::uninitialized_t) :
		store(libv::make_unique_uninitialized<T[]>(count)),
		count(count) {
	}

	dyn_array(const T* data, size_type count) :
		dyn_array(count, libv::uninitialized) {
		std::copy(data, data + count, store.get());
	}

	inline dyn_array(const dyn_array& orig) {
		*this = orig;
	}
	inline dyn_array& operator=(const dyn_array& orig) & {
		count = orig.count;
		store = libv::make_unique_uninitialized<T[]>(count);
		if constexpr (std::is_trivially_copyable_v<T>)
			std::memcpy(store.get(), orig.store.get(), count * sizeof(T));
		else
			std::copy(orig.begin(), orig.end(), store.get());
	}
	inline dyn_array(dyn_array&&) noexcept = default;
	inline dyn_array& operator=(dyn_array&&) & noexcept = default;

	~dyn_array() {
	}

public:
	// iterators:
	[[nodiscard]] inline iterator begin() noexcept {
		return store.get();
	}
	[[nodiscard]] inline const_iterator begin() const noexcept {
		return store.get();
	}
	[[nodiscard]] inline const_iterator cbegin() const noexcept {
		return store.get();
	}
	[[nodiscard]] inline iterator end() noexcept {
		return store.get() + count;
	}
	[[nodiscard]] inline const_iterator end() const noexcept {
		return store.get() + count;
	}
	[[nodiscard]] inline const_iterator cend() const noexcept {
		return store.get() + count;
	}
	[[nodiscard]] inline reverse_iterator rbegin() noexcept {
		return reverse_iterator(end());
	}
	[[nodiscard]] inline const_reverse_iterator rbegin() const noexcept {
		return reverse_iterator(end());
	}
	[[nodiscard]] inline reverse_iterator rend() noexcept {
		return reverse_iterator(begin());
	}
	[[nodiscard]] inline const_reverse_iterator rend() const noexcept {
		return reverse_iterator(begin());
	}

	// capacity:
	[[nodiscard]] inline size_type size() const noexcept {
		return count;
	}
	[[nodiscard]] inline size_type max_size() const noexcept {
		return count;
	}
	[[nodiscard]] inline bool empty() const noexcept {
		return count == 0;
	}

	// element access:
	[[nodiscard]] inline reference operator[](size_type n) noexcept {
		return store[n];
	}
	[[nodiscard]] inline const_reference operator[](size_type n) const noexcept {
		return store[n];
	}
	[[nodiscard]] inline reference front() noexcept {
		return store[0];
	}
	[[nodiscard]] inline const_reference front() const noexcept {
		return store[0];
	}
	[[nodiscard]] inline reference back() noexcept {
		return store[count - 1];
	}
	[[nodiscard]] inline const_reference back() const noexcept {
		return store[count - 1];
	}
	[[nodiscard]] inline const_reference at(size_type n) const noexcept {
		assert(n < count && "Out of range");
		return store[n];
	}
	[[nodiscard]] inline reference at(size_type n) noexcept {
		assert(n < count && "Out of range");
		return store[n];
	}

	// data access:
	[[nodiscard]] inline T* data() noexcept {
		return store.get();
	}
	[[nodiscard]] inline const T* data() const noexcept {
		return store.get();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
