// File: ResourceManagerTest.cpp, Created on 2015. február 26. 10:31, Author: Vader

#include <catch.hpp>

#include "resource_test_util.hpp"

#include <vl/resource.hpp>

#include <chrono>

// -------------------------------------------------------------------------------------------------

using namespace vl;

TEST_CASE("Cache can provide instances") {
	Cache<TestResource> rc;
	auto r0 = rc.get(4);
	auto r1 = rc.get(5);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
}

TEST_CASE("LoaderCache can provide instances") {
	LoaderCache<LoadableTestResource> rc;
	auto r0 = rc.get(10);
	auto r1 = rc.get(25);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
}

TEST_CASE("LoaderCache will load") {
	int unloaded = 0;
	LoaderCache<LoadableTestResource> rc;
	auto r0 = rc.get(10);

	CHECK(r0->loaded == 1);

	r0->unloaded = &unloaded;
	r0.reset();

	CHECK(unloaded == 1);
}

//TEST_CASE("Cache comparator can handle use option for single argument") {
//	Cache<Resource> rc;
//	auto r0 = rc.get<use<1>>(0, 1, 2);
//	auto r1 = rc.get<use<0>>(0, 1);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}
//
//TEST_CASE("Cache comparator can handle use option for multiple argument") {
//	Cache<Resource> rc;
//	auto r0 = rc.get<use<0, 2>>(0, 1, 2);
//	auto r1 = rc.get<use<1, 2>>(0, 1, 2, 3);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}
//
//TEST_CASE("Cache comparator can handle ignore option for single argument") {
//	Cache<Resource> rc;
//	auto r0 = rc.get<ignore<1>>(0, 1, 2);
//	auto r1 = rc.get<ignore<0>>(0, 1);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}
//
//TEST_CASE("Cache comparator can handle ignore option for multiple argument") {
//	Cache<Resource> rc;
//	auto r0 = rc.get<ignore<0, 2>>(0, 1, 2);
//	auto r1 = rc.get<ignore<1, 2>>(0, 1, 2, 3);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}

TEST_CASE("LoaderCache comparator can handle use option with one argument") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<use<0>>(0);
	auto r1 = rc.get<use<0>>(5);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 1u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 1u);
}

TEST_CASE("LoaderCache comparator can handle use option for single argument") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<use<0>>(0, 1, 2);
	auto r1 = rc.get<use<0>>(5, 6);

	CHECK(rc.size() == 2u);
	CHECK(r0.get() != r1.get());
	CHECK(r0->n == 0);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 5);
	CHECK(r1->argCount == 2u);
}

TEST_CASE("LoaderCache comparator can handle use option for match even different arguments") {
	LoaderCache<LoadableTestResourceB> rc;
	auto r0 = rc.get<use<2>>(42, 1, 2); // use<2> does not matter due to cache is empty
	auto r1 = rc.get<use<1>>(5, 42);

	CHECK(rc.size() == 1u);
	CHECK(r0.get() == r1.get());
	CHECK(r0->n == 42);
	CHECK(r0->argCount == 3u);
	CHECK(r1->n == 42);
	CHECK(r1->argCount == 3u);
}

//TEST_CASE("LoaderCache comparator can handle use option for multiple argument") {
//	LoaderCache<Resource> rc;
//	auto r0 = rc.get<use<0, 2>>(0, 1, 2);
//	auto r1 = rc.get<use<1, 2>>(0, 1, 2, 3);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}
//
//TEST_CASE("LoaderCache comparator can handle ignore option for single argument") {
//	LoaderCache<Resource> rc;
//	auto r0 = rc.get<ignore<1>>(0, 1, 2);
//	auto r1 = rc.get<ignore<0>>(0, 1);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}
//
//TEST_CASE("LoaderCache comparator can handle ignore option for multiple argument") {
//	LoaderCache<Resource> rc;
//	auto r0 = rc.get<ignore<0, 2>>(0, 1, 2);
//	auto r1 = rc.get<ignore<1, 2>>(0, 1, 2, 3);
//	CHECK(rc.size() == 2u);
//	CHECK(r0.get() != r1.get());
//}