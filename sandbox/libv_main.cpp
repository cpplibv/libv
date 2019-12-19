// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
//#include <boost/container/flat_map.hpp>
//#include <boost/container/flat_set.hpp>
//#include <boost/container/small_vector.hpp>
//#include <boost/fusion/adapted/std_pair.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/spirit/home/x3.hpp>
//#include <boost/spirit/home/x3/support/ast/variant.hpp>
//#include <boost/variant.hpp>
//#include <range/v3/algorithm/copy.hpp>
//#include <range/v3/algorithm/for_each.hpp>
//#include <range/v3/all.hpp>
//#include <range/v3/core.hpp>
//#include <range/v3/view/reverse.hpp>
// libv
//#include <libv/parse/color.hpp>
//#include <libv/parse/text_tag.hpp>
//#include <libv/range/view_uft8_codepoints.hpp>
//#include <libv/utility/endian.hpp>
//#include <libv/utility/hex_dump.hpp>
// std
//#include <algorithm>
//#include <chrono>
//#include <ctime>
//#include <fstream>
//#include <functional>
//#include <map>
//#include <memory>
//#include <set>
//#include <string>
//#include <unordered_map>
//#include <utility>
//#include <vector>

//http://ciere.com/cppnow15/x3_docs/index.html
//http://ciere.com/cppnow15/x3_docs/spirit/tutorials/semantic_actions.html
//https://www.boost.org/doc/libs/1_67_0/libs/spirit/doc/x3/html/index.html
//
//	std::string in = ""
//			"hjt"
//			"[color=1]"
//			"f\\e\\[col\\or=25]"
//			"[color=fe]"
//			"[/tag]"
//			"2gre"
//			"[/tag]"
//			"gre [non_tag]4\\5°t\\\\r"
//			"[tag]"
//			"freḼơᶉëᶆ ȋṕšᶙṁ ḍ"
//			"[tag=\\\"gre\\\"]"
//			"[tag=]"
//			"ỡḽǭᵳ ʂǐť ӓṁệẗ, ĉṓɲṩḙċťᶒ"
//			"[tag=\"țûɾ ấɖḯƥĭṩčįɳ]ġ >\\]<ḝłįʈ, \\\\]șếᶑ] \"]"
//			"[tag=\"țûɾ ấɖḯƥĭṩčįɳ]ġ \"ᶁⱺ ẽḭŭŝḿꝋď ]ṫĕᶆᶈṓɍ ỉñḉīḑȋᵭṵńť ṷŧ ḹẩḇőꝛế éȶ đôȇ ᵯáᶇā ąⱡîɋṹẵ.";
//
////	for (const auto& ch : in | libv::view::uft8_codepoints) {
////		std::cout << ch << ' ';
////	}
//
//	const auto parsed_values = libv::parse::parseTextTag(in);
//
//	for (const auto& v : parsed_values) {
//		std::cout << (!v.close ? " tag open: " : "tag close: ") << v.name << (v.value.empty() ? "" : " = ") << v.value << '\n';
//	}
//
//	std::cout << "Hi" << std::endl;
//	std::cout << libv::hex_dump("țûɾ ấɖḯƥĭṩčįɳ]ġ \"ᶁⱺ ẽḭŭŝḿꝋď ]ṫĕᶆᶈṓɍ ỉñḉīḑȋᵭṵńť ṷŧ ḹẩḇőꝛế éȶ đôȇ ᵯáᶇā ąⱡîɋṹẵ.") << std::endl;
//	std::cout << libv::is_big_endian() << std::endl;
//	std::cout << libv::is_little_endian() << std::endl;
//	std::cout << libv::is_network_endian() << std::endl;


