// File: bitflag.cpp - Created on 2018.04.12. 09:37 - Author: Vader

// hpp
#include <catch/catch.hpp>
// std
#include <string>
// pro
#include <libv/container/tuple_map.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("cs_map can be default constructed", "[cs_map]") {
	libv::tuple_map<int, std::string> map;

	CHECK(map.size() == 0);
	CHECK(map.capacity() == 0);
}

TEST_CASE("cs_map can be sized and resized", "[cs_map]") {
	libv::tuple_map<int, std::string> map{5};

	CHECK(map.size() == 5);
	CHECK(map.capacity() >= 5);

	SECTION("resizing bigger changes size and capacity") {
		map.resize(10);

		CHECK(map.size() == 10);
		CHECK(map.capacity() >= 10);
	}

	SECTION("resizing smaller changes size but not capacity") {
		map.resize(0);

		CHECK(map.size() == 0);
		CHECK(map.capacity() >= 5);
	}

	SECTION("reserving bigger changes capacity but not size") {
		map.reserve(10);

		CHECK(map.size() == 5);
		CHECK(map.capacity() >= 10);
	}

	SECTION("reserving smaller does not change size or capacity") {
		map.reserve(0);

		CHECK(map.size() == 5);
		CHECK(map.capacity() >= 5);
	}
}

TEST_CASE("cs_map can be emplaced", "[cs_map]") {
	libv::tuple_map<std::string, std::string> map;

	SECTION("in order") {
		map.emplace("key-0", "value-0");
		map.emplace("key-1", "value-1");
		map.emplace("key-2", "value-2");
	}

	SECTION("out of order") {
		map.emplace("key-1", "value-1");
		map.emplace("key-0", "value-0");
		map.emplace("key-2", "value-2");
	}

	SECTION("out of order 2") {
		map.emplace("key-2", "value-2");
		map.emplace("key-0", "value-0");
		map.emplace("key-1", "value-1");
	}

	SECTION("reverse order") {
		map.emplace("key-2", "value-2");
		map.emplace("key-1", "value-1");
		map.emplace("key-0", "value-0");
	}

	CHECK(map.size() == 3);
	CHECK(map.capacity() >= 3);

	CHECK(map.at<0>("key-0") == "value-0");
	CHECK(map.at<0>("key-1") == "value-1");
	CHECK(map.at<0>("key-2") == "value-2");
}

TEST_CASE("cs_map multi value", "[cs_map]") {
	libv::tuple_map<int, double, std::string> map;
	const auto& const_map = map;

	map.emplace(100, 3.14, "value");

	CHECK(map.at<0>(100) == 3.14);
	CHECK(map.at<1>(100) == "value");
	CHECK(std::is_same_v<decltype(map.at<0>(100)), double&>);
	CHECK(std::is_same_v<decltype(map.at<1>(100)), std::string&>);

	CHECK(const_map.at<0>(100) == 3.14);
	CHECK(const_map.at<1>(100) == "value");
	CHECK(std::is_same_v<decltype(const_map.at<0>(100)), const double&>);
	CHECK(std::is_same_v<decltype(const_map.at<1>(100)), const std::string&>);
}

TEST_CASE("cs_map element lookup", "[cs_map]") {
	libv::tuple_map<std::string, std::string> map;
	const auto& const_map = map;

	map.emplace("key", "value");

	CHECK(map.at<0>("key") == "value");
	CHECK(std::is_same_v<decltype(map.at<0>("key")), std::string&>);
	CHECK_THROWS_AS(map.at<0>("not_found"), std::out_of_range);

	CHECK(const_map.at<0>("key") == "value");
	CHECK(std::is_same_v<decltype(const_map.at<0>("key")), const std::string&>);
	CHECK_THROWS_AS(const_map.at<0>("not_found"), std::out_of_range);
}

// -------------------------------------------------------------------------------------------------
