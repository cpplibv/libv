// File: PropertyMapTest.cpp, Created on 2014. december 4. 17:30, Author: Vader

#include <catch.hpp>

#include <string>

#include <libv/multi_type_map.hpp>

// MultiTypeMap -------------------------------------------------------------------------------------

TEST_CASE("at with not found element") {
	libv::MultiTypeMap map;
	libv::MultiTypeMap::Key<int> a0;

	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
}

TEST_CASE("insertion") {
	libv::MultiTypeMap map;
	libv::MultiTypeMap::Key<int> a0;
	libv::MultiTypeMap::Key<std::string> a1;

	SECTION("operator[]") {
		map[a0] = 3;
		map[a1] = "xy";
	}
	SECTION("insert") {
		map.insert(a0, 3);
		map.insert(a1, std::string("xy"));
	}
	SECTION("emplace") {
		map.emplace(a0, 3);
		map.emplace(a1, "xy");
	}

	CHECK(map.at(a0) == 3);
	CHECK(map.at(a1) == "xy");
}

TEST_CASE("erase") {
	libv::MultiTypeMap map;
	libv::MultiTypeMap::Key<int> a0;

	map[a0] = 3;

	CHECK(map.at(a0) == 3);

	map.erase(a0);

	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
}