// =================================================================================================
//
//
//// ext
//#include <boost/algorithm/string/replace.hpp>
//#include <boost/container/flat_map.hpp>
//#include <boost/core/demangle.hpp>
//#include <boost/stacktrace.hpp>
//#include <fmt/printf.h>
//// libv
//#include <libv/algorithm/slice.hpp>
//// std
//#include <iostream>
//#include <libv/utility/endian.hpp>
//#include <map>
//#include <regex>
//#include <string_view>
//
//
//#ifndef WISH_SHORT_PATH_PREFIX
//#    define WISH_SHORT_PATH_PREFIX ""
//#endif
//
//void prettify(std::string& name) {
//	std::vector<std::pair<std::string_view, std::string_view>> replace_map{
//		{", ", ","},
//		{" >", ">"},
//		{"> ", ">"},
//		{"__cxx11::", ""},
//		{"std::basic_string<char,std::char_traits<char>,std::allocator<char>>", "std::string"},
//		{"std::basic_string_view<char,std::char_traits<char>>", "std::string_view"},
//	};
//	//	std::vector<std::pair<std::regex, std::string>> pattern_map{
//	//		{std::regex{"<(.*),boost::container::new_allocator<.*>>"}, "<$1>"},
//	//		{std::regex{"<(.*),std::allocator<.*>>"}, "<$1>"},
//	//		{std::regex{"<(.*),std::less<.*>>"}, "<$1>"},
//	//	};
//
//	for (const auto& [from, to] : replace_map)
//		boost::algorithm::replace_all(name, from, to);
//
//	//	for (const auto& [pattern, to] : pattern_map)
//	//		name = std::regex_replace(name, pattern, to);
//
//	boost::algorithm::replace_all(name, ",", ", ");
//}
//
//void bar(std::vector<std::vector<std::string>>) {
//	int i = 0;
//	for (const boost::stacktrace::frame& frame : boost::stacktrace::stacktrace()) {
//		auto name = boost::core::demangle(("_" + frame.name()).c_str());
//		prettify(name);
//
//		fmt::print(" {}# {:40} {}:{}\n",
//				i++,
//				name,
//				libv::slice_prefix_view(frame.source_file(), WISH_SHORT_PATH_PREFIX),
//				frame.source_line());
//	}
//}
//
//void foo(double, boost::container::flat_map<std::string_view, int>) {
//	bar({});
//}
//
//int main() {
//	foo(0,{});
//
//	return 0;
//}
//
//
// =================================================================================================
//
//// ext
//#include <fmt/printf.h>
//// std
//#include <iostream>
//// pro
//#include <libv/math/vec.hpp>
//
//
//int main() {
//	float u0{0.0}; fmt::print("{:40.60}\n", u0);
//	float u1{-0.0}; fmt::print("{:40.60}\n", u1);
//	float u2{1.0}; fmt::print("{:40.60}\n", u2);
//	float u3{-1.0}; fmt::print("{:40.60}\n", u3);
//	float u4{0.11111111111111111111111111111111f}; fmt::print("{:40.60}\n", u4);
//
//	libv::vec3f v0{0.11111111111111111111111111111111f, 0, 0}; fmt::print("{}\n", v0);
//
//
//	return 0;
//}
//
// =================================================================================================
//
//// ext
//#include <fmt/printf.h>
//// std
//#include <filesystem>
//#include <iostream>
//#include <optional>
//
//
////std::optional<std::filesystem::path> resolve(
////		const std::filesystem::path& path,
////		const std::filesystem::path& base) {
////
////
////}
//
//std::optional<std::filesystem::path> resolve(const std::filesystem::path& path) {
//	const auto res_folder = std::filesystem::path("res/font");
//
//	std::error_code ec;
////	const auto normalized = path.lexically_normal();
//
//	fmt::print("is_relative: {}, is_absolute: {}\n", path.is_relative(), path.is_absolute());
//	const auto normalized = std::filesystem::canonical(res_folder / path, ec);
//
//	if (ec) {
//		fmt::print("cannonical: {} {}\n", ec, ec.message());
//		return std::nullopt;
//	}
//
//	fmt::print("is_relative: {}, is_absolute: {}\n", normalized.is_relative(), normalized.is_absolute());
//
////	if (normalized.empty())
////		return std::nullopt;
////
////	if (normalized.is_absolute())
////		return std::nullopt;
////
////	if (*normalized.begin() == "..")
////		return std::nullopt;
////
////	if (*normalized.begin() == "/")
////		return std::nullopt;
////
////	if (normalized.has_root_path())
////		return std::nullopt;
//
//	fmt::print("normalized: {}\n", normalized.generic_string());
////	return res_folder / normalized;
//
//	const auto relative = std::filesystem::relative(normalized, ec);
//
//	if (ec) {
//		fmt::print("relative:   {} {}\n", ec, ec.message());
//		return std::nullopt;
//	}
//	fmt::print("relative:   {}\n", relative.generic_string());
//
//	return relative;
//}
//
//void print(const std::filesystem::path& path) {
//	fmt::print("path:       {}\n", path.generic_string());
////	auto x = resolve(path, "res");
//	auto x = resolve(path);
//	if (x)
//		fmt::print("resolved:   {}\n\n", x->generic_string());
//	else
//		fmt::print("false\n\n");
//}
//
//int main() {
//	fmt::print("current_path: {}\n\n", std::filesystem::current_path().generic_string());
//	print("consola.ttf");
//	print("base/consola.ttf");
//	print("base/../consola.ttf");
//	print("../consola.ttf");
//	print("./consola.ttf");
//	print("../../res/font/consola.ttf");
//	print("E:\\res/font/consola.ttf");
//	print("E:\\res/../font/consola.ttf");
//	print("E:/res/font/consola.ttf");
//	print("E://res/../font/consola.ttf");
//	print("C:res/../font/consola.ttf");
//	print("//font/consola.ttf");
//	print("/c/font/consola.ttf");
//	print("c/font/consola.ttf");
//
//
//	std::error_code ec;
//	std::filesystem::path base = "base";
//	std::filesystem::path target = "target";
//	std::filesystem::path result = base / target;
//
//	result = std::filesystem::canonical(result, ec);
//	fmt::print("[{}]\n\n", result.generic_string());
//
//	return 0;
//}
//
//// =================================================================================================
//
//// libv
//#include <libv/utility/fixed_string.hpp>
//// std
//#include <iostream>
//
//
//template <libv::fixed_string Name>
//struct S {
//	static constexpr std::string_view name = Name;
//};
//
//template <typename T>
//void foo0(T s) {
//	std::cout << s.name << " " << s.name.size() << std::endl;
//}
//
////template <size_t N, const char Name[N]>
////void foo1(S<libv::fixed_string<N>{Name}> s) {
////	std::cout << s.name << " " << s.name.size() << std::endl;
////}
//
////template <libv::fixed_string Name>
////void foo1(S<Name> s) {
////	std::cout << s.name << " " << s.name.size() << std::endl;
////}
//
////template <auto Name>
////void foo1(S<Name> s) {
////	std::cout << s.name << " " << s.name.size() << std::endl;
////}
//
//int main() {
//	{
//		S<"test"> s;
//		std::cout << s.name << " " << s.name.size() << std::endl;
//		static_assert(std::is_same_v<decltype(s), S<libv::fixed_string<4>{"test"}>>);
//	}
//
//	{
//		libv::fixed_string s{"test"};
//		std::cout << s << " " << s.size() << std::endl;
//		static_assert(std::is_same_v<decltype(s), libv::fixed_string<4>>);
//	}
//
//	{
//		foo0(S<"test">{});
//	}
//
////	{
////		foo1(S<"test">{});
////	}
//
//	return 0;
//}

