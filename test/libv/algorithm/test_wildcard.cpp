// Project: libv, File: test/libv/algorithm/test_wildcard.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// std
#include <cstdint>
// pro
#include <libv/algo/wildcard.hpp>


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

TEST_CASE("match_wildcard_glob", "[libv.algorithm.wildcard]") {
	CHECK(libv::match_wildcard_glob("", ""));
	CHECK(not libv::match_wildcard_glob("a", ""));
	CHECK(not libv::match_wildcard_glob("/", ""));
	CHECK(not libv::match_wildcard_glob("a/", ""));
	CHECK(not libv::match_wildcard_glob("/a", ""));

	CHECK(libv::match_wildcard_glob("a", "a"));
	CHECK(libv::match_wildcard_glob("a", "?"));
	CHECK(libv::match_wildcard_glob("a", "*"));
	CHECK(libv::match_wildcard_glob("a", "**"));

	CHECK(not libv::match_wildcard_glob("aa", "a"));
	CHECK(not libv::match_wildcard_glob("aa", "?"));
	CHECK(libv::match_wildcard_glob("aa", "*"));
	CHECK(libv::match_wildcard_glob("aa", "**"));

	CHECK(not libv::match_wildcard_glob("a/", "a"));
	CHECK(not libv::match_wildcard_glob("a/", "?"));
	CHECK(not libv::match_wildcard_glob("a/", "*"));
	CHECK(libv::match_wildcard_glob("a/", "**"));
	CHECK(not libv::match_wildcard_glob("/a", "a"));
	CHECK(not libv::match_wildcard_glob("/a", "?"));
	CHECK(not libv::match_wildcard_glob("/a", "*"));
	CHECK(libv::match_wildcard_glob("/a", "**"));

	CHECK(not libv::match_wildcard_glob("a", "aa"));
	CHECK(not libv::match_wildcard_glob("a", "a?"));
	CHECK(libv::match_wildcard_glob("a", "a*"));
	CHECK(libv::match_wildcard_glob("a", "a**"));
	CHECK(not libv::match_wildcard_glob("a", "aa"));
	CHECK(not libv::match_wildcard_glob("a", "?a"));
	CHECK(libv::match_wildcard_glob("a", "*a"));
	CHECK(libv::match_wildcard_glob("a", "**a"));
	CHECK(not libv::match_wildcard_glob("ab", "aa"));
	CHECK(libv::match_wildcard_glob("ab", "a?"));
	CHECK(libv::match_wildcard_glob("ab", "a*"));
	CHECK(libv::match_wildcard_glob("ab", "a**"));

	CHECK(not libv::match_wildcard_glob("a/", "aa"));
	CHECK(not libv::match_wildcard_glob("a/", "a?"));
	CHECK(not libv::match_wildcard_glob("a/", "a*"));
	CHECK(libv::match_wildcard_glob("a/", "a**"));
	CHECK(not libv::match_wildcard_glob("a/", "aa"));
	CHECK(not libv::match_wildcard_glob("a/", "?a"));
	CHECK(not libv::match_wildcard_glob("a/", "*a"));
	CHECK(not libv::match_wildcard_glob("a/", "**a"));
	CHECK(not libv::match_wildcard_glob("ab/", "aa"));
	CHECK(not libv::match_wildcard_glob("ab/", "a?"));
	CHECK(not libv::match_wildcard_glob("ab/", "a*"));
	CHECK(libv::match_wildcard_glob("ab/", "a**"));

	CHECK(not libv::match_wildcard_glob("/a", "aa"));
	CHECK(not libv::match_wildcard_glob("/a", "a?"));
	CHECK(not libv::match_wildcard_glob("/a", "a*"));
	CHECK(not libv::match_wildcard_glob("/a", "a**"));
	CHECK(not libv::match_wildcard_glob("/a", "aa"));
	CHECK(not libv::match_wildcard_glob("/a", "?a"));
	CHECK(not libv::match_wildcard_glob("/a", "*a"));
	CHECK(libv::match_wildcard_glob("/a", "**a"));
	CHECK(not libv::match_wildcard_glob("/ab", "aa"));
	CHECK(not libv::match_wildcard_glob("/ab", "a?"));
	CHECK(not libv::match_wildcard_glob("/ab", "a*"));
	CHECK(not libv::match_wildcard_glob("/ab", "a**"));

	CHECK(not libv::match_wildcard_glob("a", "aaa"));
	CHECK(not libv::match_wildcard_glob("a", "a?a"));
	CHECK(not libv::match_wildcard_glob("a", "a*a"));
	CHECK(not libv::match_wildcard_glob("a", "a**a"));
	CHECK(not libv::match_wildcard_glob("ab", "aaa"));
	CHECK(not libv::match_wildcard_glob("ab", "a?a"));
	CHECK(not libv::match_wildcard_glob("ab", "a*a"));
	CHECK(not libv::match_wildcard_glob("ab", "a**a"));

	CHECK(not libv::match_wildcard_glob("aba", "aaa"));
	CHECK(libv::match_wildcard_glob("aba", "a?a"));
	CHECK(libv::match_wildcard_glob("aba", "a*a"));
	CHECK(libv::match_wildcard_glob("aba", "a**a"));
	CHECK(libv::match_wildcard_glob("aaa", "aaa"));
	CHECK(libv::match_wildcard_glob("aaa", "a?a"));
	CHECK(libv::match_wildcard_glob("aaa", "a*a"));
	CHECK(libv::match_wildcard_glob("aaa", "a**a"));

	CHECK(not libv::match_wildcard_glob("a/a", "aaa"));
	CHECK(not libv::match_wildcard_glob("a/a", "a?a"));
	CHECK(not libv::match_wildcard_glob("a/a", "a*a"));
	CHECK(libv::match_wildcard_glob("a/a", "a**a"));

	CHECK(libv::match_wildcard_glob("a/a", "a/a"));
	CHECK(libv::match_wildcard_glob("a/a", "a/?"));
	CHECK(not libv::match_wildcard_glob("a/a", "a/?a"));
	CHECK(libv::match_wildcard_glob("a/a", "a/*a"));
	CHECK(libv::match_wildcard_glob("a/a", "a/**a"));

	CHECK(not libv::match_wildcard_glob("aba", "?a?"));
	CHECK(libv::match_wildcard_glob("aba", "*a*"));
	CHECK(libv::match_wildcard_glob("aba", "**a**"));
	CHECK(libv::match_wildcard_glob("aaa", "?a?"));
	CHECK(libv::match_wildcard_glob("aaa", "*a*"));
	CHECK(libv::match_wildcard_glob("aaa", "**a**"));
	CHECK(libv::match_wildcard_glob("bab", "?a?"));
	CHECK(libv::match_wildcard_glob("bab", "*a*"));
	CHECK(libv::match_wildcard_glob("bab", "**a**"));

	CHECK(not libv::match_wildcard_glob("a/", "?a?"));
	CHECK(not libv::match_wildcard_glob("a/", "*a*"));
	CHECK(libv::match_wildcard_glob("a/", "**a**"));
	CHECK(not libv::match_wildcard_glob("ab/", "?a?"));
	CHECK(not libv::match_wildcard_glob("ab/", "*a*"));
	CHECK(libv::match_wildcard_glob("ab/", "**a**"));
	CHECK(not libv::match_wildcard_glob("ba/", "?a?"));
	CHECK(not libv::match_wildcard_glob("ba/", "*a*"));
	CHECK(libv::match_wildcard_glob("ba/", "**a**"));
	CHECK(not libv::match_wildcard_glob("bab/", "?a?"));
	CHECK(not libv::match_wildcard_glob("bab/", "*a*"));
	CHECK(libv::match_wildcard_glob("bab/", "**a**"));

	CHECK(not libv::match_wildcard_glob("/a/", "?a?"));
	CHECK(not libv::match_wildcard_glob("/a/", "*a*"));
	CHECK(libv::match_wildcard_glob("/a/", "**a**"));
	CHECK(not libv::match_wildcard_glob("/ab/", "?a?"));
	CHECK(not libv::match_wildcard_glob("/ab/", "*a*"));
	CHECK(libv::match_wildcard_glob("/ab/", "**a**"));
	CHECK(not libv::match_wildcard_glob("/ba/", "?a?"));
	CHECK(not libv::match_wildcard_glob("/ba/", "*a*"));
	CHECK(libv::match_wildcard_glob("/ba/", "**a**"));
	CHECK(not libv::match_wildcard_glob("/bab/", "?a?"));
	CHECK(not libv::match_wildcard_glob("/bab/", "*a*"));
	CHECK(libv::match_wildcard_glob("/bab/", "**a**"));

	CHECK(not libv::match_wildcard_glob("/a/", "?a?"));
	CHECK(not libv::match_wildcard_glob("/a/", "*a*"));
	CHECK(libv::match_wildcard_glob("/a/", "**a**"));
	CHECK(not libv::match_wildcard_glob("/a/b", "?a?"));
	CHECK(not libv::match_wildcard_glob("/a/b", "*a*"));
	CHECK(libv::match_wildcard_glob("/a/b", "**a**"));
	CHECK(not libv::match_wildcard_glob("/b/a", "?a?"));
	CHECK(not libv::match_wildcard_glob("/b/a", "*a*"));
	CHECK(libv::match_wildcard_glob("/b/a", "**a**"));
	CHECK(not libv::match_wildcard_glob("/b/ab", "?a?"));
	CHECK(not libv::match_wildcard_glob("/b/ab", "*a*"));
	CHECK(libv::match_wildcard_glob("/b/ab", "**a**"));

	CHECK(not libv::match_wildcard_glob("aaa", "??"));
	CHECK(libv::match_wildcard_glob("aaa", "****"));
	CHECK(libv::match_wildcard_glob("aaa", "***"));
	CHECK(libv::match_wildcard_glob("aaa", "**"));
	CHECK(libv::match_wildcard_glob("aa", "??"));
	CHECK(libv::match_wildcard_glob("aa", "****"));
	CHECK(libv::match_wildcard_glob("aa", "***"));
	CHECK(libv::match_wildcard_glob("aa", "**"));
	CHECK(not libv::match_wildcard_glob("a", "??"));
	CHECK(libv::match_wildcard_glob("a", "****"));
	CHECK(libv::match_wildcard_glob("a", "***"));
	CHECK(libv::match_wildcard_glob("a", "**"));
	CHECK(not libv::match_wildcard_glob("", "??"));
	CHECK(libv::match_wildcard_glob("", "****"));
	CHECK(libv::match_wildcard_glob("", "***"));
	CHECK(libv::match_wildcard_glob("", "**"));

	CHECK(not libv::match_wildcard_glob("aaa", "?/?"));
	CHECK(not libv::match_wildcard_glob("aaa", "*/*"));
	CHECK(not libv::match_wildcard_glob("aaa", "**/**"));
	CHECK(not libv::match_wildcard_glob("aa", "?/?"));
	CHECK(not libv::match_wildcard_glob("aa", "*/*"));
	CHECK(not libv::match_wildcard_glob("aa", "**/**"));
	CHECK(not libv::match_wildcard_glob("a", "?/?"));
	CHECK(not libv::match_wildcard_glob("a", "*/*"));
	CHECK(not libv::match_wildcard_glob("a", "**/**"));
	CHECK(not libv::match_wildcard_glob("", "?/?"));
	CHECK(not libv::match_wildcard_glob("", "*/*"));
	CHECK(not libv::match_wildcard_glob("", "**/**"));

	CHECK(not libv::match_wildcard_glob("a/a/aa", "?/?"));
	CHECK(not libv::match_wildcard_glob("a/a/aa", "*/*"));
	CHECK(libv::match_wildcard_glob("a/a/aa", "**/**"));
	CHECK(not libv::match_wildcard_glob("a/a/a", "?/?"));
	CHECK(not libv::match_wildcard_glob("a/a/a", "*/*"));
	CHECK(libv::match_wildcard_glob("a/a/a", "**/**"));
	CHECK(not libv::match_wildcard_glob("a/a/", "?/?"));
	CHECK(not libv::match_wildcard_glob("a/a/", "*/*"));
	CHECK(libv::match_wildcard_glob("a/a/", "**/**"));

	CHECK(not libv::match_wildcard_glob("a/aa", "?/?"));
	CHECK(libv::match_wildcard_glob("a/aa", "*/*"));
	CHECK(libv::match_wildcard_glob("a/aa", "**/**"));
	CHECK(libv::match_wildcard_glob("a/a", "?/?"));
	CHECK(libv::match_wildcard_glob("a/a", "*/*"));
	CHECK(libv::match_wildcard_glob("a/a", "**/**"));
	CHECK(not libv::match_wildcard_glob("a/", "?/?"));
	CHECK(libv::match_wildcard_glob("a/", "*/*"));
	CHECK(libv::match_wildcard_glob("a/", "**/**"));

	CHECK(not libv::match_wildcard_glob("/aaa", "?/?"));
	CHECK(libv::match_wildcard_glob("/aaa", "*/*"));
	CHECK(libv::match_wildcard_glob("/aaa", "**/**"));
	CHECK(not libv::match_wildcard_glob("/aa", "?/?"));
	CHECK(libv::match_wildcard_glob("/aa", "*/*"));
	CHECK(libv::match_wildcard_glob("/aa", "**/**"));
	CHECK(not libv::match_wildcard_glob("/a", "?/?"));
	CHECK(libv::match_wildcard_glob("/a", "*/*"));
	CHECK(libv::match_wildcard_glob("/a", "**/**"));
	CHECK(not libv::match_wildcard_glob("/", "?/?"));
	CHECK(libv::match_wildcard_glob("/", "*/*"));
	CHECK(libv::match_wildcard_glob("/", "**/**"));

	CHECK(libv::match_wildcard_glob("vec3f", "vec?*f"));
	CHECK(libv::match_wildcard_glob("vec2i", "vec?*"));
	CHECK(libv::match_wildcard_glob("test_erase_if_unstable.cpp", "test_*if*.cpp"));
	CHECK(not libv::match_wildcard_glob("test/test_erase_if_unstable.cpp", "test_*if*.cpp"));
	CHECK(libv::match_wildcard_glob("test/test_erase_if_unstable.cpp", "test/test_*if*.cpp"));
	CHECK(libv::match_wildcard_glob("test/test_erase_if_unstable.cpp", "*/test_*if*.cpp"));
	CHECK(libv::match_wildcard_glob("test/test_erase_if_unstable.cpp", "**/test_*if*.cpp"));
	CHECK(libv::match_wildcard_glob("test/test_erase_if_unstable.cpp", "**test_*if*.cpp"));
	CHECK(not libv::match_wildcard_glob("test/test_erase_unstable.cpp", "test/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_glob("test/test_erase_unstable.cpp", "*/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_glob("test/test_erase_unstable.cpp", "**/test_*if*.cpp"));
	CHECK(not libv::match_wildcard_glob("test/test_erase_unstable.cpp", "**test_*if*.cpp"));

	CHECK(libv::match_wildcard_glob("testalgoif.cpp", "test**algo**if**.cpp"));
	CHECK(libv::match_wildcard_glob("test/algo/if/x.cpp", "test**algo**if**.cpp"));
	CHECK(libv::match_wildcard_glob("test_algo_if_x.cpp", "test**algo**if**.cpp"));

	CHECK(not libv::match_wildcard_glob("a", "\\\\"));
	CHECK(not libv::match_wildcard_glob("a", "\\"));
	CHECK(libv::match_wildcard_glob("\\", "\\\\"));
	CHECK(libv::match_wildcard_glob("\\", "\\"));
	CHECK(not libv::match_wildcard_glob("aa", "a\\\\"));
	CHECK(not libv::match_wildcard_glob("aa", "a\\"));
	CHECK(libv::match_wildcard_glob("a\\", "a\\\\"));
	CHECK(libv::match_wildcard_glob("a\\", "a\\"));
	CHECK(not libv::match_wildcard_glob("a", "\\\\a"));
	CHECK(not libv::match_wildcard_glob("\\a", "\\a"));
	CHECK(libv::match_wildcard_glob("\\a", "\\\\a"));
	CHECK(libv::match_wildcard_glob("a", "\\a"));

	CHECK(libv::match_wildcard_glob("?", "?"));
	CHECK(libv::match_wildcard_glob("?", "\\?"));
	CHECK(not libv::match_wildcard_glob("\\?", "\\?"));
	CHECK(libv::match_wildcard_glob("\\?", "\\\\?"));
	CHECK(libv::match_wildcard_glob("\\a", "\\\\?"));

	CHECK(libv::match_wildcard_glob("*", "*"));
	CHECK(libv::match_wildcard_glob("*", "\\*"));
	CHECK(not libv::match_wildcard_glob("\\*", "\\*"));
	CHECK(libv::match_wildcard_glob("\\*", "\\\\*"));
	CHECK(libv::match_wildcard_glob("\\a", "\\\\*"));

	CHECK(libv::match_wildcard_glob("**", "**"));
	CHECK(libv::match_wildcard_glob("**", "\\**"));
	CHECK(not libv::match_wildcard_glob("\\**", "\\**"));
	CHECK(libv::match_wildcard_glob("\\**", "\\\\**"));
	CHECK(libv::match_wildcard_glob("\\a", "\\\\**"));

	CHECK(libv::match_wildcard_glob("**", "**"));
	CHECK(libv::match_wildcard_glob("**", "\\*\\*"));
	CHECK(not libv::match_wildcard_glob("**a", "\\*\\*"));
	CHECK(not libv::match_wildcard_glob("\\**", "\\*\\*"));
	CHECK(libv::match_wildcard_glob("\\**", "\\\\*\\*"));
	CHECK(not libv::match_wildcard_glob("\\a", "\\\\*\\*"));
	CHECK(libv::match_wildcard_glob("\\*", "\\\\*\\*"));
	CHECK(not libv::match_wildcard_glob("\\a", "\\\\*\\*a"));
	CHECK(libv::match_wildcard_glob("\\a*a", "\\\\*\\*a"));
	CHECK(libv::match_wildcard_glob("\\a", "\\\\*\\a"));
	CHECK(not libv::match_wildcard_glob("\\*", "\\\\*\\a"));
}

// -------------------------------------------------------------------------------------------------
