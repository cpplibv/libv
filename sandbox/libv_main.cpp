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

//#include <bit>
//#include <iostream>
//#include <libv/utility/hash.hpp>
//#include <libv/math/fixed_point.hpp>
//#include <libv/utility/histogram.hpp>
//
//// -------------------------------------------------------------------------------------------------
//
//template <std::size_t N>
//struct Spectrum {
//	std::array<std::size_t, N> data;
//
//	float min;
//	float max;
//
//	explicit Spectrum(float min = 0.f, float max = 0.f) : min(min), max(max) {
//		data.fill(0);
//	}
//
//	void sample(float value) {
////		data[(value - min) / ((max - min) / N)]++;
//		const auto index = static_cast<std::size_t>(std::clamp((value - min) / ((max - min) / static_cast<float>(N)), 0.f, static_cast<float>(N - 1)));
//		data[index]++;
//	}
//
//	void clear() {
//		data.fill(0);
//	}
//
//	friend std::ostream& operator<<(std::ostream& os, const Spectrum& var) {
//		for (std::size_t i = 0; i < N; ++i)
//			os << var.data[i] << ' ';
//		return os;
//	}
//};

//int main() {
//	Spectrum<1000> spectrum{0, 3600};
//
//	for (int32_t i = 0; i < 1000000; ++i) {
//		const auto ui = std::bit_cast<uint32_t>(i + 1);
//		const auto hash = libv::hash_int(ui);
//		const auto f_high = ((hash & 0xFFFF0000u) >> 16u) % 3600u;
//		const auto f_low = (hash & 0x0000FFFFu);
//
//		const auto offset = static_cast<float>(f_high) + static_cast<float>(f_low) / static_cast<float>(0xFFFF);
//
////		const auto f = libv::convert_from_16_16<float>(hash);
////		const auto offset = std::fmod(f, 3600.0f);
////		std::cout << i << ") Hash: " << hash << ", float: " << f << ", offset: " << offset << std::endl;
////		std::cout << i << ") Hash: " << hash << ", offset: " << offset << std::endl;
//
//		spectrum.sample(offset);
//	}
//
//	std::cout << spectrum << std::endl;
//	return EXIT_SUCCESS;
//}

// -------------------------------------------------------------------------------------------------

