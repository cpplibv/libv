// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>

#include <libv/scoped_cache.hpp>
#include <thread>

#include "cache_test_util.hpp"

using libv::ScopedCache;

// Forward Declarable ------------------------------------------------------------------------------

struct TypeFwdB2;
void cacheForwardBRest(libv::ScopedCache<TypeFwdB2>& cache);

TEST_CASE("ScopedCache can use forward declared type and use it when complete") {
	libv::ScopedCache<TypeFwdB2> cache;
	cacheForwardBRest(cache);
}

struct TypeFwdB2 {
	int size;
	TypeFwdB2(int size) : size(size) { }

	bool operator<(const TypeFwdB2& r) const {
		return size < r.size;
	}
	friend bool operator<(int t, const TypeFwdB2& r) {
		return t < r.size;
	}
	friend bool operator<(const TypeFwdB2& r, int t) {
		return r.size < t;
	}
};

void cacheForwardBRest(libv::ScopedCache<TypeFwdB2>& cache) {
	auto x = cache.get(2);
	CHECK(x.first->size == 2);
	CHECK(cache.size() == 1);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("ScopedCache can provide instance") {
	ScopedCache<TestRA> rc;
	CHECK(TestRA::instanceNumber == 0);

	auto v = rc.get(std::string("b"));

	CHECK(v.first->getn() == "b");
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("ScopedCache can provide instance from multiple argument") {
	ScopedCache<TestRM> rc;
	auto x = rc.get(42, 5);
	CHECK(x.first != nullptr);
	CHECK(x.first->n0 == 42);
	CHECK(x.first->n1 == 5);
	CHECK(TestRM::instanceNumber == 1);
}

TEST_CASE("ScopedCache can provide multiple instance from multiple argument") {
	ScopedCache<TestRM> rc;
	auto x = rc.get(42, 3);
	auto y = rc.get(42, 6);
	CHECK(x.first != y.first);
	CHECK(TestRM::instanceNumber == 2);
}

TEST_CASE("ScopedCache can provide instance without duplication") {
	ScopedCache<TestRA> rc;

	auto v0 = rc.get(std::string("b"));
	auto v1 = rc.get(std::string("b"));

	CHECK(v0.first->getn() == "b");
	CHECK(v1.first->getn() == "b");
	CHECK(v0.first == v1.first);
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("ScopedCache can provide different instances") {
	ScopedCache<TestRA> rc;

	auto v0 = rc.get(std::string("a"));
	auto v1 = rc.get(std::string("b"));

	CHECK(v0.first->getn() == "a");
	CHECK(v1.first->getn() == "b");
	CHECK(v0.first != v1.first);
	CHECK(TestRA::instanceNumber == 2);
}

TEST_CASE("ScopedCache does participate in ownership") {
	ScopedCache<TestRA> rc;

	{
		auto v0 = rc.get(std::string("b"));
		CHECK(TestRA::instanceNumber == 1);
	}
	CHECK(TestRA::instanceNumber == 1);
	rc.cleanup();
	CHECK(TestRA::instanceNumber == 0);
}

TEST_CASE("ScopedCache should handle convertible arguments") {
	ScopedCache<std::string> rc;

	auto v0 = rc.get("x");
	auto v1 = rc.get("x");
	CHECK(v0.first == v1.first);
	auto v2 = rc.get("y");
	CHECK(v0.first != v2.first);
}

TEST_CASE("ScopedCache should handle non copyable argument lvalue reference", "[static]") {
	ScopedCache<TestRNonCopyArged> rc;
	NonCopyable a0(3);
	auto v0 = rc.get(a0);
}

TEST_CASE("ScopedCache should handle non copyable argument prvalue reference", "[static]") {
	ScopedCache<TestRNonCopyArged> rc;
	auto v0 = rc.get(NonCopyable(3));
}

TEST_CASE("ScopedCache should handle non copyable argument rvalue reference", "[static]") {
	ScopedCache<TestRNonCopyArged> rc;
	NonCopyable a0(3);
	auto v0 = rc.get(std::move(a0));
}

// libv::use -----------------------------------------------------------------------------------------

TEST_CASE("ScopedCache comparator can handle use option for single argument") {
	ScopedCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(0, 1, 2);
	auto r1 = rc.get<libv::use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.first != r1.first);
	CHECK(r0.first->n == 0);
	CHECK(r0.first->argCount == 3u);
	CHECK(r1.first->n == 5);
	CHECK(r1.first->argCount == 2u);
}

TEST_CASE("ScopedCache comparator can handle use option for match even different pos arguments") {
	ScopedCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<libv::use<0>>(42, 1, 2);
	auto r1 = rc.get<libv::use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.first == r1.first);
	CHECK(r0.first->n == 42);
	CHECK(r0.first->argCount == 3u);
	CHECK(r1.first->n == 42);
	CHECK(r1.first->argCount == 3u);
}

// libv::ignore -------------------------------------------------------------------------------

//TEST_CASE("ScopedCache comparator can handle use option for single argument") {
//	ScopedCache<LoadableTestResourceB> rc;
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
//TEST_CASE("ScopedCache comparator can handle use option for match even different arguments") {
//	ScopedCache<LoadableTestResourceB> rc;
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
void cacheUseTask(ScopedCache<LoadableTestResource>* c) {
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

TEST_CASE("ScopedCache multithread stress") {
	ScopedCache<LoadableTestResource> c;
	std::thread t0(cacheUseTask, &c);
	std::thread t1(cacheUseTask, &c);
	std::thread t2(cacheUseTask, &c);
	t0.join();
	t1.join();
	t2.join();
	CHECK(c.size() > 0);
	c.cleanup();
	CHECK(c.size() == 0);
}
