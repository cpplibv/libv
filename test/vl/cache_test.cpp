// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>
#include <vl/cache.hpp>
#include "cache_test_util.hpp"

int TestRA::instanceNumber = 0;
int TestRB::instanceNumber = 0;
int TestRM::instanceNumber = 0;

using vl::Cache;
using vl::LoaderCache;

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
	NonCopyable a0;
	auto v0 = rc.get(a0);
}

TEST_CASE("Cache should handle non copyable argument prvalue reference", "[static]") {
	Cache<TestRNonCopyArged> rc;
	auto v0 = rc.get(NonCopyable());
}

TEST_CASE("Cache should handle non copyable argument rvalue reference", "[static]") {
	Cache<TestRNonCopyArged> rc;
	NonCopyable a0;
	auto v0 = rc.get(std::move(a0));
}

// vl::use -----------------------------------------------------------------------------------------

TEST_CASE("Cache comparator can handle use option for single argument") {
	Cache<LoadableTestResourceB> rc;
	auto r0 = rc.get<vl::use<0>>(0, 1, 2);
	auto r1 = rc.get<vl::use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 2u);
}

TEST_CASE("Cache comparator can handle use option for match even different pos arguments") {
	Cache<LoadableTestResourceB> rc;
	auto r0 = rc.get<vl::use<0>>(42, 1, 2);
	auto r1 = rc.get<vl::use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.get() == r1.get());
	CHECK(r0->n == 42);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 42);
	CHECK(r1->argCount == 3u);
}

TEST_CASE("LoaderCache comparator can handle use option for single argument") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<vl::use<0>>(0, 1, 2);
	auto r1 = rc.get<vl::use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 2u);
}

TEST_CASE("LoaderCache comparator can handle use option for match even different pos arguments") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<vl::use<0>>(42, 1, 2);
	auto r1 = rc.get<vl::use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.get() == r1.get());
	CHECK(r0->n == 42);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 42);
	CHECK(r1->argCount == 3u);
}

// vl::ignore -------------------------------------------------------------------------------

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