//#include <libv/utility/random/xoroshiro128.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <libv/utility/timer.hpp>
//#include <random>
//
//int main() {
//	// libv::xoroshiro128 benchmarking
//
//	Spectrum<1000> spectrumXO{0, 1};
//	Spectrum<1000> spectrumFork{0, 1};
//	Spectrum<1000> spectrumBad{0, 1};
//	Spectrum<1000> spectrumMT{0, 1};
//	Spectrum<1000> spectrumMT64{0, 1};
//
//	auto dist = libv::make_uniform_distribution_inclusive(0.f, 1.f);
//
//	for (int j = 0; j < 3; ++j) {
//		auto forkParent = libv::xoroshiro128(j);
//
//		libv::Timer timer;
//
//		for (int32_t i = 0; i < 5000000; ++i) {
//			auto rngXO = libv::xoroshiro128(i);
//			spectrumXO.sample(dist(rngXO));
//		}
//		const auto timeXO = timer.timef_ms();
//
//		for (int32_t i = 0; i < 5000000; ++i) {
//			auto rngFork = forkParent.fork();
//			spectrumFork.sample(dist(rngFork));
//		}
//		const auto timeFork = timer.timef_ms();
//
//		for (int32_t i = 0; i < 5000000; ++i) {
//			auto rngBad = libv::xoroshiro128::high_quality_seed(i);
//			spectrumBad.sample(dist(rngBad));
//		}
//		const auto timeBad = timer.timef_ms();
//
//		for (int32_t i = 0; i < 5000000; ++i) {
//			auto rngMT = std::mt19937(i);
//			spectrumMT.sample(dist(rngMT));
//		}
//		const auto timeMT = timer.timef_ms();
//
//		for (int32_t i = 0; i < 5000000; ++i) {
//			auto rngMT64 = std::mt19937_64(i);
//			spectrumMT64.sample(dist(rngMT64));
//		}
//		const auto timeMT64 = timer.timef_ms();
//
//		std::cout << "Construction timeXO  : " << timeXO.count() << std::endl;
//		std::cout << "Construction timeFork: " << timeFork.count() << std::endl;
//		std::cout << "Construction timeBad : " << timeBad.count() << std::endl;
//		std::cout << "Construction timeMT  : " << timeMT.count() << std::endl;
//		std::cout << "Construction timeMT64: " << timeMT64.count() << std::endl;
//		std::cout << std::endl;
//	}
//
//	std::cout << spectrumXO << std::endl;
//	std::cout << spectrumFork << std::endl;
//	std::cout << spectrumBad << std::endl;
//	std::cout << spectrumMT << std::endl;
//	std::cout << spectrumMT64 << std::endl;
//	spectrumXO.clear();
//	spectrumFork.clear();
//	spectrumBad.clear();
//	spectrumMT.clear();
//	spectrumMT64.clear();
//
//	for (int j = 0; j < 3; ++j) {
//		auto rngXO = libv::xoroshiro128(j);
//		auto rngMT = std::mt19937(j);
//		auto rngMT64 = std::mt19937_64(j);
//
//		libv::Timer timer;
//		for (int32_t i = 0; i < 500000000; ++i) {
//			spectrumXO.sample(dist(rngXO));
//		}
//		const auto timeXO = timer.timef_ms();
//
//		for (int32_t i = 0; i < 500000000; ++i) {
//			spectrumMT.sample(dist(rngMT));
//		}
//		const auto timeMT = timer.timef_ms();
//
//		for (int32_t i = 0; i < 500000000; ++i) {
//			spectrumMT64.sample(dist(rngMT64));
//		}
//		const auto timeMT64 = timer.timef_ms();
//
//		std::cout << "Generation timeXO  : " << timeXO.count() << std::endl;
//		std::cout << "Generation timeMT  : " << timeMT.count() << std::endl;
//		std::cout << "Generation timeMT64: " << timeMT64.count() << std::endl;
//		std::cout << std::endl;
//	}
//
//	std::cout << spectrumXO << std::endl;
//	std::cout << spectrumMT << std::endl;
//	std::cout << spectrumMT64 << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "sizeof(libv::xoroshiro128): " << sizeof(libv::xoroshiro128) << std::endl;
//	std::cout << "sizeof(std::mt19937)      : " << sizeof(std::mt19937) << std::endl;
//	std::cout << "sizeof(std::mt19937_64)   : " << sizeof(std::mt19937_64) << std::endl;
//
//	return EXIT_SUCCESS;
//}

// -------------------------------------------------------------------------------------------------

//#include <libv/utility/random/normal_distribution.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
//#include <libv/utility/timer.hpp>
//#include <random>
//
//
//
//int main() {
//	Spectrum<1000> spectrum_normal995_clamp{0, 3};
//	Spectrum<1000> spectrum_normal995_repeat{0, 3};
//	Spectrum<1000> spectrum_normal995d_clamp{0, 3};
//	Spectrum<1000> spectrum_normal995d_repeat{0, 3};
////	Spectrum<1000> spectrum_normal995i_clamp{0, 3000};
////	Spectrum<1000> spectrum_normal995i_repeat{0, 3000};
//
//	libv::xoroshiro128 rng;
//
//	libv::Timer timer;
//
//	for (int32_t i = 0; i < 10000000; ++i) {
//		spectrum_normal995_clamp.sample(libv::normal995_clamp(rng, 1.5f, 0.5f));
//	}
//	const auto time_normal995_clamp = timer.timef_ms();
//
//	for (int32_t i = 0; i < 10000000; ++i) {
//		spectrum_normal995_repeat.sample(libv::normal995_repeat(rng, 1.5f, 0.5f));
//	}
//	const auto time_normal995_repeat = timer.timef_ms();
//
//	for (int32_t i = 0; i < 10000000; ++i) {
//		spectrum_normal995d_clamp.sample(libv::normal995_clamp(rng, 1.5f, 0.5f));
//	}
//	const auto time_normal995d_clamp = timer.timef_ms();
//
//	for (int32_t i = 0; i < 10000000; ++i) {
//		spectrum_normal995d_repeat.sample(libv::normal995_repeat(rng, 1.5f, 0.5f));
//	}
//	const auto time_normal995d_repeat = timer.timef_ms();
//
////	for (int32_t i = 0; i < 10000000; ++i) {
////		spectrum_normal995i_clamp.sample(normal995_clamp(rng, 1000, 2000));
////	}
////	const auto time_normal995i_clamp = timer.timef_ms();
////
////	for (int32_t i = 0; i < 10000000; ++i) {
////		spectrum_normal995i_repeat.sample(normal995_repeat(rng, 1000, 2000));
////	}
////	const auto time_normal995i_repeat = timer.timef_ms();
//
//	std::cout << "Time spectrum_normal995_clamp : " << time_normal995_clamp.count() << std::endl;
//	std::cout << "Time spectrum_normal995_repeat : " << time_normal995_repeat.count() << std::endl;
//	std::cout << "Time spectrum_normal995d_clamp : " << time_normal995d_clamp.count() << std::endl;
//	std::cout << "Time spectrum_normal995d_repeat : " << time_normal995d_repeat.count() << std::endl;
////	std::cout << "Time spectrum_normal995i_clamp : " << time_normal995i_clamp.count() << std::endl;
////	std::cout << "Time spectrum_normal995i_repeat : " << time_normal995i_repeat.count() << std::endl;
//
//	std::cout << "spectrum_normal995_clamp " << spectrum_normal995_clamp << std::endl;
//	std::cout << "spectrum_normal995_repeat " << spectrum_normal995_repeat << std::endl;
//	std::cout << "spectrum_normal995d_clamp " << spectrum_normal995d_clamp << std::endl;
//	std::cout << "spectrum_normal995d_repeat " << spectrum_normal995d_repeat << std::endl;
////	std::cout << "spectrum_normal995i_clamp " << spectrum_normal995i_clamp << std::endl;
////	std::cout << "spectrum_normal995i_repeat " << spectrum_normal995i_repeat << std::endl;
//	return EXIT_SUCCESS;
//}

