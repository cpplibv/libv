// Project: libv.utility, File: test/libv/utility/test_entity_ptr.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/cloud.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test Cloud simple set/get/remove", "[libv.utility.cloud]") {
	libv::Cloud cloud;

	int objectA = 0;
	int objectB = 0;

	CHECK(cloud.num_object() == 0);
	CHECK(cloud.get<int>() == nullptr);

	cloud.set<int>(&objectA);
	CHECK(cloud.num_object() == 1);
	CHECK(cloud.get<int>() == &objectA);

	cloud.set<int>(&objectB);
	CHECK(cloud.num_object() == 1);
	CHECK(cloud.get<int>() == &objectB);

	cloud.remove<int>();
	CHECK(cloud.num_object() == 0);
	CHECK(cloud.get<int>() == nullptr);
}

TEST_CASE("Test Cloud addressed set/get/remove", "[libv.utility.cloud]") {
	libv::Cloud cloud;

	int objectA = 0;
	int objectB = 0;

	CHECK(cloud.num_object() == 0);
	CHECK(cloud.get<int>("A") == nullptr);
	CHECK(cloud.get<int>("B") == nullptr);

	cloud.set<int>("A", &objectA);
	CHECK(cloud.num_object() == 1);
	CHECK(cloud.get<int>("A") == &objectA);
	CHECK(cloud.get<int>("B") == nullptr);

	cloud.set<int>("B", &objectB);
	CHECK(cloud.num_object() == 2);
	CHECK(cloud.get<int>("A") == &objectA);
	CHECK(cloud.get<int>("B") == &objectB);

	SECTION("Without simple names") {
		// Noop
	}
	SECTION("With simple names") {
		CHECK(cloud.num_object() == 2);
		CHECK(cloud.get<int>() == nullptr);

		cloud.set<int>(&objectA);
		CHECK(cloud.num_object() == 3);
		CHECK(cloud.get<int>() == &objectA);

		cloud.set<int>(&objectB);
		CHECK(cloud.num_object() == 3);
		CHECK(cloud.get<int>() == &objectB);

		cloud.remove<int>();
		CHECK(cloud.num_object() == 2);
		CHECK(cloud.get<int>() == nullptr);
	}

	cloud.set<int>("A", &objectB);
	CHECK(cloud.num_object() == 2);
	CHECK(cloud.get<int>("A") == &objectB);
	CHECK(cloud.get<int>("B") == &objectB);

	cloud.remove<int>("A");
	CHECK(cloud.num_object() == 1);
	CHECK(cloud.get<int>("A") == nullptr);
	CHECK(cloud.get<int>("B") == &objectB);

	cloud.remove<int>("B");
	CHECK(cloud.num_object() == 0);
	CHECK(cloud.get<int>("A") == nullptr);
	CHECK(cloud.get<int>("B") == nullptr);
}
