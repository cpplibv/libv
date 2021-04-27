// Project: libv.container, File: src/libv/container/dynarray.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <exception>


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
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

private:
	T* store;
	size_type count;

private:
	void check(size_type n) {
		if (n >= count)
			throw std::out_of_range("dyn_array");
	}

	T* alloc(size_type n) {
		return reinterpret_cast<T*>(new char[n * sizeof(T)]);
	}

public:
	dyn_array() = delete;
	const dyn_array operator=(const dyn_array&) = delete;

	explicit dyn_array(size_type c) :
		store(alloc(c)),
		count(c) {
		size_type i;
		try {
			for (i = 0; i < count; ++i)
				new(store + i) T;
		} catch (...) {
			for (; i > 0; --i)
				(store + (i - 1))->~T();
			throw;
		}
	}

	dyn_array(const dyn_array& d) :
		store(alloc(d.count)),
		count(d.count) {
		try {
			std::uninitialized_copy(d.begin(), d.end(), begin());
		} catch (...) {
			delete store;
			throw;
		}
	}

	~dyn_array() {
		for (size_type i = 0; i < count; ++i)
			(store + i)->~T();
		delete[] store;
	}

public:
	// iterators:
	iterator begin() {
		return store;
	}
	const_iterator begin() const {
		return store;
	}
	const_iterator cbegin() const {
		return store;
	}
	iterator end() {
		return store + count;
	}
	const_iterator end() const {
		return store + count;
	}
	const_iterator cend() const {
		return store + count;
	}
	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const {
		return reverse_iterator(end());
	}
	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const {
		return reverse_iterator(begin());
	}

	// capacity:
	size_type size() const {
		return count;
	}
	size_type max_size() const {
		return count;
	}
	bool empty() const {
		return false;
	}

	// element access:
	reference operator[](size_type n) {
		return store[n];
	}
	const_reference operator[](size_type n) const {
		return store[n];
	}
	reference front() {
		return store[0];
	}
	const_reference front() const {
		return store[0];
	}
	reference back() {
		return store[count - 1];
	}
	const_reference back() const {
		return store[count - 1];
	}
	const_reference at(size_type n) const {
		check(n);
		return store[n];
	}
	reference at(size_type n) {
		check(n);
		return store[n];
	}

	// data access:
	T* data() {
		return store;
	}
	const T* data() const {
		return store;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