// =================================================================================================
// === Code binary size tracking ===

// // libv
// #include <libv/algo/slice.hpp>
// #include <libv/range/view_split_sv.hpp>
// #include <libv/utility/parse_number.hpp>
// #include <libv/utility/read_file.hpp>
// #include <libv/utility/write_file.hpp>
// // std
// #include <filesystem>
// #include <iostream>
// #include <map>
// #include <sstream>
// #include <vector>
//
//
// struct Entry {
// 	std::vector<uint64_t> values;
// };
//
// int main() {
//
// 	// Configs
//
// 	const auto prefix = "build/dev/CMakeFiles/space.dir/app/";
// 	const auto dir = std::filesystem::path(prefix);
// 	const auto stats_file = "build/dev/space.stats.csv";
//
// 	// Load file
//
// 	std::string stat_history;
//
// 	try {
// 		stat_history = libv::read_file_or_throw(stats_file);
// 	} catch (const std::system_error& ex) {
// 		if (ex.code() != std::make_error_code(std::errc::no_such_file_or_directory)) {
// 			std::cerr << "Failed to load statics history:\n" << ex.what() << std::endl;
// 			return EXIT_FAILURE;
// 		}
// 	}
//
// 	// Parse file
//
// 	std::map<std::string, Entry, std::less<>> statistics;
// 	uint64_t history_depth = 0;
//
// 	try {
// 		if (!stat_history.empty()) {
// 			for (const auto& line : stat_history | libv::view::split_sv('\n')) {
// 				if (line.empty())
// 					continue;
//
// 				uint64_t current_history_depth = 0;
// 				bool is_first_cell = true;
// 				Entry* currentEntry = nullptr;
//
// 				for (const auto& cell : line | libv::view::split_sv(',')) {
// 					if (is_first_cell) {
// 						currentEntry = &statistics.emplace(cell, Entry{}).first->second;
// 						is_first_cell = false;
// 						continue;
// 					}
//
// 					++current_history_depth;
// 					currentEntry->values.emplace_back(libv::parse_number_or_throw<uint64_t>(cell));
// 				}
//
// 				history_depth = std::max(history_depth, current_history_depth);
// 			}
// 		}
//
// 	} catch (const std::exception& ex) {
// 		std::cerr << "Failed to parse statics history:\n"
// 				<< ex.what() << "\n"
// 				<< "line: " << statistics.size()
// 				<< std::endl;
// 		return EXIT_FAILURE;
// 	}
//
// 	// Record current state
//
// 	for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
// 		if (!entry.is_regular_file())
// 			continue;
//
// 		const auto full_path = entry.path().generic_string();
// 		const auto path = libv::slice_prefix_view(libv::slice_suffix_view(full_path, ".obj"), prefix);
//
// 		const auto size = entry.file_size();
// 		std::cout << path << " " << static_cast<double>(size) / 1024.0 << " KB" << std::endl;
//
// 		auto stat_it = statistics.find(path);
// 		if (stat_it == statistics.end()) {
// 			stat_it = statistics.emplace(path, Entry{}).first;
// 			stat_it->second.values.resize(history_depth);
// 		}
//
// 		stat_it->second.values.emplace_back(size);
// 	}
//
// 	for (auto& [key, stats] : statistics)
// 		if (stats.values.size() != history_depth + 1)
// 			stats.values.emplace_back(0);
//
// 	// touch /E/dev/cpp/libv/app/space/network/network_client.cpp
// 	// time ninja -C build/dev /E/dev/cpp/libv/app/space/network/network_client.cpp^
//
// 	// Serialize statistics
//
// 	std::ostringstream new_stat_history;
// 	for (const auto& [key, stats] : statistics) {
// 		new_stat_history << key;
// 		for (const auto& value : stats.values)
// 			new_stat_history << ',' << value;
// 		new_stat_history << '\n';
// 	}
//
// 	// Save statistics
//
// 	try {
// 		libv::write_file_or_throw(stats_file, std::move(new_stat_history).str());
// 	} catch (const std::system_error& ex) {
// 		std::cerr << "Failed to save statics history:\n" << ex.what() << std::endl;
// 		return EXIT_FAILURE;
// 	}
// }

