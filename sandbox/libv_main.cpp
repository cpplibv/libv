// Project: libv, File: sandbox/libv_main.cpp

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
//#include <libv/algo/slice.hpp>
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

//#include <iostream>
//#include <filesystem>
//#include <chrono>
//#include <fmt/printf.h>
//#include <fmt/chrono.h>
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, const char** argv) {
//	const auto path = std::filesystem::path(argv[0]);
//
//	const auto lwt = std::filesystem::last_write_time(path);
//	const auto sys_lwt = decltype(lwt)::clock::to_sys(lwt);
//	fmt::print("{}\n", path.generic_string());
//	fmt::print("Last modified {:%Y.%m.%d %H:%M:%S}\n", fmt::localtime(std::chrono::system_clock::to_time_t(sys_lwt)));
//
//	return EXIT_SUCCESS;
//}

//// =================================================================================================
//
//#include <iostream>
//#include <fmt/printf.h>
//#include <libv/mt/work_cooldown.hpp>
//#include <libv/mt/worker_thread.hpp>
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, const char**) {
//
//	libv::mt::worker_thread worker_thread{"lua-engine-worker"};
//	libv::mt::work_cooldown load_cd{std::chrono::milliseconds{100}};
//
//	const auto start = std::chrono::steady_clock::now();
//
//	for (int i = 0; i < 100; ++i) {
//		std::this_thread::sleep_until(start + std::chrono::milliseconds{10 * i});
//
//		{
//			const auto now = std::chrono::steady_clock::now();
//			fmt::print("   i: {:>3}, t: {:>3} ms\n", i, std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
//		}
//
//		worker_thread.execute_async([i, start] {
//			const auto now = std::chrono::steady_clock::now();
//			fmt::print("AS i: {:>3}, t: {:>3} ms\n", i, std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
//		});
//
//		load_cd.execute_async([i, start] {
//			const auto now = std::chrono::steady_clock::now();
//			fmt::print("CD i: {:>3}, t: {:>3} ms\n", i, std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
//		}, worker_thread);
//	}
//
//	return EXIT_SUCCESS;
//}
//
//// =================================================================================================
//
//#include <iostream>
//#include <libv/utility/hex_dump.hpp>
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, const char**) {
//	for (int i = 0; i <= 64; ++i) {
//		std::string str;
//		for (int j = 0; j < i; ++j) {
//			str += (j <= 'z' - 'a') ? 'a' + j : 'A' + (j - ('z' - 'a'));
//		}
//
//		std::cout << libv::hex_dump_with_ascii(str) << std::endl;
//	}
//
//	return EXIT_SUCCESS;
//}
//
//// =================================================================================================
//
//#include <iostream>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <random>
//#include <chrono>
//
//// -------------------------------------------------------------------------------------------------
//
//#define LIFT(FUNC) [](auto... a) { return FUNC(a...); }
//
//int main(int, const char**) {
//	static constexpr auto seed = 0x5EED;
//	std::mt19937_64 rng;
//
//	const auto make = [&rng](auto f, auto... a) {
//		std::cout << " --- " << std::endl;
//		rng.seed(seed);
//
//		auto dist = f(a...);
//
//		for (int i = 0; i < 10; ++i)
//			if constexpr ((libv::is_std_chrono_duration_v<decltype(a)> && ...))
//				std::cout << dist(rng).count() << std::endl;
//			else
//				std::cout << dist(rng) << std::endl;
//	};
//
//	using isec = std::chrono::seconds;
//	using fsec = std::chrono::duration<float, std::chrono::seconds::period>;
//
//	make(LIFT(libv::make_uniform_distribution_inclusive), 10, 20);
//	make(LIFT(libv::make_uniform_distribution_inclusive), 10, 20.0);
//	make(LIFT(libv::make_uniform_distribution_inclusive), isec{10}, isec{20});
//	make(LIFT(libv::make_uniform_distribution_inclusive), fsec{10}, fsec{20.0});
//
//	make(LIFT(libv::make_uniform_distribution_inclusive), 20);
//	make(LIFT(libv::make_uniform_distribution_inclusive), 20.0);
//	make(LIFT(libv::make_uniform_distribution_inclusive), isec{20});
//	make(LIFT(libv::make_uniform_distribution_inclusive), fsec{20.0});
//
//	make(LIFT(libv::make_uniform_distribution_exclusive), 10, 20);
//	make(LIFT(libv::make_uniform_distribution_exclusive), 10, 20.0);
//	make(LIFT(libv::make_uniform_distribution_exclusive), isec{10}, isec{20});
//	make(LIFT(libv::make_uniform_distribution_exclusive), fsec{10}, fsec{20.0});
//
//	make(LIFT(libv::make_uniform_distribution_exclusive), 20);
//	make(LIFT(libv::make_uniform_distribution_exclusive), 20.0);
//	make(LIFT(libv::make_uniform_distribution_exclusive), isec{20});
//	make(LIFT(libv::make_uniform_distribution_exclusive), fsec{20.0});
//
//
////	rng.seed(seed);
////
////	auto dist_in = libv::make_uniform_distribution_inclusive(10, 20);
////	auto dist_ex = libv::make_uniform_distribution_exclusive(10, 20);
////
////	for (int i = 0; i < 100; ++i)
////		std::cout << dist_in(rng) << " "  << dist_ex(rng) << std::endl;
//
//
//	return EXIT_SUCCESS;
//}
//
//// =================================================================================================
//
//#include <libv/process/lock_file.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <chrono>
//#include <iostream>
//#include <mutex>
//#include <thread>
//#include <random>
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int argc, const char** argv) {
//	if (argc < 2)
//		return EXIT_FAILURE;
//
//	std::random_device rng;
//	std::this_thread::sleep_for(libv::make_uniform_distribution_inclusive(std::chrono::milliseconds(1000))(rng));
//
//	libv::process::lock_file mutex(".lock");
//	std::unique_lock lock(mutex, std::defer_lock);
//
//	for (int i = 0; i < 1000; i++) {
//		if (lock.try_lock()) {
//			break;
//		}
//		std::cout << argv[1] << " Waiting" << std::endl;
//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
//	}
//
//	for (int i = 0; i < 10; i++) {
//		std::cout << argv[1] << " Working" << std::endl;
//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
//	}
//
//	return EXIT_SUCCESS;
//}
//
//// -------------------------------------------------------------------------------------------------

#include <bit>
#include <iostream>
#include <libv/utility/hash.hpp>
#include <libv/math/fixed_point.hpp>
#include <libv/utility/histogram.hpp>

// -------------------------------------------------------------------------------------------------

template <std::size_t N>
struct Spectrum {
	std::array<std::size_t, N> data;

	float min = 0;
	float max = 3600;

	Spectrum() {
		data.fill(0);
	}

	void sample(float value) {
//		data[(value - min) / ((max - min) / N)]++;
		data[static_cast<std::size_t>((value - min) / ((max - min) / static_cast<float>(N)))]++;
	}

	friend std::ostream& operator<<(std::ostream& os, const Spectrum& var) {
		for (std::size_t i = 0; i < N; ++i)
			os << var.data[i] << ' ';
		return os;
	}
};

int main() {
	Spectrum<1000> spectrum;

	for (int32_t i = 0; i < 1000000; ++i) {
		const auto ui = std::bit_cast<uint32_t>(i + 1);
		const auto hash = libv::hash_int(ui);
		const auto f_high = ((hash & 0xFFFF0000u) >> 16u) % 3600u;
		const auto f_low = (hash & 0x0000FFFFu);

		const auto offset = static_cast<float>(f_high) + static_cast<float>(f_low) / static_cast<float>(0xFFFF);

//		const auto f = libv::convert_from_16_16<float>(hash);
//		const auto offset = std::fmod(f, 3600.0f);
//		std::cout << i << ") Hash: " << hash << ", float: " << f << ", offset: " << offset << std::endl;
//		std::cout << i << ") Hash: " << hash << ", offset: " << offset << std::endl;

		spectrum.sample(offset);
	}

	std::cout << spectrum << std::endl;
	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
