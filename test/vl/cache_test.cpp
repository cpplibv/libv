// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>
#include "vl/cache.hpp"
#include "cache_test_util.hpp"

int TestRA::instanceNumber = 0;
int TestRB::instanceNumber = 0;

using vl::Cache;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Cache can provide instance") {
	Cache<TestRA> rc;
	CHECK(TestRA::instanceNumber == 0);

	auto v = rc.get(std::string("b"));

	CHECK("b" == v->getn());
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("Cache can provide instance from multiple argument") {
	// TODO
}

TEST_CASE("Cache can provide instance without duplication") {
	Cache<TestRA> rc;

	auto v0 = rc.get(std::string("b"));
	auto v1 = rc.get(std::string("b"));

	CHECK("b" == v0->getn());
	CHECK("b" == v1->getn());
	CHECK(v0.get() == v1.get());
	CHECK(TestRA::instanceNumber == 1);
}

TEST_CASE("Cache can provide different instances") {
	Cache<TestRA> rc;

	auto v0 = rc.get(std::string("a"));
	auto v1 = rc.get(std::string("b"));

	CHECK("a" == v0->getn());
	CHECK("b" == v1->getn());
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

