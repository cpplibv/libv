// Created by Vader on 2023-02-24.

#pragma once

// ext
// #include <fmt/format.h>


// namespace libv {
//
// // -------------------------------------------------------------------------------------------------
//
// template <typename T>
// struct LocNumber {
// 	value_or_ref<T> var;
// };
//
// template <typename T>
// 	is_number<T>
// auto loc(T&& var) {
// 	return LocNumber<T>(std::forward<T>(var));
// }
//
// } // namespace libv --------------------------------------------------------------------------------
//
//
// void foo() {
//
// 	// format spec:
// 	//		<empty> => format underlying
// 	//		<empty> [":" underlying_fmt_spec] => format underlying
// 	//		"v" [":" underlying_fmt_spec] => format underlying
// 	//
// 	//		"x" {"|" <string>} * [":" underlying_fmt_spec] => format underlying
// 	//		"x" {"|" <string>} * [":" underlying_fmt_spec] => format underlying
// 	//
// 	//		"z" "?" <string> => if number == 0 then format <string>
// 	//		"q" "?" <string> => if number != 0 then format <string>
// 	//		"-" "?" <string> => if number < 0 then format <string>
// 	//		"+" "?" <string> => if number > 0 then format <string>
// 	//		"w" "?" <string> => if number == |1| then format <string>
// 	//		"s" "?" <string> => if number != |1| then format <string>
// 	//		"z" "?" <string> => if number > |1| then format <string>
// 	//		"d" "?" <string> => if |2| <= number <= |5| then format <string>
// 	//		"p" "?" <string> => if number > |5| then format <string>
//
// 	CHECK(fmt::format("{0:} byte{0:s?s}", libv::loc(1)) == "1 byte");
// 	CHECK(fmt::format("{0:} byte{0:s?s}", libv::loc(42)) == "42 bytes");
// 	CHECK(fmt::format("{0::04} byte{0:s?s}", libv::loc(1)) == "0001 byte");
// 	CHECK(fmt::format("{0::04} byte{0:s?s}", libv::loc(42)) == "0042 bytes");
// 	CHECK(fmt::format("{:x|s byte|p bytes:04}", libv::loc(1)) == "0001 bytes");
// 	CHECK(fmt::format("{:x|s byte|p bytes:04}", libv::loc(42)) == "0042 bytes");
// 	// CHECK(fmt::format("{:x|{04} byte|{04} bytes}", libv::loc(1)) == "0001 bytes");
// 	// CHECK(fmt::format("{:x|{04} byte|{04} bytes}", libv::loc(42)) == "0042 bytes");
// }
//
//
// template <std::size_t N, typename T>
// struct fmt::formatter<libv::vec_t<N, T>, char, void> : public fmt::formatter<T, char, void> {
//
// 	template <typename ParseContext>
// 	constexpr auto parse(ParseContext& ctx) {
// 	}
//
// 	template <typename FormatContext>
// 	auto format(const libv::vec_t<N, T>& vec, FormatContext& ctx) const {
// 	}
// };

// -------------------------------------------------------------------------------------------------
