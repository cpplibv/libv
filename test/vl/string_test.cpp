// File: string_test.cpp, Created on 2015. március 25. 22:12, Author: Vader

#include <catch.hpp>

#include "vl/string.hpp"

//SUITE(SplitCommandLineTest) {
//	TEST_CASE("splitChar") {
//		auto result = vl::splitCommandLine("123 45 6789");
//
//		CHECK(3u == result.size());
//		CHECK("123" == result[0]);
//		CHECK("45" == result[1]);
//		CHECK("6789" == result[2]);
//	}
//
//	TEST_CASE("splitString") {
//		auto result = vl::splitCommandLine(std::string("123 45 6789"));
//
//		CHECK(3u == result.size());
//		CHECK("123" == result[0]);
//		CHECK("45" == result[1]);
//		CHECK("6789" == result[2]);
//	}
//
//	TEST_CASE("spliOne") {
//		auto result = vl::splitCommandLine("12(345,6789");
//
//		CHECK(1u == result.size());
//		CHECK("12(345,6789" == result[0]);
//	}
//
//	TEST_CASE("emitEmpty") {
//		auto result = vl::splitCommandLine(std::string("1  2 3"));
//
//		CHECK(3u == result.size());
//		CHECK("1" == result[0]);
//		CHECK("2" == result[1]);
//		CHECK("3" == result[2]);
//	}
//
//	TEST_CASE("keepMarkedTogether") {
//		auto result = vl::splitCommandLine(std::string("1 \"2 3\""));
//
//		CHECK(2u == result.size());
//		CHECK("1" == result[0]);
//		CHECK("\"2 3\"" == result[1]);
//	}
//}