// File: PropertyMapTest.cpp, Created on 2014. december 4. 17:30, Author: Vader

#include <catch.hpp>

#include <string>

#include <libv/property_map.hpp>

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

// PropertyMap -------------------------------------------------------------------------------------

TEST_CASE("set") {
	libv::PropertyMap map;
	libv::PropertyMap::Key<int> a0;

	map.set(a0, 42);

	CHECK(42 == map.get(a0).value());
}

TEST_CASE("setter proxy") {
	libv::PropertyMap map;
	libv::PropertyMap::Key<int> a0;
	libv::PropertyMap::Key<int> a1;

	map.set()
		(a0, 42)
		(a1, 43)
	;

	CHECK(42 == map.get(a0).value());
	CHECK(43 == map.get(a1).value());
}

TEST_CASE("differentType") {
	libv::PropertyMap map;
	libv::PropertyMap::Key<int> a0;
	libv::PropertyMap::Key<std::string> a1;

	CHECK(map.get(a0).is_initialized() == false);
	CHECK(map.get(a1).is_initialized() == false);

	map.set(a0, 42);
	map.set(a1, "xyz");

	CHECK(42 == map.get(a0).value());
	CHECK("xyz" == map.get(a1).value());

	CHECK(42 == map.get(a0).value());
	CHECK("xyz" == map.get(a1).value());
}

TEST_CASE("remove") {
	libv::PropertyMap map;
	libv::PropertyMap::Key<int> a0;

	CHECK(map.get(a0).is_initialized() == false);
	map.set(a0, 42);
	CHECK(42 == map.get(a0).value());
	map.remove(a0);
	CHECK(map.get(a0).is_initialized() == false);
}