// =================================================================================================
//
//// libv
//#include <libv/math/bezier_curve.hpp>
//// std
//#include <iostream>
//
//
//int main() {
//	libv::math::BezierCurve curve;
//	curve.points.emplace_back(0, 0);
//		curve.points.emplace_back(0, 1);
//		curve.points.emplace_back(2, 1);
//	curve.points.emplace_back(2, 0);
//		curve.points.emplace_back(2, -1);
//		curve.points.emplace_back(1, -1);
//	curve.points.emplace_back(1, 0);
//		curve.points.emplace_back(0, 0.5f);
//		curve.points.emplace_back(1.5f, 0.5f);
//	curve.points.emplace_back(1.5f, 0);
//
//	for (int i = -15; i <= 45; ++i) {
//		const float t = static_cast<float>(i) * 0.1f;
//		std::cout << t << " = " << curve.eval(t) << std::endl;
//	}
//
//	std::cout << "min:  " << curve.min() << std::endl;
//	std::cout << "max:  " << curve.max() << std::endl;
//	std::cout << "size: " << curve.size() << std::endl;
//
//	return 0;
//}

// =================================================================================================

//// libv
//#include <libv/math/bezier_curve.hpp>
//// std
//#include <iostream>
//
//
//[[nodiscard]] constexpr inline bool interval_check(double value, double lo, double hi) noexcept {
//	return !((value > lo && value > hi) || (value < lo && value < hi));
//}
//
//[[nodiscard]] constexpr inline double interval_clamp(double value, double lo, double hi) noexcept {
//	if (hi < lo)
//		return std::clamp(value, hi, lo);
//	else
//		return std::clamp(value, lo, hi);
//}
//
//int main() {
//	for (int i = 0; i < 7; ++i) {
//		std::cout << interval_clamp(i, 2, 4) << " " << interval_clamp(i, 4, 2) << std::endl;
//	}
//	std::cout << std::endl;
//	for (int i = 0; i < 7; ++i) {
//		std::cout << interval_check(i, 2, 4) << " " << interval_check(i, 4, 2) << std::endl;
//	}
//
//	return 0;
//}

// =================================================================================================

