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
//#include <libv/utility/hex_dump.hpp>
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, const char**) {
//	for (int i = 0; i <= 64; ++i) {
//		std::string str;
//		for (int j = 0; j < i; ++j) {
//			str += (j < 'z' - 'a') ? 'a' + j : 'A' + (j - ('z' - 'a'));
//		}
//
//		std::cout << libv::hex_dump_with_ascii(str) << std::endl;
//	}
//
//	return EXIT_SUCCESS;
//}
//
//// =================================================================================================


#include <cstring>
#include <iostream>
#include <hdiffpatch/HDiff/diff.h>
#include <hdiffpatch/HPatch/patch.h>
#include <libv/utility/hex_dump.hpp>
#include <libv/diff/diff.hpp>


// -------------------------------------------------------------------------------------------------


const auto read_fn = [](
		const hpatch_TStreamInput* stream,
		hpatch_StreamPos_t readFromPos,
		unsigned char* out_data,
		unsigned char* out_data_end
) mutable {
	std::cout << "read_fn" << std::endl;
	auto str = reinterpret_cast<const std::string_view*>(stream->streamImport);

	std::memcpy(out_data, str->data() + readFromPos, out_data_end - out_data);

	return hpatch_TRUE;
};

const auto write_dyn_fn = [](
		const struct hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t writeToPos,
		const unsigned char* out_data,
		const unsigned char* out_data_end
) mutable {
	std::cout << "write_dyn_fn" << std::endl;

	auto vec = reinterpret_cast<std::vector<unsigned char>*>(stream->streamImport);

	vec->resize(std::max(vec->size(), writeToPos + (out_data_end - out_data)));

	std::memcpy(vec->data() + writeToPos, out_data, out_data_end - out_data);

	return hpatch_TRUE;
};

const auto write_fix_fn = [](
		const struct hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t writeToPos,
		const unsigned char* out_data,
		const unsigned char* out_data_end
) mutable {
	std::cout << "write_fix_fn" << std::endl;

	auto vec = reinterpret_cast<std::vector<unsigned char>*>(stream->streamImport);

	std::memcpy(vec->data() + writeToPos, out_data, out_data_end - out_data);

	return hpatch_TRUE;
};

const auto read_back_fn = [](
		const struct hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t readFromPos,
		unsigned char* data,
		unsigned char* data_end
) mutable {
	std::cout << "read_back_fn" << std::endl;

	auto str = reinterpret_cast<std::vector<unsigned char>*>(stream->streamImport);

	std::memcpy(data, str->data() + readFromPos, data_end - data);

	return hpatch_TRUE;
};


void my_create_compressed_diff_stream(std::string_view old_, std::string_view new_, std::vector<unsigned char>& diff_) {
	hpatch_TStreamInput old_stream;
	old_stream.streamImport = reinterpret_cast<void*>(&old_);
	old_stream.streamSize = old_.size();
	old_stream.read = read_fn;

	hpatch_TStreamInput new_stream;
	new_stream.streamImport = reinterpret_cast<void*>(&new_);
	new_stream.streamSize = new_.size();
	new_stream.read = read_fn;

	hpatch_TStreamOutput diff_stream;
	diff_stream.streamImport = reinterpret_cast<void*>(&diff_);
	diff_stream.streamSize = std::numeric_limits<size_t>::max();
	diff_stream.write = write_dyn_fn;
	diff_stream.read_writed = read_back_fn;

	create_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr, 16);
}

bool my_check_compressed_diff_stream(std::string_view old_, std::string_view new_, std::string_view diff_) {

	hpatch_TStreamInput old_stream;
	old_stream.streamImport = reinterpret_cast<void*>(&old_);
	old_stream.streamSize = old_.size();
	old_stream.read = read_fn;

	hpatch_TStreamInput new_stream;
	new_stream.streamImport = reinterpret_cast<void*>(&new_);
	new_stream.streamSize = new_.size();
	new_stream.read = read_fn;

	hpatch_TStreamInput diff_stream;
	diff_stream.streamImport = reinterpret_cast<void*>(&diff_);
	diff_stream.streamSize = diff_.size();
	diff_stream.read = read_fn;

	return check_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr);
}


bool my_apply_patch(std::string_view old_, std::string_view diff_, std::vector<unsigned char>& new_) {
	hpatch_compressedDiffInfo diff_info;
	getCompressedDiffInfo_mem(&diff_info,
			reinterpret_cast<const unsigned char*>(diff_.data()),
			reinterpret_cast<const unsigned char*>(diff_.data() + diff_.size())
	);

	std::cout << "diff_info.newDataSize: " << diff_info.newDataSize << std::endl;
	std::cout << "diff_info.oldDataSize: " << diff_info.oldDataSize << std::endl;
	std::cout << "diff_info.compressType: " << diff_info.compressType << std::endl;
	std::cout << "diff_info.compressedCount: " << diff_info.compressedCount << std::endl;

	new_.resize(diff_info.newDataSize);

	hpatch_TStreamInput old_stream;
	old_stream.streamImport = reinterpret_cast<void*>(&old_);
	old_stream.streamSize = old_.size();
	old_stream.read = read_fn;

	hpatch_TStreamInput diff_stream;
	diff_stream.streamImport = reinterpret_cast<void*>(&diff_);
	diff_stream.streamSize = diff_.size();
	diff_stream.read = read_fn;

//	hpatch_TStreamInput diff_stream;
//	mem_as_hStreamInput(
//			&diff_stream,
//			reinterpret_cast<const unsigned char*>(diff_.data()),
//			reinterpret_cast<const unsigned char*>(diff_.data() + diff_.size())
//	);

	hpatch_TStreamOutput new_stream;
	new_stream.streamImport = reinterpret_cast<void*>(&new_);
//	new_stream.streamSize = std::numeric_limits<size_t>::max();
	new_stream.streamSize = new_.size();
	new_stream.write = write_fix_fn;
	new_stream.read_writed = read_back_fn;


	const auto success = patch_decompress(&new_stream, &old_stream, &diff_stream, nullptr);




	return success;
}