// =================================================================================================
// std::expected

//#include <iostream>
//#include <expected>
//
//
//std::expected<int, std::error_code> foo() {
//	return std::unexpected<std::error_code>{std::make_error_code(std::errc::invalid_argument)};
////	return {52};
//}
//
//int main() {
////	std::expected<int, bool> var;
////	var.emplace(45);
////	var.emplace(false);
//
////	std::expected<int, int> var;
////	var.emplace(45);
////	var.emplace(std::unexpected<int>(45));
////	var = std::unexpected<int>(45);
//
//	auto var = foo();
//
////	var = std::unexpect;
////	var.emplace(std::unexpect, false);
//
//	if (var)
//		std::cout << "Value: " << var.value() << std::endl;
//	else
//		std::cout << "Error: " << var.error() << std::endl;
//}


// =================================================================================================
// zstd

// #include <iostream>
// #include <string_view>
// #include <vector>
// // #include <expected>
// #include <zstd.h>
// #include <cassert>
// #include <span>
//
//
// std::vector<std::byte> compress(std::string_view data, int level) {
// 	assert(level >= 1 && level <= 22);
//
// 	std::vector<std::byte> result;
// 	result.resize(ZSTD_compressBound(data.size()));
//
// 	std::cout << "bound:       " << result.size() << std::endl;
//
// 	const auto cSize = ZSTD_compress(result.data(), result.size(), data.data(), data.size(), level);
// 	// CHECK_ZSTD(cSize);
// 	// auto code = ZSTD_isError(cSize);
// 	// if (code) {
// 	// 	return code;
// 	// 	ZSTD_getErrorName
// 	// }
// 	result.erase(result.begin() + cSize, result.end());
//
// 	return result;
// }
//
// std::string decompress(std::span<const std::byte> data) {
// 	std::string result;
//
// 	const auto rSize = ZSTD_getFrameContentSize(data.data(), data.size());
// 	// CHECK(rSize != ZSTD_CONTENTSIZE_ERROR, "%s: not compressed by zstd!", fname);
//  	// CHECK(rSize != ZSTD_CONTENTSIZE_UNKNOWN, "%s: original size unknown!", fname);
// 	// auto code = ZSTD_isError(rSize); ?
// 	// if (code) {
// 	// 	return code;
// 	// 	ZSTD_getErrorName
// 	// }
// 	result.resize(rSize);
//
// 	const auto dSize = ZSTD_decompress(result.data(), result.size(), data.data(), data.size());
// 	(void) dSize;
// 	// CHECK_ZSTD(dSize);
// 	// CHECK(dSize == rSize, "Impossible because zstd will check this condition!");
// 	// auto code = ZSTD_isError(dSize);
// 	// if (code) {
// 	// 	return code;
// 	// 	ZSTD_getErrorName
// 	// }
//
// 	return result;
// }
//
// int main() {
// 	const std::string_view data = "No plan of operations extends with certainty beyond the first encounter with the enemy's main strength";
//
// 	std::cout << "ZSTD" << std::endl;
// 	std::cout << "size:        " << data.size() << std::endl;
//
// 	{
// 		const auto compressedData = compress(data, 1);
// 		std::cout << "com size  1: " << compressedData.size() << std::endl;
//
// 		std::cout << "com data  1: ";
// 		for (const auto& byte : compressedData)
// 			std::cout << (std::isprint(static_cast<int>(byte)) ? static_cast<char>(byte) : '.');
// 		std::cout << std::endl;
//
// 		std::cout << "decomp:      " << decompress(compressedData) << std::endl;
// 	} {
// 		const auto compressedData = compress(data, 9);
// 		std::cout << "com size  9: " << compressedData.size() << std::endl;
//
// 		std::cout << "com data  9: ";
// 		for (const auto& byte : compressedData)
// 			std::cout << (std::isprint(static_cast<int>(byte)) ? static_cast<char>(byte) : '.');
// 		std::cout << std::endl;
//
// 		std::cout << "decomp:      " << decompress(compressedData) << std::endl;
// 	} {
// 		const auto compressedData = compress(data, 22);
// 		std::cout << "com size 22: " << compressedData.size() << std::endl;
//
// 		std::cout << "com data 22: ";
// 		for (const auto& byte : compressedData)
// 			std::cout << (std::isprint(static_cast<int>(byte)) ? static_cast<char>(byte) : '.');
// 		std::cout << std::endl;
//
// 		std::cout << "decomp:      " << decompress(compressedData) << std::endl;
// 	}
//
//
// 	return EXIT_SUCCESS;
// }

