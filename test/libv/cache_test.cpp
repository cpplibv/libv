// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>

#include <libv/container/cache.hpp>
#include <thread>

#include "cache_test_util.hpp"

int TestRA::instanceNumber = 0;
int TestRB::instanceNumber = 0;
int TestRM::instanceNumber = 0;
std::atomic<int> LoadableTestResource::nextid{0};

using libv::Cache;
using libv::LoaderCache;

// Forward Declarable ------------------------------------------------------------------------------

TEST_CASE("Cache can use forward declared type") {
	struct TypeFwd;
	libv::Cache<TypeFwd> cache;
	CHECK(cache.size() == 0);
}

struct TypeFwd2;
void cacheForwardRest(libv::Cache<TypeFwd2>& cache);

TEST_CASE("Cache can use forward declared type and use it when complete") {
	libv::Cache<TypeFwd2> cache;
	cacheForwardRest(cache);
}

struct TypeFwd2 {
	int size;
	TypeFwd2(int size) : size(size) { }

	bool operator<(const TypeFwd2& r) const {
		return size < r.size;
	}
	friend bool operator<(int t, const TypeFwd2& r) {
		return t < r.size;
	}
	friend bool operator<(const TypeFwd2& r, int t) {
		return r.size < t;
	}
};

void cacheForwardRest(libv::Cache<TypeFwd2>& cache) {
	auto x = cache.get(2);
	CHECK(x->size == 2);
	CHECK(cache.size() == 1);
}

TEST_CASE("LoaderCache can use forward declared type") {
	struct TypeFwd;
	libv::LoaderCache<TypeFwd> cache;
	CHECK(cache.size() == 0);
}

struct TypeFwd3;
void cacheForwardRest2(libv::LoaderCache<TypeFwd3>& cache);

TEST_CASE("LoaderCache can use forward declared type and use it when complete") {
	libv::LoaderCache<TypeFwd3> cache;
	cacheForwardRest2(cache);
}

struct TypeFwd3 {
	int size;
	TypeFwd3(int size) : size(size) { }

	void load(const std::shared_ptr<TypeFwd3>&) {}
	void unload(const std::shared_ptr<TypeFwd3>&) {}
	bool operator<(const TypeFwd3& r) const {
		return size < r.size;
	}
	friend bool operator<(int t, const TypeFwd3& r) {
		return t < r.size;
	}
	friend bool operator<(const TypeFwd3& r, int t) {
		return r.size < t;
	}
};

void cacheForwardRest2(libv::LoaderCache<TypeFwd3>& cache) {
	auto x = cache.get(2);
	CHECK(x->size == 2);
	CHECK(cache.size() == 1);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Cache can provide instance") {
	Cache<TestRA> rc;
	CHECK(TestRA::instanceNumber == 0);

	auto v = rc.get(std::string("b"));

	CHECK(v->getn() == "b");
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("Cache can provide instance from multiple argument") {
	Cache<TestRM> rc;
	auto x = rc.get(42, 5);
	CHECK(x.get() != nullptr);
	CHECK(x->n0 == 42);
	CHECK(x->n1 == 5);
	CHECK(TestRM::instanceNumber == 1);
}

TEST_CASE("Cache can provide multiple instance from multiple argument") {
	Cache<TestRM> rc;
	auto x = rc.get(42, 3);
	auto y = rc.get(42, 6);
	CHECK(x.get() != y.get());
	CHECK(TestRM::instanceNumber == 2);
}

TEST_CASE("Cache can provide instance without duplication") {
	Cache<TestRA> rc;

	auto v0 = rc.get(std::string("b"));
	auto v1 = rc.get(std::string("b"));

	CHECK(v0->getn() == "b");
	CHECK(v1->getn() == "b");
	CHECK(v0.get() == v1.get());
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("Cache can provide different instances") {
	Cache<TestRA> rc;

	auto v0 = rc.get(std::string("a"));
	auto v1 = rc.get(std::string("b"));

	CHECK(v0->getn() == "a");
	CHECK(v1->getn() == "b");
	CHECK(v0.get() != v1.get());
	CHECK(TestRA::instanceNumber == 2);
}

TEST_CASE("Cache does not participate in ownership") {
	Cache<TestRA> rc;

	{
		auto v0 = rc.get(std::string("b"));
		CHECK(TestRA::instanceNumber == 1);
	}
	CHECK(TestRA::instanceNumber == 0);
}

TEST_CASE("Cache should handle convertible arguments") {
	Cache<std::string> rc;

	auto v0 = rc.get("x");
	auto v1 = rc.get("x");
	CHECK(v0.get() == v1.get());
	auto v2 = rc.get("y");
	CHECK(v0.get() != v2.get());
}

TEST_CASE("Cache should handle non copyable argument lvalue reference", "[static]") {
	Cache<TestRNonCopyArged> rc;
	NonCopyable a0(3);
	auto v0 = rc.get(a0);
}

TEST_CASE("Cache should handle non copyable argument prvalue reference", "[static]") {
	Cache<TestRNonCopyArged> rc;
	auto v0 = rc.get(NonCopyable(3));
}

TEST_CASE("Cache should handle non copyable argument rvalue reference", "[static]") {
	Cache<TestRNonCopyArged> rc;
	NonCopyable a0(3);
	auto v0 = rc.get(std::move(a0));
}

// libv::use -----------------------------------------------------------------------------------------

TEST_CASE("Cache comparator can handle use option for single argument") {
	Cache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(0, 1, 2);
	auto r1 = rc.get<libv::use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 2u);
}

TEST_CASE("Cache comparator can handle use option for match even different pos arguments") {
	Cache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(42, 1, 2);
	auto r1 = rc.get<libv::use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.get() == r1.get());
	CHECK(r0->n == 42);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 42);
	CHECK(r1->argCount == 3u);
}

