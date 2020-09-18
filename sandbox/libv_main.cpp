// Project: libv, File: sandbox/libv_main.cpp, Author: Császár Mátyás [Vader]

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
//
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
//
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
// =================================================================================================
//
//// ext
//#include <fmt/printf.h>
//// std
//#include <filesystem>
//#include <iostream>
//#include <optional>
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
// =================================================================================================

#include <iostream>
#include <filesystem>
#include <chrono>
#include <fmt/printf.h>
#include <fmt/chrono.h>

// -------------------------------------------------------------------------------------------------

int main(int, const char** argv) {
	const auto path = std::filesystem::path(argv[0]);

	const auto lwt = std::filesystem::last_write_time(path);
	const auto sys_lwt = decltype(lwt)::clock::to_sys(lwt);
	fmt::print("{}\n", path.generic_string());
	fmt::print("Last modified {:%Y.%m.%d %H:%M:%S}\n", fmt::localtime(std::chrono::system_clock::to_time_t(sys_lwt)));

	return EXIT_SUCCESS;
}
