// File: VaridicMapTest.cpp, Created on 2014. december 4. 17:30, Author: Vader

#include <catch.hpp>
#include <string>
#include "vl/varidic_map.hpp"

TEST_CASE("set") {
	vl::VaridicMap map;

	vl::VaridicMap::Address<int> a0(0);
	map.set(a0, 42);
	CHECK(42 == map.at(a0));
}

TEST_CASE("defaultValue") {
	vl::VaridicMap map;
	vl::VaridicMap::Address<int> a0(0);
	vl::VaridicMap::Address<int> a1(42);

	CHECK(0 == map.get(a0));
	CHECK(42 == map.get(a1));

	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
	CHECK_THROWS_AS(map.at(a1), std::out_of_range);
}

TEST_CASE("differentType") {
	vl::VaridicMap map;
	vl::VaridicMap::Address<int> a0(0);
	vl::VaridicMap::Address<std::string> a1("test");

	CHECK(0 == map.get(a0));
	CHECK("test" == map.get(a1));

	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
	CHECK_THROWS_AS(map.at(a1), std::out_of_range);

	map.set(a0, 42);
	map.set(a1, "xyz");

	CHECK(42 == map.get(a0));
	CHECK("xyz" == map.get(a1));

	CHECK(42 == map.at(a0));
	CHECK("xyz" == map.at(a1));
}

TEST_CASE("remove") {
	vl::VaridicMap map;
	vl::VaridicMap::Address<int> a0(0);

	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
	map.set(a0, 42);
	CHECK(42 == map.at(a0));
	map.remove(a0);
	CHECK_THROWS_AS(map.at(a0), std::out_of_range);
}