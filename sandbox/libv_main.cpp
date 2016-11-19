// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <algorithm>
#include <boost/container/small_vector.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

//struct N {
//	char data[200];
//	N() { std::cout << "N" << std::endl; }
//	N(N&) { std::cout << "N&" << std::endl; }
//	N(N&&) { std::cout << "N&&" << std::endl; }
//	N(const N&) { std::cout << "constN&" << std::endl; }
//	~N() { std::cout << "~N" << std::endl; }
//};
//
//template<typename T>
//struct CacheAllocator {
//	typedef T value_type;
//
//	CacheAllocator() noexcept {
//		std::cout << "CacheAllocator() " << typeid (T).name() << std::endl;
//	}
//
//	template<typename U>
//	CacheAllocator(const CacheAllocator<U>&) noexcept {
//		std::cout << "CacheAllocator(CacheAllocator<U>&) " << typeid (T).name() << std::endl;
//	}
//
//	inline T* allocate(size_t n, const void* = nullptr) {
//		std::cout << "new begin " << typeid (T).name() << std::endl;
//		auto x = static_cast<T*> (::operator new(n * sizeof (T)));
//		std::cout << "new end " << std::endl;
//		return x;
//	}
//
//	inline void deallocate(T* ptr, size_t) {
//		std::cout << "delete begin " << ptr << " " << typeid (T).name() << std::endl;
//		::operator delete(ptr);
//		std::cout << "delete end " << std::endl;
//	}
//
//	template <typename... Args>
//	inline void construct(T* ptr, Args&&... args) {
//		std::cout << "construct begin " << ptr << " " << typeid (T).name() << std::endl;
//		new (ptr) T(std::forward<Args>(args)...);
//		std::cout << "construct end " << std::endl;
//	}
//
//	inline void destroy(T* ptr) {
//		std::cout << "destroy begin " << ptr << " " << typeid (T).name() << std::endl;
//		ptr->~T();
//		std::cout << "destroy end " << std::endl;
//	}
//};
//
//int main(int, char **) {
//	{
//		std::cout << "--1" << std::endl;
//		auto x = std::allocate_shared<N>(CacheAllocator<N>());
//		std::cout << "--2" << std::endl;
//	}
//	std::cout << "--3" << std::endl;
//	return 0;
//}
//
//#include <libv/scoped_cache.hpp>
//
//struct TestRA;
//void foo(libv::ScopedCache<TestRA>&);
//
//int main(int, char **) {
//	libv::ScopedCache<TestRA> cache;
//
//	std::cout << cache.size() << std::endl;
//	foo(cache);
//	std::cout << cache.size() << std::endl;
//	cache.cleanup();
//	std::cout << cache.size() << std::endl;
//
//	return 0;
//}
//
//struct TestRA {
//	TestRA(const std::string& n) : n(n) {
//	}
//	const std::string& getn() const {
//		return n;
//	}
//	~TestRA() {
//	}
//	bool operator<(const TestRA& r) const {
//		return n < r.n;
//	}
//private:
//	std::string n;
//	friend bool operator<(const std::string& t, const TestRA& r) {
//		return t < r.n;
//	}
//	friend bool operator<(const TestRA& r, const std::string& t) {
//		return r.n < t;
//	}
//};
//
//void foo(libv::ScopedCache<TestRA>& cache) {
//	auto x = cache.get("x");
//	auto y = cache.get("y");
//	auto z = cache.get("z");
//}
//
//enum class ThreadAffinity {
//	ANY,
//	GL,
//	IO,
//};
//struct Step {
//	ThreadAffinity affinity;
//	uint32_t priority;
//};
//
//static constexpr Step ResourceLoadSequence[] = {
//	{ThreadAffinity::IO, 0},
//	{ThreadAffinity::GL, 1},
//	{ThreadAffinity::GL, 2}
//};
//
//template<typename T, size_t N>
//size_t count_of(const T (&)[N]) {
//	return N;
//}

//template <typename... Args>
//int foo(Args&&... args) {
//	return (std::forward<Args>(args) + ... + 3);
//	return (std::forward<Args>(args) + ...);
//	return (... + std::forward<Args>(args));
//	return (3 + ... + std::forward<Args>(args));
//}

int main(int, char **) {
	//	std::cout << foo(5, 6, 7, 4) << std::endl;
	//	std::cout << foo() << std::endl;

	//	fprintf(stdout, "\033]10;%s;%s\007\n", __FILE__":141", __FILE__":141");
	//	fprintf(stdout, "\033]10;%s;%s\007\n", "libv_main.cpp:141", "libv_main.cpp:141");

	std::cout << sizeof (std::function<void()>) << std::endl;
	std::cout << sizeof (std::vector<int>) << std::endl;
	std::cout << sizeof (std::shared_ptr<int>) << std::endl;
	std::cout << sizeof (std::string) << std::endl;

	//	for (size_t i = 0; i < count_of(ResourceLoadSequence); ++i) {
	//		std::cout << (int) ResourceLoadSequence[i].affinity << ResourceLoadSequence[i].priority << std::endl;
	//	}
	//	for (uint32_t i = 0; i < sizeof (ResourceLoadSequence) / sizeof (ResourceLoadSequence[0]); ++i) {
	//		std::cout << (int) ResourceLoadSequence[i].affinity << ResourceLoadSequence[i].priority << std::endl;
	//	}
	//	for (const auto& x : ResourceLoadSequence) {
	//		std::cout << (int) x.affinity << x.priority << std::endl;
	//	}

	//	std::cout
	//	<< sizeof(uint64_t) << std::endl
	//	<< sizeof(std::unordered_map<uint64_t, uint64_t>) << std::endl
	//	<< sizeof(boost::container::small_vector<uint64_t, 5>) << std::endl
	//	<< sizeof(std::unordered_map<uint64_t, boost::container::small_vector<uint64_t, 5>>) << std::endl;
}