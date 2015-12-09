// File: ResourceManagerTest.cpp, Created on 2015. február 26. 10:31, Author: Vader

#include <catch.hpp>

#include "resource_test_util.hpp"

#include <vl/cache.hpp>

// -------------------------------------------------------------------------------------------------

using namespace vl;

TEST_CASE("Cache comparator can handle ignore option for single argument") {

	ResourceService rs;
	LoaderCache<TestResource> rc;
	auto r0 = rc.get<use < 1 >> (&rs, 1);
	auto r1 = rc.get<use < 1 >> (&rs, 2);
	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());

	CHECK(rs.cacheDataGL.size() == 2u);
	CHECK(rs.cacheDataIO.size() == 2u);

	CHECK(r0->depsLoaded == true);
	CHECK(r1->depsLoaded == true);
}