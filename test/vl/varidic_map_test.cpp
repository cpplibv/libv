// File: VaridicMapTest.cpp, Created on 2014. december 4. 17:30, Author: Vader


#include <UnitTest++/UnitTest++.h>

#include <string>

#include "vl/varidic_map.hpp"
SUITE(VaridicMapTest) {
	TEST(set) {
		vl::VaridicMap map;

		vl::VaridicMap::Address<int> a0(0);
		map.set(a0, 42);
		CHECK(42 == map.at(a0));
	}
	TEST(defaultValue) {
		vl::VaridicMap map;
		vl::VaridicMap::Address<int> a0(0);
		vl::VaridicMap::Address<int> a1(42);

		CHECK(0 == map.get(a0));
		CHECK(42 == map.get(a1));

		CHECK_THROW(map.at(a0), std::out_of_range);
		CHECK_THROW(map.at(a1), std::out_of_range);
	}
	TEST(differentType) {
		vl::VaridicMap map;
		vl::VaridicMap::Address<int> a0(0);
		vl::VaridicMap::Address<std::string> a1("test");

		CHECK_EQUAL(0, map.get(a0));
		CHECK_EQUAL("test", map.get(a1));

		CHECK_THROW(map.at(a0), std::out_of_range);
		CHECK_THROW(map.at(a1), std::out_of_range);

		map.set(a0, 42);
		map.set(a1, "xyz");

		CHECK_EQUAL(42, map.get(a0));
		CHECK_EQUAL("xyz", map.get(a1));

		CHECK_EQUAL(42, map.at(a0));
		CHECK_EQUAL("xyz", map.at(a1));
	}
	TEST(remove) {
		vl::VaridicMap map;
		vl::VaridicMap::Address<int> a0(0);

		CHECK_THROW(map.at(a0), std::out_of_range);
		map.set(a0, 42);
		CHECK_EQUAL(42, map.at(a0));
		map.remove(a0);
		CHECK_THROW(map.at(a0), std::out_of_range);
	}
}