TEST_CASE("LoaderCache comparator can handle use option for single argument") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(0, 1, 2);
	auto r1 = rc.get<libv::use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 2u);
}

TEST_CASE("LoaderCache comparator can handle use option for match even different pos arguments") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(42, 1, 2);
	auto r1 = rc.get<libv::use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.get() == r1.get());
	CHECK(r0->n == 42);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 42);
	CHECK(r1->argCount == 3u);
}

// libv::ignore -------------------------------------------------------------------------------

//TEST_CASE("Cache comparator can handle use option for single argument") {
//	Cache<LoadableTestResourceB> rc;
//	auto r0 = rc.get<ignore<0>>(0, 1, 2);
//	auto r1 = rc.get<ignore<0>>(5, 6);
//
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//	CHECK(r0->n == 0);
//	CHECK(r0->argCount == 3u);
//	CHECK(r1->n == 5);
//	CHECK(r1->argCount == 2u);
//}
//
//TEST_CASE("Cache comparator can handle use option for match even different arguments") {
//	Cache<LoadableTestResourceB> rc;
//	auto r0 = rc.get<ignore<0>>(42, 1, 2);
//	auto r1 = rc.get<ignore<1>>(5, 42);
//
//	CHECK(rc.size() == 1u);
//	CHECK(r0.get() == r1.get());
//	CHECK(r0->n == 42);
//	CHECK(r0->argCount == 3u);
//	CHECK(r1->n == 42);
//	CHECK(r1->argCount == 2u);
//}
//
//TEST_CASE("LoaderCache comparator can handle use option for single argument") {
//	LoaderCache<LoadableTestResourceB> rc;
//	auto r0 = rc.get<ignore<0>>(0, 1, 2);
//	auto r1 = rc.get<ignore<0>>(5, 6);
//
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//	CHECK(r0->n == 0);
//	CHECK(r0->argCount == 3u);
//	CHECK(r1->n == 5);
//	CHECK(r1->argCount == 2u);
//}
//
//TEST_CASE("LoaderCache comparator can handle use option for match even different arguments") {
//	LoaderCache<LoadableTestResourceB> rc;
//	auto r0 = rc.get<ignore<0>>(42, 1, 2);
//	auto r1 = rc.get<ignore<1>>(5, 42);
//
//	CHECK(rc.size() == 1u);
//	CHECK(r0.get() == r1.get());
//	CHECK(r0->n == 42);
//	CHECK(r0->argCount == 3u);
//	CHECK(r1->n == 42);
//	CHECK(r1->argCount == 2u);
//}

// multithread -------------------------------------------------------------------------------------

void cacheUseTask(Cache<LoadableTestResource>* c) {
	for (size_t i = 0; i < 200; ++i) {
		auto x0 = c->get(i+0);
		auto x1 = c->get(i+1);
		auto x2 = c->get(i+2);
		auto x3 = c->get(i+3);
		auto x4 = c->get(i+4);
		auto x5 = c->get(i+5);
		auto x6 = c->get(i+6);
		auto x7 = c->get(i+7);
		auto x8 = c->get(i+8);
		auto x9 = c->get(i+9);
	}
}

TEST_CASE("Cache multithread stress") {
	Cache<LoadableTestResource> c;
	std::thread t0(cacheUseTask, &c);
	std::thread t1(cacheUseTask, &c);
	std::thread t2(cacheUseTask, &c);
	t0.join();
	t1.join();
	t2.join();
	CHECK(c.size() == 0);
}

void loaderCacheUseTask(LoaderCache<LoadableTestResource>* c) {
	for (size_t i = 0; i < 200; ++i) {
		auto x0 = c->get(i+0);
		auto x1 = c->get(i+1);
		auto x2 = c->get(i+2);
		auto x3 = c->get(i+3);
		auto x4 = c->get(i+4);
		auto x5 = c->get(i+5);
		auto x6 = c->get(i+6);
		auto x7 = c->get(i+7);
		auto x8 = c->get(i+8);
		auto x9 = c->get(i+9);
	}
}

TEST_CASE("LoaderCache multithread stress") {
	LoaderCache<LoadableTestResource> c;
	std::thread t0(loaderCacheUseTask, &c);
	std::thread t1(loaderCacheUseTask, &c);
	std::thread t2(loaderCacheUseTask, &c);
	t0.join();
	t1.join();
	t2.join();
	CHECK(c.size() == 0);
}