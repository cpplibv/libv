// File: NullGuard.hpp, Created on 2014. október 28. 1:21, Author: Vader

#pragma once

// std
#include <utility>

namespace vl {

class VoidGuardBase {
	void* ptr;
public:
	inline void* getPtr() const {
		return ptr;
	}
	VoidGuardBase() { }
	VoidGuardBase(void* ptr) : ptr(ptr) { }
	virtual ~VoidGuardBase() { }
};

template <typename T>
struct VoidGuard : public VoidGuardBase {
	template <typename... Args>
	VoidGuard(Args&&... args) :
		VoidGuardBase(reinterpret_cast<void*> (new T(std::forward<Args>(args)...))) { }
	virtual ~VoidGuard() {
		delete reinterpret_cast<T*> (getPtr());
	}
};
} //namespace vl