int main(int, const char**) {
	const std::string_view old_ = "<libv/utilessed it by yourself or use create_compressed_diff()/patch_decompress() create compressed diffData;\n"
	                              "//\tif your file size very __ grelkj 999`888 98 large or request faster and less memity/hex_dump.hpp>handle_disconnectb";
	const std::string_view new_ = "<hdiffpatch/HDiessed it by yourself or use c555()/patch_decompress() create compressed diffData;\n"
							      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
                                  "//\tif your file size very __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request faster and less memff/diff.h>handle_disconnecta";
//	const std::string_view old_ = "f your file sizef your file size very large or rvery large or r";
//	const std::string_view new_ = "f your file sizef your file size avery large or rvery large or r";
//	const std::string_view old_ = "";
//	const std::string_view new_ = "a";
	std::vector<unsigned char> diff;

//	libv::diff::create_diff()

//	create_diff(
//	create_compressed_diff(
//			reinterpret_cast<const unsigned char*>(new_.begin()),
//			reinterpret_cast<const unsigned char*>(new_.end()),
//			reinterpret_cast<const unsigned char*>(old_.begin()),
//			reinterpret_cast<const unsigned char*>(old_.end()),
//			diff
//	);

	my_create_compressed_diff_stream(old_, new_, diff);

	const auto diff_str = std::string_view(reinterpret_cast<const char*>(diff.data()), diff.size());

//	std::cout << "Str:" << std::endl;
//	std::cout << diff_str << std::endl;
	std::cout << "New size: " << new_.size() << std::endl;
	std::cout << "Old size: " << old_.size() << std::endl;
	std::cout << "Diffsize: " << diff_str.size() << std::endl;

	std::cout << "Hex:" << std::endl;
	std::cout << libv::hex_dump_with_ascii(diff_str) << std::endl;

//	const auto check_result = check_compressed_diff(
//	const auto check_result = check_diff(
//	const auto check_result = check_compressed_diff(
//			reinterpret_cast<const unsigned char*>(new_.begin()),
//			reinterpret_cast<const unsigned char*>(new_.end()),
//			reinterpret_cast<const unsigned char*>(old_.begin()),
//			reinterpret_cast<const unsigned char*>(old_.end()),
//			diff.data(),
//			diff.data() + diff.size()
//			,nullptr
//	);

	const auto check_result = my_check_compressed_diff_stream(old_, new_, diff_str);

	std::cout << "check_result: " << check_result << std::endl;

	std::vector<unsigned char> patched;

//	uint64_t patched_size = 0;
//	const auto kNewDataSizeSavedSize = readSavedSize(diff.data(), diff.size(), &patched_size);
//	std::cout << "kNewDataSizeSavedSize: " << kNewDataSizeSavedSize << std::endl;
//	std::cout << "patched_size: " << patched_size << std::endl;

//	hpatch_compressedDiffInfo diff_info;
//	getCompressedDiffInfo_mem(&diff_info,
//			diff.data(),
//			diff.data() + diff.size()
//	);
//
//	std::cout << "diff_info.newDataSize: " << diff_info.newDataSize << std::endl;
//	std::cout << "diff_info.oldDataSize: " << diff_info.oldDataSize << std::endl;
//	std::cout << "diff_info.compressType: " << diff_info.compressType << std::endl;
//	std::cout << "diff_info.compressedCount: " << diff_info.compressedCount << std::endl;
//
//	patched.resize(diff_info.newDataSize);

	const auto patch_result = my_apply_patch(old_, diff_str, patched);
//	const auto patch_result = patch(
//	const auto patch_result = patch_decompress_mem(
//////	const auto patch_result = patch_decompress(
//			patched.data(),
//			patched.data() + patched.size(),
//			reinterpret_cast<const unsigned char*>(old_.begin()),
//			reinterpret_cast<const unsigned char*>(old_.end()),
//			diff.data(),
//			diff.data() + diff.size()
//			,nullptr
//	);
//
	std::cout << "patch_result: " << patch_result << std::endl;
//
	const auto patched_str = std::string_view(reinterpret_cast<const char*>(patched.data()), patched.size());
	std::cout << "Patched:" << std::endl;
	std::cout << patched_str << std::endl;

	std::cout << "Matches new the patched: " << (new_ == patched_str) << std::endl;



	return EXIT_SUCCESS;
}

// =================================================================================================