//// libv
//#include <libv/math/constants.hpp>
//#include <libv/math/bezier_curve.hpp>
//#include <libv/math/vec.hpp>
//// std
//#include <cmath>
//#include <iostream>
//
//
////auto dclamp(int v, int hi, int lo) {
////    return std::min((v - lo), hi) + lo;
////    //return std::min((v - lo), hi - lo) + lo;
////	//return std::clamp(v, lo, hi);
////}
//
//
//[[nodiscard]] constexpr inline bool interval_check(double value, double lo, double hi) noexcept {
//	return !((value > lo && value > hi) || (value < lo && value < hi));
//}
//
//[[nodiscard]] constexpr inline double interval_clamp(double value, double lo, double hi) noexcept {
//	if (hi < lo)
//		return std::clamp(value, hi, lo);
//	else
//		return std::clamp(value, lo, hi);
//}
//
//int main() {
//	for (int i = 0; i < 7; ++i) {
//		std::cout << interval_clamp(i, 2, 4) << " " << interval_clamp(i, 4, 2) << std::endl;
//	}
//	std::cout << std::endl;
//	for (int i = 0; i < 7; ++i) {
//		std::cout << interval_check(i, 2, 4) << " " << interval_check(i, 4, 2) << std::endl;
//	}
//
//	return 0;
//}
//
// =================================================================================================

//// libv
//#include <libv/math/constants.hpp>
//#include <libv/math/vec.hpp>
//// std
//#include <cmath>
//#include <iostream>
//#include <cassert>
//#include <cstdlib>
//
//
//inline double get_dx(const double x[]) {
//	return x[1] - x[0];
//}
//
//int find_xb(double x, const double a[], int n, int b) {
//	assert(b < n);
//	double dx = get_dx(a);
//	int ix = (x - a[0]) / dx;
//	ix = std::max(ix, b);
//	ix = std::min(ix, n - b - 1);
//	return ix;
//}
//
//double lagrange_basis(int j, double x,
//		const double x_values[],
//		int num_x) {
//	double l_j = 1;
//	double r;
//	assert(0 <= j && j < num_x && "j is in bounds");
//	if (num_x == 0 || num_x == 1) return 1.;
//	for (int m = 0; m < num_x; m++) {
//		if (m != j) {
//			r = (x - x_values[m]) / (x_values[j] - x_values[m]);
//			l_j *= r;
//		}
//	}
//	return l_j;
//}
//
//double lagrange_interp_1D(double x,
//		const double x_values[],
//		int num_x,
//		const double y_values[]) {
//	double out = 0;
//	for (int j = 0; j < num_x; j++) {
//		out += y_values[j] * lagrange_basis(j, x, x_values, num_x);
//	}
//	return out;
//}
//
//double lagrange_interp_1Dfo(double x, int order,
//		const double x_values[], int num_x,
//		const double y_values[]) {
//	assert(order % 2 == 0);
//	assert(num_x > order);
//	double mx[order + 1];
//	double my[order + 1];
//	int i;
//	int ix = find_xb(x, x_values, num_x, order / 2);
//	for (i = 0; i < order + 1; i++) {
//		mx[i] = x_values[ix + i - order / 2];
//		my[i] = y_values[ix + i - order / 2];
//	}
//	return lagrange_interp_1D(x, mx, order + 1, my);
//}
//
//int main() {
//	double xs[] = {0, 1, 2};
//	double ys[] = {0, 0, 1};
//
//	for (double t = 0; t < 2; t += 0.2) {
//		std::cout << lagrange_interp_1Dfo(t, 2, xs, 3, ys) << std::endl;
//	}
//
//	return 0;
//}

// =================================================================================================

//// ext
//#include <fmt/format.h>
//// libv
//#include <libv/math/pid.hpp>
//
//
//int main() {
//	libv::PID<double> pid(0.25, 0.01, 0.5);
//
//	double dt = 0.1;
//	double current = 200;
//
//	for (int i = 0; i <= 100; i++) {
//		double target = static_cast<double>(i);
//		double feedback = pid.calculate(dt, current, target);
//
//		fmt::print("current: {:8.4f}, target: {:8.4f}, feedback: {:8.4f}\n", current, target, feedback);
//		current += feedback;
//	}
//
//	return 0;
//}


//// ext
//#include <fmt/format.h>
//// libv
//#include <libv/math/pid.hpp>
#include <iostream>
#include <cmath>
#include <filesystem>


void _append_slash(std::filesystem::path& dir) {
	static constexpr decltype(std::filesystem::path::preferred_separator) sep[] = {std::filesystem::path::preferred_separator, 0};

	if (dir.native().back() != std::filesystem::path::preferred_separator)
		dir.concat(sep);
}

std::filesystem::path _parent_directory(const std::filesystem::path& dir) {
	auto result = dir;

//	if (result.native().back() == std::filesystem::path::preferred_separator)
//		result = result.parent_path();
//
//	result = result.parent_path();
	_append_slash(result);
	return result;
}

int main() {
	std::cout << _parent_directory("E:\\dev\\cpp\\libv\\tmp") << std::endl;
	std::cout << _parent_directory("E:\\dev\\cpp\\libv\\tmp\\") << std::endl;
	return 0;
}
