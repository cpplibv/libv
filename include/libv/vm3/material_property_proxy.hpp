// File: MaterialPropertyProxy.hpp, Created on 2015. február 11. 20:49, Author: Vader

#pragma once

#include <cassert>

namespace libv {
	namespace vm3 {
		template<typename T>
		class MaterialPropertyProxy {
			T* ptr;
		public:
			MaterialPropertyProxy(T* ptr) :
				ptr(ptr) { }
			T* operator->() {
				assert(ptr != nullptr);
				return ptr;
			}
			T& operator*() {
				assert(ptr != nullptr);
				return *ptr;
			}
			const T* operator->() const {
				assert(ptr != nullptr);
				return ptr;
			}
			const T& operator*() const {
				assert(ptr != nullptr);
				return *ptr;
			}
			template<typename K>
			T& operator=(K&& v) {
				assert(ptr != nullptr);
				return *ptr = std::forward<K>(v);
			}
			explicit operator bool() const {
				return ptr != nullptr;
			}
		};
	} // namespace vm3
} // namespace libv