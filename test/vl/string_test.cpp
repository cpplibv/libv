// File: string_test.cpp, Created on 2015. március 25. 22:12, Author: Vader

#include <UnitTest++/UnitTest++.h>

#include "vl/string.hpp"

//SUITE(SplitCommandLineTest) {
//	TEST(splitChar) {
//		auto result = vl::splitCommandLine("123 45 6789");
//
//		CHECK_EQUAL(3u, result.size());
//		CHECK_EQUAL("123", result[0]);
//		CHECK_EQUAL("45", result[1]);
//		CHECK_EQUAL("6789", result[2]);
//	}
//
//	TEST(splitString) {
//		auto result = vl::splitCommandLine(std::string("123 45 6789"));
//
//		CHECK_EQUAL(3u, result.size());
//		CHECK_EQUAL("123", result[0]);
//		CHECK_EQUAL("45", result[1]);
//		CHECK_EQUAL("6789", result[2]);
//	}
//
//	TEST(spliOne) {
//		auto result = vl::splitCommandLine("12(345,6789");
//
//		CHECK_EQUAL(1u, result.size());
//		CHECK_EQUAL("12(345,6789", result[0]);
//	}
//
//	TEST(emitEmpty) {
//		auto result = vl::splitCommandLine(std::string("1  2 3"));
//
//		CHECK_EQUAL(3u, result.size());
//		CHECK_EQUAL("1", result[0]);
//		CHECK_EQUAL("2", result[1]);
//		CHECK_EQUAL("3", result[2]);
//	}
//
//	TEST(keepMarkedTogether) {
//		auto result = vl::splitCommandLine(std::string("1 \"2 3\""));
//
//		CHECK_EQUAL(2u, result.size());
//		CHECK_EQUAL("1", result[0]);
//		CHECK_EQUAL("\"2 3\"", result[1]);
//	}
//}