// File: ResourceManagerTest.cpp, Created on 2015. február 26. 10:31, Author: Vader

#include <catch.hpp>

#include "vl/resource_manager.hpp"

// -------------------------------------------------------------------------------------------------

using vl::TestResource;
using vl::TestResourceManager;

TEST_CASE("Resource Manager can provide instance which avoids duplication") {
	TestResourceManager rm0;
	TestResource tr0(rm0, 42);
	TestResource tr1(rm0, 42);
	TestResource tr2(rm0, 40);

	CHECK(tr0.ioData.get() == tr1.ioData.get());
	CHECK(tr0.ioData.get() != tr2.ioData.get());
	CHECK(tr0.glData.get() == tr1.glData.get());
	CHECK(tr0.glData.get() != tr2.glData.get());
}

TEST_CASE("") {
}