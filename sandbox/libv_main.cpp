// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <ctime>
#include <chrono>

//#include <libv/cache.hpp>

struct N {
	char data[200];
	N() { std::cout << "N" << std::endl; }
	N(N&) { std::cout << "N&" << std::endl; }
	N(N&&) { std::cout << "N&&" << std::endl; }
	N(const N&) { std::cout << "constN&" << std::endl; }
	~N() { std::cout << "~N" << std::endl; }
};

template<typename T>
struct CacheAllocator {
	typedef T value_type;

	CacheAllocator() noexcept {
		std::cout << "CacheAllocator() " << typeid (T).name() << std::endl;
	}

	template<typename U>
	CacheAllocator(const CacheAllocator<U>&) noexcept {
		std::cout << "CacheAllocator(CacheAllocator<U>&) " << typeid (T).name() << std::endl;
	}

	inline T* allocate(size_t n, const void* = nullptr) {
		std::cout << "new begin " << typeid (T).name() << std::endl;
		auto x = static_cast<T*> (::operator new(n * sizeof (T)));
		std::cout << "new end " << std::endl;
		return x;
	}

	inline void deallocate(T* ptr, size_t) {
		std::cout << "delete begin " << ptr << " " << typeid (T).name() << std::endl;
		::operator delete(ptr);
		std::cout << "delete end " << std::endl;
	}

	template <typename... Args>
	inline void construct(T* ptr, Args&&... args) {
		std::cout << "construct begin " << ptr << " " << typeid (T).name() << std::endl;
		new (ptr) T(std::forward<Args>(args)...);
		std::cout << "construct end " << std::endl;
	}

	inline void destroy(T* ptr) {
		std::cout << "destroy begin " << ptr << " " << typeid (T).name() << std::endl;
		ptr->~T();
		std::cout << "destroy end " << std::endl;
	}
};

int main(int, char **) {
	{
		std::cout << "--1" << std::endl;
		auto x = std::allocate_shared<N>(CacheAllocator<N>());
		std::cout << "--2" << std::endl;
	}
	std::cout << "--3" << std::endl;
	return 0;
}