// =================================================================================================
// Unicode IO
//
// // So the solution on Windows is SetConsoleOutputCP(CP_UTF8), but it only works for output
// // Could not find a reliable was to handle stdin
//
// #include <cstdio>
// #include <fmt/printf.h>
// #include <fmt/ostream.h>
// #include <iostream>
// #include <io.h>
// #include <fcntl.h>
// #include <Windows.h>
//
//
// int main() {
// 	constexpr auto specimen = "кошка日本";
//
// 	std::cout << "std::cout             : " << specimen << std::endl;
// 	std::printf("std::printf           : %s\n", specimen);
// 	fmt::print("fmt::printf           : {}\n", specimen);
// 	fmt::print(std::cout, "fmt::printf(std::cout): {}\n", specimen);
//
// 	std::cout << "==========" << std::endl;
// 	// Set console code page to UTF-8 so console known how to interpret string data
// 	SetConsoleOutputCP(CP_UTF8);
// 	// SetConsoleCP(CP_UTF8);
// 	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
// 	// setvbuf(stdout, nullptr, _IOFBF, 1000);
// 	// setvbuf(stdout, nullptr, _IOFBF, 0);
//
// 	std::cout << "std::cout             : " << specimen << std::endl;
// 	std::printf("std::printf           : %s\n", specimen);
// 	fmt::print("fmt::printf           : {}\n", specimen);
// 	fmt::print(std::cout, "fmt::printf(std::cout): {}\n", specimen);
//
// 	std::string x;
// 	std::cin >> x;
// 	std::cout << "echo: " << x.size() << std::endl;
// 	std::cout << x << std::endl;
// 	std::cout << "end" << std::endl;
//
// 	// _setmode for stdout is just broken on Win 11
//
// 	// std::cout << "==========" << std::endl;
// 	// if (const int result = _setmode(_fileno(stdout), _O_U8TEXT); result == -1) {
// 	// 	std::cerr << "Cannot set mode" << std::endl;
// 	// 	return EXIT_FAILURE;
// 	// }
// 	//
// 	// std::cout << "std::cout             : " << specimen << std::endl;
// 	// std::printf("std::printf           : %s\n", specimen);
// 	// fmt::print("fmt::printf           : {}\n", specimen);
// 	// fmt::print(std::cout, "fmt::printf(std::cout): {}\n", specimen);
//
// 	return EXIT_SUCCESS;
// }

// =================================================================================================
// Resource

// #include <iostream>
// #include <filesystem>
// #include <libv/sys/executable_path.hpp>
//
// int main(int, char** argv) {
// 	std::cout << "cur: " << std::filesystem::current_path().generic_string() << std::endl;
// 	std::cout << "exe: " << libv::sys::executable_path_fs().generic_string() << std::endl;
// 	std::cout << "arg: " << std::filesystem::path(argv[0]).generic_string() << std::endl;
//
// 	return EXIT_SUCCESS;
// }

// =================================================================================================

#include <iostream>


struct S {
	int member;
};

int main(int, char**) {
	std::cout << "Hello World!" << std::endl;
	return EXIT_SUCCESS;
}
