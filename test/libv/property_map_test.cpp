// File: PropertyMapTest.cpp, Created on 2014. december 4. 17:30, Author: Vader

#include <catch.hpp>
#include <string>
#include <libv/property_map.hpp>

TEST_CASE("set") {
	libv::PropertyMap map;

	libv::PropertyMap::Address<int> a0;
	map.set(a0, 42);
	CHECK(42 == *map.get(a0));
}

TEST_CASE("differentType") {
	libv::PropertyMap map;
	libv::PropertyMap::Address<int> a0;
	libv::PropertyMap::Address<std::string> a1;

	CHECK(map.get(a0).is_initialized() == false);
	CHECK(map.get(a1).is_initialized() == false);

	map.set(a0, 42);
	map.set(a1, "xyz");

	CHECK(42 == *map.get(a0));
	CHECK("xyz" == *map.get(a1));

	CHECK(42 == *map.get(a0));
	CHECK("xyz" == *map.get(a1));
}

TEST_CASE("remove") {
	libv::PropertyMap map;
	libv::PropertyMap::Address<int> a0;

	CHECK(map.get(a0).is_initialized() == false);
	map.set(a0, 42);
	CHECK(42 == *map.get(a0));
	map.remove(a0);
	CHECK(map.get(a0).is_initialized() == false);
}