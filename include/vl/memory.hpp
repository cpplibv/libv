// File: memory.hpp, Created on 2015. április 21. 1:43, Author: Vader

#pragma once

// std
#include <memory>

namespace vl {

template <typename T>
struct shared_or_raw_ptr {
	T* ptr;
	std::shared_ptr<T> sp;
	shared_or_raw_ptr(T* const& component) :
		ptr(component),
		sp(nullptr) { }
	shared_or_raw_ptr(T*&& component) :
		ptr(component),
		sp(component) { }
	shared_or_raw_ptr(std::shared_ptr<T> component) :
		ptr(component.get()),
		sp(component) { }
	shared_or_raw_ptr(T& component) :
		ptr(&component),
		sp(nullptr) { }
//	template<typename K>
//	shared_or_raw_ptr(K&& component) :
//		ptr(nullptr),
//		sp(std::make_shared<K>(std::move(component))) {
//		ptr = &*sp;
//	}
	//
	template<typename K>
	shared_or_raw_ptr(shared_or_raw_ptr<K>&& component) = delete;
	//
	shared_or_raw_ptr() :
		ptr(nullptr),
		sp(nullptr) { }
	shared_or_raw_ptr(const shared_or_raw_ptr<T>& other) :
		ptr(other.ptr),
		sp(other.sp) { }
	shared_or_raw_ptr(shared_or_raw_ptr<T>&& other) :
		ptr(other.ptr),
		sp(std::move(other.sp)) { }
	//
	shared_or_raw_ptr<T>& operator=(const shared_or_raw_ptr<T>& right) = default;
	shared_or_raw_ptr<T>& operator=(shared_or_raw_ptr<T>&& right) = default;

	//
	inline T* operator->() {
		return ptr;
	}
	inline const T* operator->() const {
		return ptr;
	}
	//
	inline T& operator*() {
		return *ptr;
	}
	inline const T& operator*() const {
		return *ptr;
	}
	inline operator bool() const {
		return ptr;
	}
};

} //namespace vl