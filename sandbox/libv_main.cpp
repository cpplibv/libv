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
//
//
////// ext
////#include <fmt/format.h>
////// libv
////#include <libv/math/pid.hpp>
//#include <iostream>
//#include <cmath>
//#include <filesystem>
//
//
//void _append_slash(std::filesystem::path& dir) {
//	static constexpr decltype(std::filesystem::path::preferred_separator) sep[] = {std::filesystem::path::preferred_separator, 0};
//
//	if (dir.native().back() != std::filesystem::path::preferred_separator)
//		dir.concat(sep);
//}
//
//std::filesystem::path _parent_directory(const std::filesystem::path& dir) {
//	auto result = dir;
//
////	if (result.native().back() == std::filesystem::path::preferred_separator)
////		result = result.parent_path();
////
////	result = result.parent_path();
//	_append_slash(result);
//	return result;
//}
//
//int main() {
//	std::cout << _parent_directory("E:\\dev\\cpp\\libv\\tmp") << std::endl;
//	std::cout << _parent_directory("E:\\dev\\cpp\\libv\\tmp\\") << std::endl;
//	return 0;
//}

// =================================================================================================

//// ext
//#include <boost/spirit/home/x3.hpp>
//// libv
//#include <libv/utility/utf8.hpp>
//// std
//#include <iomanip>
//#include <iostream>
//#include <optional>
//// pro
//#include <libv/ctrl/sequence.hpp>
//
//
//using namespace libv::ctrl;
//
//template <typename T>
//struct Scream;
//
//template <typename T>
//void scream(T&& t) {
//	Scream<T> x;
//}
//
//void foo(std::string_view input) {
//	namespace x3 = boost::spirit::x3;
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto fp_codepoint = [](auto& ctx) {
//		auto it = _attr(ctx).begin();
//		const auto end = _attr(ctx).end();
//		uint32_t unicode = 0;
//
//		auto valid = libv::next_unicode(it, end, unicode);
//		valid = valid && it == end;
//
//		_pass(ctx) = valid;
//		_val(ctx) = InputID(Codepoint{unicode});
//	};
//
//	const auto fp_scancode = [](auto& ctx) {
//		_val(ctx) = InputID(Scancode{_attr(ctx)});
//	};
//
//	const auto fp_codepoint_esc = [](auto& ctx) {
//		_val(ctx) = InputID(_attr(ctx));
//	};
//
//	const auto fp_keycode = [](auto& ctx) {
//		_val(ctx) = InputID(_attr(ctx));
//	};
//
//	const auto fp_modifier = [](auto& ctx) {
//		_val(ctx) = InputID(Modifier{_attr(ctx)});
//	};
//
//	const auto fp_mouse_button = [](auto& ctx) {
//		_val(ctx) = InputID(MouseButton{_attr(ctx)});
//	};
//
//	const auto fp_mouse_movement = [](auto& ctx) {
//		_val(ctx) = InputID(MouseMovement{_attr(ctx)});
//	};
//
//	// -------------------------------------------------------------------------------------------------
//
//	struct key_named_ : x3::symbols<Keycode> { key_named_() { add
////		("unknown",      Keycode::Unknown)
//		("space",        Keycode::Space)
//		("apostrophe",   Keycode::Apostrophe)
//		("'",            Keycode::Apostrophe)
//		("comma",        Keycode::Comma)
//		(",",            Keycode::Comma)
//		("minus",        Keycode::Minus)
//		("-",            Keycode::Minus)
//		// <editor-fold defaultstate="collapsed" desc="...">
//		("period",       Keycode::Period)
//		(".",            Keycode::Period)
//		("slash",        Keycode::Slash)
//		("/",            Keycode::Slash)
//		("semicolon",    Keycode::Semicolon)
//		(";",            Keycode::Semicolon)
//		("equals",       Keycode::Equals)
//		("=",            Keycode::Equals)
//		("a",            Keycode::A)
//		("b",            Keycode::B)
//		("c",            Keycode::C)
//		("d",            Keycode::D)
//		("e",            Keycode::E)
//		("f",            Keycode::F)
//		("g",            Keycode::G)
//		("h",            Keycode::H)
//		("i",            Keycode::I)
//		("j",            Keycode::J)
//		("k",            Keycode::K)
//		("l",            Keycode::L)
//		("m",            Keycode::M)
//		("n",            Keycode::N)
//		("o",            Keycode::O)
//		("p",            Keycode::P)
//		("q",            Keycode::Q)
//		("r",            Keycode::R)
//		("s",            Keycode::S)
//		("t",            Keycode::T)
//		("u",            Keycode::U)
//		("v",            Keycode::V)
//		("w",            Keycode::W)
//		("x",            Keycode::X)
//		("y",            Keycode::Y)
//		("z",            Keycode::Z)
//		("[",            Keycode::BracketOpen)
//		("backslash",    Keycode::Backslash)
//		("\\",           Keycode::Backslash)
//		("]",            Keycode::BracketClose)
//		("backtick",     Keycode::Backtick)
//		("`",            Keycode::Backtick)
//		("world1",       Keycode::World1)
//		("world2",       Keycode::World2)
//		("escape",       Keycode::Escape)
//		("esc",          Keycode::Escape)
//		("enter",        Keycode::Enter)
//		("tab",          Keycode::Tab)
//		("backspace",    Keycode::Backspace)
//		("insert",       Keycode::Insert)
//		("delete",       Keycode::Delete)
//		("right",        Keycode::Right)
//		("left",         Keycode::Left)
//		("down",         Keycode::Down)
//		("up",           Keycode::Up)
//		("home",         Keycode::Home)
//		("end",          Keycode::End)
//		("pause",        Keycode::Pause)
//		("f1",           Keycode::F1)
//		("f2",           Keycode::F2)
//		("f3",           Keycode::F3)
//		("f4",           Keycode::F4)
//		("f5",           Keycode::F5)
//		("f6",           Keycode::F6)
//		("f7",           Keycode::F7)
//		("f8",           Keycode::F8)
//		("f9",           Keycode::F9)
//		("f10",          Keycode::F10)
//		("f11",          Keycode::F11)
//		("f12",          Keycode::F12)
//		("f13",          Keycode::F13)
//		("f14",          Keycode::F14)
//		("f15",          Keycode::F15)
//		("f16",          Keycode::F16)
//		("f17",          Keycode::F17)
//		("f18",          Keycode::F18)
//		("f19",          Keycode::F19)
//		("f20",          Keycode::F20)
//		("f21",          Keycode::F21)
//		("f22",          Keycode::F22)
//		("f23",          Keycode::F23)
//		("f24",          Keycode::F24)
//		("f25",          Keycode::F25)
//		("menu",         Keycode::Menu)
//		// </editor-fold>
//
//		// extra aliases:
//		("dash",         Keycode::Minus)
//		("hyphen",       Keycode::Minus)
//	; }	} key_named;
//
//	const auto key_kp_base = x3::rule<class key_kp_base, Keycode>{} =
//			(-x3::lit("num") >> x3::lit("0") >> x3::attr(Keycode::KPNum0)) |
//			(-x3::lit("num") >> x3::lit("1") >> x3::attr(Keycode::KPNum1)) |
//			(-x3::lit("num") >> x3::lit("2") >> x3::attr(Keycode::KPNum2)) |
//			(-x3::lit("num") >> x3::lit("3") >> x3::attr(Keycode::KPNum3)) |
//			(-x3::lit("num") >> x3::lit("4") >> x3::attr(Keycode::KPNum4)) |
//			(-x3::lit("num") >> x3::lit("5") >> x3::attr(Keycode::KPNum5)) |
//			(-x3::lit("num") >> x3::lit("6") >> x3::attr(Keycode::KPNum6)) |
//			(-x3::lit("num") >> x3::lit("7") >> x3::attr(Keycode::KPNum7)) |
//			(-x3::lit("num") >> x3::lit("8") >> x3::attr(Keycode::KPNum8)) |
//			(-x3::lit("num") >> x3::lit("9") >> x3::attr(Keycode::KPNum9)) |
//			(x3::lit(".") | x3::lit("dot")) >> x3::attr(Keycode::KPDot) |
//			(x3::lit("/") | x3::lit("slash")) >> x3::attr(Keycode::KPSlash) |
//			(x3::lit("*") | x3::lit("asterisk")) >> x3::attr(Keycode::KPAsterisk) |
//			(x3::lit("-") | x3::lit("minus")) >> x3::attr(Keycode::KPMinus) |
//			(x3::lit("+") | x3::lit("plus")) >> x3::attr(Keycode::KPPlus) |
//			(x3::lit("enter") >> x3::attr(Keycode::KPEnter)) |
//			(x3::lit("=") | x3::lit("equal")) >> x3::attr(Keycode::KPEqual);
//
//	const auto key_num = x3::rule<class key_num, Keycode>{} =
//			(-x3::lit("num") >> x3::lit("0") >> x3::attr(Keycode::Num0)) |
//			(-x3::lit("num") >> x3::lit("1") >> x3::attr(Keycode::Num1)) |
//			(-x3::lit("num") >> x3::lit("2") >> x3::attr(Keycode::Num2)) |
//			(-x3::lit("num") >> x3::lit("3") >> x3::attr(Keycode::Num3)) |
//			(-x3::lit("num") >> x3::lit("4") >> x3::attr(Keycode::Num4)) |
//			(-x3::lit("num") >> x3::lit("5") >> x3::attr(Keycode::Num5)) |
//			(-x3::lit("num") >> x3::lit("6") >> x3::attr(Keycode::Num6)) |
//			(-x3::lit("num") >> x3::lit("7") >> x3::attr(Keycode::Num7)) |
//			(-x3::lit("num") >> x3::lit("8") >> x3::attr(Keycode::Num8)) |
//			(-x3::lit("num") >> x3::lit("9") >> x3::attr(Keycode::Num9));
//
//	const auto key_kp = x3::rule<class key_kp, Keycode>{} =
//			// Note: lit "." cannot be optional as it would consume the "." token from KPDot
//			x3::lit("kp") >> (x3::lit(".") >> key_kp_base | key_kp_base);
//
//	const auto key_pg = x3::rule<class key_pg, Keycode>{} =
//			(
//				x3::lit("pg") >> -x3::lit(".") |
//				x3::lit("page")
//			) >> (
//				(x3::lit("up") >> x3::attr(Keycode::PageUp)) |
//				(x3::lit("down") >> x3::attr(Keycode::PageDown))
//			);
//
//	const auto key_sided_left = x3::rule<class key_sided_left, Keycode>{} =
//			(x3::lit("alt") >> x3::attr(Keycode::AltLeft)) |
//			(x3::lit("control") >> x3::attr(Keycode::ControlLeft)) |
//			(x3::lit("ctrl") >> x3::attr(Keycode::ControlLeft)) |
//			(x3::lit("shft") >> x3::attr(Keycode::ShiftLeft)) |
//			(x3::lit("shift") >> x3::attr(Keycode::ShiftLeft)) |
//			(x3::lit("super") >> x3::attr(Keycode::SuperLeft));
//
//	const auto key_sided_right = x3::rule<class key_sided_right, Keycode>{} =
//			(x3::lit("alt") >> x3::attr(Keycode::AltRight)) |
//			(x3::lit("control") >> x3::attr(Keycode::ControlRight)) |
//			(x3::lit("ctrl") >> x3::attr(Keycode::ControlRight)) |
//			(x3::lit("shft") >> x3::attr(Keycode::ShiftRight)) |
//			(x3::lit("shift") >> x3::attr(Keycode::ShiftRight)) |
//			(x3::lit("super") >> x3::attr(Keycode::SuperRight));
//
//	const auto key_sided = x3::rule<class key_sided, Keycode>{} =
//			(x3::lit("left") | x3::lit("l") >> -x3::lit(".")) >> key_sided_left |
//			(x3::lit("right") | x3::lit("r") >> -x3::lit(".")) >> key_sided_right |
//			key_sided_left >> (x3::lit("left") | x3::lit("l") >> -x3::lit(".")) |
//			key_sided_right >> (x3::lit("right") | x3::lit("r") >> -x3::lit("."));
//
//	const auto key_multi_word = x3::rule<class key_multi_word, Keycode>{} =
//			x3::lit("bracket") >> x3::lit("open") >> x3::attr(Keycode::BracketOpen) |
//			x3::lit("bracket") >> x3::lit("close") >> x3::attr(Keycode::BracketClose) |
//			x3::lit("caps") >>   (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Keycode::CapsLock) |
//			x3::lit("scroll") >> (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Keycode::ScrollLock) |
//			x3::lit("num") >>    (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Keycode::NumLock) |
//			(x3::lit("print") | x3::lit("prt") >> -x3::lit(".")) >> (x3::lit("screen") | (x3::lit("scn") | x3::lit("sc")) >> -x3::lit(".")) >> x3::attr(Keycode::PrintScreen) |
//			x3::lit("alt") >> x3::lit("gr") >> -x3::lit(".") >> x3::attr(Keycode::AltRight);
//
//	// -------------------------------------------------------------------------------------------------
//
//	struct modifier_named_ : x3::symbols<Modifier> { modifier_named_() { add
//		("alt",       Modifier::alt)
//		("control",   Modifier::control)
//		("ctrl",      Modifier::control)
//		("shift",     Modifier::shift)
//		("shft",      Modifier::shift)
//		("super",     Modifier::super)
//		("sper",      Modifier::super)
//	; }	} modifier_named;
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto mouse_word_button = x3::rule<class key_sided_left, void>{} =
//			x3::lit("button") | (x3::lit("btn") | x3::lit("bt") | x3::lit("b")) >> -x3::lit(".");
//
//	const auto mouse_word_mouse = x3::rule<class key_sided_left, void>{} =
//			x3::lit("mouse") | x3::lit("m") >> -x3::lit(".");
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto joystick_button_rule = x3::rule<class joystick_button_rule, InputID>{} =
//			x3::lit("sc") >> x3::int32[fp_scancode];
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto joystick_analog_rule = x3::rule<class joystick_analog_rule, InputID>{} =
//			x3::lit("sc") >> x3::int32[fp_scancode];
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto mouse_move_rule = x3::rule<class mouse_move_rule, InputID>{} =
//			(x3::lit("horizontal") | (x3::lit("hori") | x3::lit("h")) >> -x3::lit(".") | x3::lit("x")) >> mouse_word_mouse >> x3::attr(MouseMovement::x)[fp_mouse_movement] |
//			mouse_word_mouse >> (x3::lit("horizontal") | (x3::lit("hori") | x3::lit("h")) >> -x3::lit(".") | x3::lit("x")) >> x3::attr(MouseMovement::x)[fp_mouse_movement] |
//			(x3::lit("vertical") | (x3::lit("vert") | x3::lit("v")) >> -x3::lit(".") | x3::lit("y")) >> mouse_word_mouse >> x3::attr(MouseMovement::y)[fp_mouse_movement] |
//			mouse_word_mouse >> (x3::lit("vertical") | (x3::lit("vert") | x3::lit("v")) >> -x3::lit(".") | x3::lit("y")) >> x3::attr(MouseMovement::y)[fp_mouse_movement];
//
//	const auto mouse_button_rule = x3::rule<class mouse_button_rule, InputID>{} =
//			(x3::lit("left") | x3::lit("l") >> -x3::lit(".")) >> mouse_word_mouse >> -mouse_word_button >> x3::attr(MouseButton::Left)[fp_mouse_button] |
//			mouse_word_mouse >> -mouse_word_button >> (x3::lit("left") | x3::lit("l") >> -x3::lit(".")) >> x3::attr(MouseButton::Left)[fp_mouse_button] |
//			(x3::lit("right") | x3::lit("r") >> -x3::lit(".")) >> mouse_word_mouse >> -mouse_word_button >> x3::attr(MouseButton::Right)[fp_mouse_button] |
//			mouse_word_mouse >> -mouse_word_button >> (x3::lit("right") | x3::lit("r") >> -x3::lit(".")) >> x3::attr(MouseButton::Right)[fp_mouse_button] |
//			(x3::lit("middle") | x3::lit("m") >> -x3::lit(".")) >> mouse_word_mouse >> -mouse_word_button >> x3::attr(MouseButton::Middle)[fp_mouse_button] |
//			mouse_word_mouse >> -mouse_word_button >> (x3::lit("middle") | x3::lit("m") >> -x3::lit(".")) >> x3::attr(MouseButton::Middle)[fp_mouse_button] |
//			x3::int32[fp_mouse_button] >> mouse_word_mouse >> -mouse_word_button |
//			mouse_word_mouse >> -mouse_word_button >> x3::int32[fp_mouse_button];
//
//	const auto codepoint_rule = x3::rule<class codepoint_rule, InputID>{} =
//			x3::lit("\"\\\"\"") >> x3::attr(Codepoint{'"'})[fp_codepoint_esc] |
//			x3::lit("\"\\\'\"") >> x3::attr(Codepoint{'\''})[fp_codepoint_esc] |
//			x3::lit("\'\\\"\'") >> x3::attr(Codepoint{'"'})[fp_codepoint_esc] |
//			x3::lit("\'\\\'\'") >> x3::attr(Codepoint{'\''})[fp_codepoint_esc] |
//			x3::lexeme[x3::lit("\"") >> (*~x3::char_('\"'))[fp_codepoint] >> x3::lit("\"")] |
//			x3::lexeme[x3::lit("'") >> (*~x3::char_('\''))[fp_codepoint] >> x3::lit("'")];
//
//	const auto scancode_rule = x3::rule<class scancode_rule, InputID>{} =
//			(x3::lit("scancode") | x3::lit("scan") | x3::lit("sc")) >> x3::int32[fp_scancode];
//
//	const auto keycode_or_modifier_rule = x3::rule<class keycode_or_modifier_rule, InputID>{} =
//			key_num[fp_keycode] |
//			key_kp[fp_keycode] |
//			key_pg[fp_keycode] |
//			key_multi_word[fp_keycode] |
//			key_sided[fp_keycode] |
//			modifier_named[fp_modifier] | // NOTE: modifier_named has to be after key_sided and before key_named
//			key_named[fp_keycode];
//
//	// -------------------------------------------------------------------------------------------------
//
//	const auto input_id_rule = x3::rule<class input_id_rule, InputID>{} =
//			x3::no_case[
//				mouse_move_rule |
//				mouse_button_rule |
//				scancode_rule |
//				codepoint_rule |
//				keycode_or_modifier_rule
//			];
//
//	// -------------------------------------------------------------------------------------------------
//
//	InputID result;
//
//	auto it = input.begin();
//	auto success = x3::phrase_parse(it, input.end(), input_id_rule, x3::space, result);
//	auto full_success = success && it == input.end();
//
//	if (full_success)
//		std::cout << "   OK   " << std::setw(12) << input << " = " << result << " " << result.rep() << std::endl;
//	else if (success)
//		std::cout << "Consume " << std::setw(12) << input << " = " << std::string_view(input.begin(), std::distance(input.begin(), it)) << std::endl;
//	else
//		std::cout << "Failure " << std::setw(12) << input << std::endl;
//}

//#include <iostream>
//#include <libv/color/spread.hpp>
//#include <libv/math/vec.hpp>
//
//int main() {
//	for (size_t i = 0; i < 16; i++) {
//		std::cout << i << " - " << libv::color::incremental_spread_to_01(i) << std::endl;
//	}
//	std::cout << "----------------------------------" << std::endl;
//	for (size_t i = 0; i < 200; i++) {
//		std::cout << i << " - " << libv::color::incremental_spread<libv::vec3f>(i) << std::endl;
//	}
//
//	return 0;
//}


#include <iostream>
#include <type_traits>
#include <libv/utility/extract_bits.hpp>


int main() {
	std::cout << std::hex;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 0, 8>(0x0123456789ABCDEFull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 1, 8>(0x0123456789ABCDEFull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 8, 8>(0x0123456789ABCDEFull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 0, 8>(0xFEDCBA9876543210ull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 1, 8>(0xFEDCBA9876543210ull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 8, 8>(0xFEDCBA9876543210ull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 56, 8>(0x0123456789ABCDEFull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 56, 8>(0xFEDCBA9876543210ull)) << std::endl;
	std::cout << static_cast<uint64_t>(libv::extract_high_bits<uint8_t, 16, 1>(0xFEDCBA9876543210ull)) << std::endl;

	return 0;
}
