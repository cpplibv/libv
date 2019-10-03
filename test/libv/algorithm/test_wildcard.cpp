// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// std
#include <cstdint>
// pro
#include <libv/algorithm/wildcard.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("match_wildcard", "[libv.algorithm.wildcard]") {
	CHECK(libv::match_wildcard("", ""));
	CHECK(not libv::match_wildcard("a", ""));

	CHECK(libv::match_wildcard("a", "a"));
	CHECK(libv::match_wildcard("a", "?"));
	CHECK(libv::match_wildcard("a", "*"));

	CHECK(not libv::match_wildcard("a", "aa"));
	CHECK(not libv::match_wildcard("a", "a?"));
	CHECK(libv::match_wildcard("a", "a*"));
	CHECK(not libv::match_wildcard("a", "aa"));
	CHECK(not libv::match_wildcard("a", "?a"));
	CHECK(libv::match_wildcard("a", "*a"));
	CHECK(not libv::match_wildcard("ab", "aa"));
	CHECK(libv::match_wildcard("ab", "a?"));
	CHECK(libv::match_wildcard("ab", "a*"));

	CHECK(not libv::match_wildcard("a", "aaa"));
	CHECK(not libv::match_wildcard("a", "a?a"));
	CHECK(not libv::match_wildcard("a", "a*a"));
	CHECK(not libv::match_wildcard("ab", "aaa"));
	CHECK(not libv::match_wildcard("ab", "a?a"));
	CHECK(not libv::match_wildcard("ab", "a*a"));

	CHECK(not libv::match_wildcard("aba", "aaa"));
	CHECK(libv::match_wildcard("aba", "a?a"));
	CHECK(libv::match_wildcard("aba", "a*a"));
	CHECK(libv::match_wildcard("aaa", "aaa"));
	CHECK(libv::match_wildcard("aaa", "a?a"));
	CHECK(libv::match_wildcard("aaa", "a*a"));

	CHECK(not libv::match_wildcard("aba", "?a?"));
	CHECK(libv::match_wildcard("aba", "*a*"));
	CHECK(libv::match_wildcard("aaa", "?a?"));
	CHECK(libv::match_wildcard("aaa", "*a*"));
	CHECK(libv::match_wildcard("bab", "?a?"));
	CHECK(libv::match_wildcard("bab", "*a*"));

	CHECK(not libv::match_wildcard("aaa", "??"));
	CHECK(libv::match_wildcard("aaa", "**"));
	CHECK(libv::match_wildcard("aa", "??"));
	CHECK(libv::match_wildcard("aa", "**"));
	CHECK(not libv::match_wildcard("a", "??"));
	CHECK(libv::match_wildcard("a", "**"));
	CHECK(not libv::match_wildcard("", "??"));
	CHECK(libv::match_wildcard("", "**"));

	CHECK(libv::match_wildcard("aaa", "*?"));
	CHECK(libv::match_wildcard("aaa", "?*"));
	CHECK(libv::match_wildcard("aa", "?*"));
	CHECK(libv::match_wildcard("aa", "?*"));
	CHECK(libv::match_wildcard("a", "?*"));
	CHECK(libv::match_wildcard("a", "?*"));
	CHECK(not libv::match_wildcard("", "?*"));
	CHECK(not libv::match_wildcard("", "?*"));

	CHECK(libv::match_wildcard("aaa", "*?a"));
	CHECK(libv::match_wildcard("aaa", "?*a"));
	CHECK(libv::match_wildcard("aa", "?*a"));
	CHECK(libv::match_wildcard("aa", "?*a"));
	CHECK(not libv::match_wildcard("a", "?*a"));
	CHECK(not libv::match_wildcard("a", "?*a"));
	CHECK(not libv::match_wildcard("", "?*a"));
	CHECK(not libv::match_wildcard("", "?*a"));

	CHECK(libv::match_wildcard("vec3f", "vec?*f"));
	CHECK(libv::match_wildcard("vec2i", "vec?*"));
	CHECK(libv::match_wildcard("test_erase_if_unstable.cpp", "test_*if*.cpp"));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("match_wildcard_layer", "[libv.algorithm.wildcard]") {
	CHECK(libv::match_wildcard_layer("", ""));
	CHECK(not libv::match_wildcard_layer("a", ""));
	CHECK(not libv::match_wildcard_layer("/", ""));
	CHECK(not libv::match_wildcard_layer("a/", ""));
	CHECK(not libv::match_wildcard_layer("/a", ""));

	CHECK(libv::match_wildcard_layer("a", "a"));
	CHECK(libv::match_wildcard_layer("a", "?"));
	CHECK(libv::match_wildcard_layer("a", "*"));
	CHECK(libv::match_wildcard_layer("a", "$"));

	CHECK(not libv::match_wildcard_layer("aa", "a"));
	CHECK(not libv::match_wildcard_layer("aa", "?"));
	CHECK(libv::match_wildcard_layer("aa", "*"));
	CHECK(libv::match_wildcard_layer("aa", "$"));

	CHECK(not libv::match_wildcard_layer("a/", "a"));
	CHECK(not libv::match_wildcard_layer("a/", "?"));
	CHECK(not libv::match_wildcard_layer("a/", "*"));
	CHECK(libv::match_wildcard_layer("a/", "$"));
	CHECK(not libv::match_wildcard_layer("/a", "a"));
	CHECK(not libv::match_wildcard_layer("/a", "?"));
	CHECK(not libv::match_wildcard_layer("/a", "*"));
	CHECK(libv::match_wildcard_layer("/a", "$"));

	CHECK(not libv::match_wildcard_layer("a", "aa"));
	CHECK(not libv::match_wildcard_layer("a", "a?"));
	CHECK(libv::match_wildcard_layer("a", "a*"));
	CHECK(libv::match_wildcard_layer("a", "a$"));
	CHECK(not libv::match_wildcard_layer("a", "aa"));
	CHECK(not libv::match_wildcard_layer("a", "?a"));
	CHECK(libv::match_wildcard_layer("a", "*a"));
	CHECK(libv::match_wildcard_layer("a", "$a"));
	CHECK(not libv::match_wildcard_layer("ab", "aa"));
	CHECK(libv::match_wildcard_layer("ab", "a?"));
	CHECK(libv::match_wildcard_layer("ab", "a*"));
	CHECK(libv::match_wildcard_layer("ab", "a$"));

	CHECK(not libv::match_wildcard_layer("a/", "aa"));
	CHECK(not libv::match_wildcard_layer("a/", "a?"));
	CHECK(not libv::match_wildcard_layer("a/", "a*"));
	CHECK(libv::match_wildcard_layer("a/", "a$"));
	CHECK(not libv::match_wildcard_layer("a/", "aa"));
	CHECK(not libv::match_wildcard_layer("a/", "?a"));
	CHECK(not libv::match_wildcard_layer("a/", "*a"));
	CHECK(not libv::match_wildcard_layer("a/", "$a"));
	CHECK(not libv::match_wildcard_layer("ab/", "aa"));
	CHECK(not libv::match_wildcard_layer("ab/", "a?"));
	CHECK(not libv::match_wildcard_layer("ab/", "a*"));
	CHECK(libv::match_wildcard_layer("ab/", "a$"));

	CHECK(not libv::match_wildcard_layer("/a", "aa"));
	CHECK(not libv::match_wildcard_layer("/a", "a?"));
	CHECK(not libv::match_wildcard_layer("/a", "a*"));
	CHECK(not libv::match_wildcard_layer("/a", "a$"));
	CHECK(not libv::match_wildcard_layer("/a", "aa"));
	CHECK(not libv::match_wildcard_layer("/a", "?a"));
	CHECK(not libv::match_wildcard_layer("/a", "*a"));
	CHECK(libv::match_wildcard_layer("/a", "$a"));
	CHECK(not libv::match_wildcard_layer("/ab", "aa"));
	CHECK(not libv::match_wildcard_layer("/ab", "a?"));
	CHECK(not libv::match_wildcard_layer("/ab", "a*"));
	CHECK(not libv::match_wildcard_layer("/ab", "a$"));

	CHECK(not libv::match_wildcard_layer("a", "aaa"));
	CHECK(not libv::match_wildcard_layer("a", "a?a"));
	CHECK(not libv::match_wildcard_layer("a", "a*a"));
	CHECK(not libv::match_wildcard_layer("a", "a$a"));
	CHECK(not libv::match_wildcard_layer("ab", "aaa"));
	CHECK(not libv::match_wildcard_layer("ab", "a?a"));
	CHECK(not libv::match_wildcard_layer("ab", "a*a"));
	CHECK(not libv::match_wildcard_layer("ab", "a$a"));

	CHECK(not libv::match_wildcard_layer("aba", "aaa"));
	CHECK(libv::match_wildcard_layer("aba", "a?a"));
	CHECK(libv::match_wildcard_layer("aba", "a*a"));
	CHECK(libv::match_wildcard_layer("aba", "a$a"));
	CHECK(libv::match_wildcard_layer("aaa", "aaa"));
	CHECK(libv::match_wildcard_layer("aaa", "a?a"));
	CHECK(libv::match_wildcard_layer("aaa", "a*a"));
	CHECK(libv::match_wildcard_layer("aaa", "a$a"));

	CHECK(not libv::match_wildcard_layer("a/a", "aaa"));
	CHECK(not libv::match_wildcard_layer("a/a", "a?a"));
	CHECK(not libv::match_wildcard_layer("a/a", "a*a"));
	CHECK(libv::match_wildcard_layer("a/a", "a$a"));

	CHECK(libv::match_wildcard_layer("a/a", "a/a"));
	CHECK(libv::match_wildcard_layer("a/a", "a/?"));
	CHECK(not libv::match_wildcard_layer("a/a", "a/?a"));
	CHECK(libv::match_wildcard_layer("a/a", "a/*a"));
	CHECK(libv::match_wildcard_layer("a/a", "a/$a"));

	CHECK(not libv::match_wildcard_layer("aba", "?a?"));
	CHECK(libv::match_wildcard_layer("aba", "*a*"));
	CHECK(libv::match_wildcard_layer("aba", "$a$"));
	CHECK(libv::match_wildcard_layer("aaa", "?a?"));
	CHECK(libv::match_wildcard_layer("aaa", "*a*"));
	CHECK(libv::match_wildcard_layer("aaa", "$a$"));
	CHECK(libv::match_wildcard_layer("bab", "?a?"));
	CHECK(libv::match_wildcard_layer("bab", "*a*"));
	CHECK(libv::match_wildcard_layer("bab", "$a$"));

	CHECK(not libv::match_wildcard_layer("a/", "?a?"));
	CHECK(not libv::match_wildcard_layer("a/", "*a*"));
	CHECK(libv::match_wildcard_layer("a/", "$a$"));
	CHECK(not libv::match_wildcard_layer("ab/", "?a?"));
	CHECK(not libv::match_wildcard_layer("ab/", "*a*"));
	CHECK(libv::match_wildcard_layer("ab/", "$a$"));
	CHECK(not libv::match_wildcard_layer("ba/", "?a?"));
	CHECK(not libv::match_wildcard_layer("ba/", "*a*"));
	CHECK(libv::match_wildcard_layer("ba/", "$a$"));
	CHECK(not libv::match_wildcard_layer("bab/", "?a?"));
	CHECK(not libv::match_wildcard_layer("bab/", "*a*"));
	CHECK(libv::match_wildcard_layer("bab/", "$a$"));

	CHECK(not libv::match_wildcard_layer("/a/", "?a?"));
	CHECK(not libv::match_wildcard_layer("/a/", "*a*"));
	CHECK(libv::match_wildcard_layer("/a/", "$a$"));
	CHECK(not libv::match_wildcard_layer("/ab/", "?a?"));
	CHECK(not libv::match_wildcard_layer("/ab/", "*a*"));
	CHECK(libv::match_wildcard_layer("/ab/", "$a$"));
	CHECK(not libv::match_wildcard_layer("/ba/", "?a?"));
	CHECK(not libv::match_wildcard_layer("/ba/", "*a*"));
	CHECK(libv::match_wildcard_layer("/ba/", "$a$"));
	CHECK(not libv::match_wildcard_layer("/bab/", "?a?"));
	CHECK(not libv::match_wildcard_layer("/bab/", "*a*"));
	CHECK(libv::match_wildcard_layer("/bab/", "$a$"));

	CHECK(not libv::match_wildcard_layer("/a/", "?a?"));
	CHECK(not libv::match_wildcard_layer("/a/", "*a*"));
	CHECK(libv::match_wildcard_layer("/a/", "$a$"));
	CHECK(not libv::match_wildcard_layer("/a/b", "?a?"));
	CHECK(not libv::match_wildcard_layer("/a/b", "*a*"));
	CHECK(libv::match_wildcard_layer("/a/b", "$a$"));
	CHECK(not libv::match_wildcard_layer("/b/a", "?a?"));
	CHECK(not libv::match_wildcard_layer("/b/a", "*a*"));
	CHECK(libv::match_wildcard_layer("/b/a", "$a$"));
	CHECK(not libv::match_wildcard_layer("/b/ab", "?a?"));
	CHECK(not libv::match_wildcard_layer("/b/ab", "*a*"));
	CHECK(libv::match_wildcard_layer("/b/ab", "$a$"));

	CHECK(not libv::match_wildcard_layer("aaa", "??"));
	CHECK(libv::match_wildcard_layer("aaa", "**"));
	CHECK(libv::match_wildcard_layer("aaa", "$$"));
	CHECK(libv::match_wildcard_layer("aa", "??"));
	CHECK(libv::match_wildcard_layer("aa", "**"));
	CHECK(libv::match_wildcard_layer("aa", "$$"));
	CHECK(not libv::match_wildcard_layer("a", "??"));
	CHECK(libv::match_wildcard_layer("a", "**"));
	CHECK(libv::match_wildcard_layer("a", "$$"));
	CHECK(not libv::match_wildcard_layer("", "??"));
	CHECK(not libv::match_wildcard_layer("", "**"));
	CHECK(libv::match_wildcard_layer("", "$$"));

	CHECK(not libv::match_wildcard_layer("aaa", "?/?"));
	CHECK(not libv::match_wildcard_layer("aaa", "*/*"));
	CHECK(not libv::match_wildcard_layer("aaa", "$/$"));
	CHECK(not libv::match_wildcard_layer("aa", "?/?"));
	CHECK(not libv::match_wildcard_layer("aa", "*/*"));
	CHECK(not libv::match_wildcard_layer("aa", "$/$"));
	CHECK(not libv::match_wildcard_layer("a", "?/?"));
	CHECK(not libv::match_wildcard_layer("a", "*/*"));
	CHECK(not libv::match_wildcard_layer("a", "$/$"));
	CHECK(not libv::match_wildcard_layer("", "?/?"));
	CHECK(not libv::match_wildcard_layer("", "*/*"));
	CHECK(not libv::match_wildcard_layer("", "$/$"));

	CHECK(not libv::match_wildcard_layer("a/a/aa", "?/?"));
	CHECK(not libv::match_wildcard_layer("a/a/aa", "*/*"));
	CHECK(libv::match_wildcard_layer("a/a/aa", "$/$"));
	CHECK(not libv::match_wildcard_layer("a/a/a", "?/?"));
	CHECK(not libv::match_wildcard_layer("a/a/a", "*/*"));
	CHECK(libv::match_wildcard_layer("a/a/a", "$/$"));
	CHECK(not libv::match_wildcard_layer("a/a/", "?/?"));
	CHECK(not libv::match_wildcard_layer("a/a/", "*/*"));
	CHECK(libv::match_wildcard_layer("a/a/", "$/$"));

	CHECK(not libv::match_wildcard_layer("a/aa", "?/?"));
	CHECK(libv::match_wildcard_layer("a/aa", "*/*"));
	CHECK(libv::match_wildcard_layer("a/aa", "$/$"));
	CHECK(libv::match_wildcard_layer("a/a", "?/?"));
	CHECK(libv::match_wildcard_layer("a/a", "*/*"));
	CHECK(libv::match_wildcard_layer("a/a", "$/$"));
	CHECK(not libv::match_wildcard_layer("a/", "?/?"));
	CHECK(libv::match_wildcard_layer("a/", "*/*"));
	CHECK(libv::match_wildcard_layer("a/", "$/$"));

	CHECK(not libv::match_wildcard_layer("/aaa", "?/?"));
	CHECK(libv::match_wildcard_layer("/aaa", "*/*"));
	CHECK(libv::match_wildcard_layer("/aaa", "$/$"));
	CHECK(not libv::match_wildcard_layer("/aa", "?/?"));
	CHECK(libv::match_wildcard_layer("/aa", "*/*"));
	CHECK(libv::match_wildcard_layer("/aa", "$/$"));
	CHECK(not libv::match_wildcard_layer("/a", "?/?"));
	CHECK(libv::match_wildcard_layer("/a", "*/*"));
	CHECK(libv::match_wildcard_layer("/a", "$/$"));
	CHECK(not libv::match_wildcard_layer("/", "?/?"));
	CHECK(libv::match_wildcard_layer("/", "*/*"));
	CHECK(libv::match_wildcard_layer("/", "$/$"));

	CHECK(libv::match_wildcard_layer("vec3f", "vec?*f"));
	CHECK(libv::match_wildcard_layer("vec2i", "vec?*"));
	CHECK(libv::match_wildcard_layer("test_erase_if_unstable.cpp", "test_*if*.cpp"));
	CHECK(not libv::match_wildcard_layer("test/test_erase_if_unstable.cpp", "test_*if*.cpp"));
	CHECK(libv::match_wildcard_layer("test/test_erase_if_unstable.cpp", "test/test_*if*.cpp"));
	CHECK(libv::match_wildcard_layer("test/test_erase_if_unstable.cpp", "*/test_*if*.cpp"));
	CHECK(libv::match_wildcard_layer("test/test_erase_if_unstable.cpp", "$/test_*if*.cpp"));
	CHECK(libv::match_wildcard_layer("test/test_erase_if_unstable.cpp", "$test_*if*.cpp"));
	CHECK(not libv::match_wildcard_layer("test/test_erase_unstable.cpp", "test/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_layer("test/test_erase_unstable.cpp", "*/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_layer("test/test_erase_unstable.cpp", "$/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_layer("test/test_erase_unstable.cpp", "$test_*if*.cpp"));
}

// -------------------------------------------------------------------------------------------------
