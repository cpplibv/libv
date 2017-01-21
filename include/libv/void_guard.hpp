// File: NullGuard.hpp, Created on 2014. október 28. 1:21, Author: Vader

#pragma once

// std
#include <utility>

namespace libv {

class VoidGuardBase {
private:
	void* ptr_;
public:
	inline void* ptr() const {
		return ptr_;
	}
	VoidGuardBase() { }
	VoidGuardBase(void* ptr) : ptr_(ptr) { }
	virtual ~VoidGuardBase() { }
};

template <typename T>
struct VoidGuard : public VoidGuardBase {
	template <typename... Args>
	VoidGuard(Args&&... args) :
		VoidGuardBase(reinterpret_cast<void*> (new T(std::forward<Args>(args)...))) { }
	virtual ~VoidGuard() {
		delete reinterpret_cast<T*>(ptr());
	}
};

} // namespace libv