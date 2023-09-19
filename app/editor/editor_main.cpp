// Project: libv, File: app/editor/editor_main.cpp

#include <ctre.hpp>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/printf.h>

#include <libv/arg/arg.hpp>
#include <libv/range/view_lines_string_view.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


// -------------------------------------------------------------------------------------------------

std::optional<std::string_view> extract_include(const std::string_view line) noexcept {
	if (auto m = ctre::match<R"qq(^[ \t]*(\\/\\/[ \t\\/]*)?[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq">(line)) {
		return m.get<1>().to_view().empty() ? m.get<2>().to_view() : m.get<1>().to_view();
	} else {
		return std::nullopt;
	}
}

bool hasInclude(const std::string_view line) noexcept {
	return ctre::match<R"qq(^[ \t]*(\\/\\/[ \t\\/]*)?[ \t]*#[ \t]*include[ \t]+.*)qq">(line);
}

bool isComment(const std::string_view line) noexcept {
	return ctre::match<R"qq(^[ \t]*//+.*)qq">(line);
}

bool isEmpty(const std::string_view line) noexcept {
	return line == "\n" || ctre::match<R"qq(^[ \t]*$)qq">(line);
}

bool isPP(const std::string_view line) noexcept {
	return ctre::match<R"qq(^[ \t]*(\\/\\/[ \t\\/]*)?[ \t]*#.*)qq">(line);
}

std::optional<std::string_view> extract_pp(const std::string_view line) noexcept {
	if (auto m = ctre::match<R"qq(^[ \t]*(\\/\\/[ \t\\/]*)?[ \t]*#([a-zA-Z]+).*)qq">(line)) {
		return m.get<1>().to_view();
	} else {
		return std::nullopt;
	}
}

bool canPPEffectsInclude(std::optional<std::string_view> pp) {
	return pp && (
			pp == "if" ||
			pp == "ifdef" ||
			pp == "ifndef" ||
			pp == "elif" ||
			pp == "elifdef" ||
			pp == "elifndef" ||
			pp == "else" ||
			pp == "undef" ||
			pp == "endif" ||
			pp == "pragma");
}

// -------------------------------------------------------------------------------------------------

bool isLineBeforeIncludes(const std::string_view line) noexcept {
	return !hasInclude(line) && (isPP(line) || isComment(line) || isEmpty(line));
}

bool isLineAcceptedAsInclude(const std::string_view line) noexcept {
	return !canPPEffectsInclude(extract_pp(line)) && (hasInclude(line) || isComment(line) || isEmpty(line));
}

// - Keep comments at the end of an include
// - Keep subsequent lines that are commented but contain no #include with their original preceding include line
// 		IF the preceding include has a comment at the end
//		Otherwise keep it with the following include if that has no comment
//		Otherwise keep it with preceding include
// - Sort commented #include lines too
// - Once the meat started aka non-commented non-empty characters are detected stop the process of the file
//
// - ? Track ifdef blocks and stop sorting or consider ther meaning and sort them too
//
// - Special order for main-header files (the header with the same name but (could be more than 1 hpp/hxx))
// - Special order for fwd headers
// - Warn if hxx is included from hpp

struct IncludeSorter {
	struct Group {
		std::string regex;
		int order;
	};

	struct IncludeEntry {
		std::string include;
		std::string line;
		bool prevIncludeHasInlineComment = false;
		bool nextIncludeHasInlineComment = false;
		std::string prevCommentedLines;
		std::string nextCommentedLines;
		// bool isMainHeader = false;

		Group* group = nullptr;
	};

	std::vector<Group> groups;
	std::vector<IncludeEntry> entries;

public:
	Group* findGroup(std::string_view filename, std::string_view include) {
		(void) filename;
		(void) include;
		// libv::linear_find_if(groups, ) ...
		return nullptr;
	}
	std::string process(std::string filename, std::string fileContent) {
		std::ostringstream osBeforeIncludes;
		std::ostringstream osIncludes;
		std::ostringstream osAfterIncludes;

		bool beforeIncudes = true;
		bool afterIncudes = false;

		for (const auto& line : fileContent | libv::view::lines_string_view) {
			if (beforeIncudes) {
				if (isLineBeforeIncludes(line))
					osBeforeIncludes << line;
				else
					beforeIncudes = false;
			}
			if (!beforeIncudes && !afterIncudes) {
				if (isLineAcceptedAsInclude(line)) {
					if (auto include = extract_include(line)) {
						entries.push_back(IncludeEntry{
							std::string(*include),
							std::string(line),
							false,
							false,
							"",
							"",
							findGroup(filename, *include)
						});
					} else if (isComment(line)) {
						// ADD COMMENT LINE before and after the entires
					}

				} else
					afterIncudes = true;
			}
			if (afterIncudes)
				osAfterIncludes << line;
		}

		// fmt::print("isEmpty(''): {}\n", isEmpty(""));
		// fmt::print("isEmpty(' '): {}\n", isEmpty(" "));
		// fmt::print("isEmpty('\t'): {}\n", isEmpty("\t"));
		// fmt::print("isEmpty('\t  '): {}\n", isEmpty("\t  "));
		// fmt::print("isEmpty('\t  \t '): {}\n", isEmpty("\t  \t "));
		fmt::print("{}", std::move(osBeforeIncludes).str());
		fmt::print("==============================================================================\n");
		fmt::print("{}", std::move(osAfterIncludes).str());

		return "";
	}
};

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Editor", "Source file editor and formatter utility");

	auto filename = args.require<std::string>
			("-f", "--file")
			("file", "File path of the source file to be formatted");

	args.positional(filename);
	args.require_no_unused();

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	std::cout << args.report(100);

#if not WISH_BUILD_PACKAGE
	std::filesystem::current_path(WISH_PATH_TO_SOURCE);
#endif

	// if (argc != 3 && argc != 4) {
	// 	fmt::print("Expected 2 or 3 argument but received {}\n", argc);
	// 	fmt::print("Usage: {} <input_file> <output_file> [function_name]\n", std::filesystem::path(argv[0]).generic_string());
	// 	return EXIT_FAILURE;
	// }
	//
	// const auto input_file = argv[1];
	// const auto output_file = argv[2];
	// const auto function_name = argc < 4 ? "get_raw_data" : argv[3];

	// fmt::print("Input   : {}\n", input_file);
	// fmt::print("Output  : {}\n", output_file);
	// fmt::print("Function: {}\n", function_name);

	const auto [content, ec_read] = libv::read_file_ec(filename.value());
	if (ec_read) {
		fmt::print("Failed to read file: {}: {} ({})\n", filename.value(), ec_read.message(), ec_read);
		return EXIT_FAILURE;
	}

	IncludeSorter sorter;
	sorter.process(filename.value(), content);

	// const auto size = content.size();
	//
	// std::ostringstream payload;
	//
	// auto lineLength = 0uz;
	// for (const auto& byte_ : content) {
	// 	const auto byteStr = fmt::format("{:d},", static_cast<uint8_t>(byte_));
	//
	// 	if (lineLength + byteStr.size() > payload_max_line_Length - 1) {
	// 		payload << '\n';
	// 		lineLength = 0;
	// 	}
	// 	payload << byteStr;
	// 	lineLength += byteStr.size();
	// }
	//
	// auto ec_write = libv::write_file_ec(fmt::format("{}.hpp", output_file), fmt::format(fmt::runtime(file_template_hpp),
	// 		fmt::arg("input_file", input_file),
	// 		fmt::arg("output_file", output_file),
	// 		fmt::arg("function_name", function_name),
	// 		fmt::arg("size", size)
	// ));
	//
	// if (ec_write) {
	// 	fmt::print("Failed to write file: {}: {} - {}\n", output_file, fmt::streamed(ec_write_hpp), ec_write_hpp.message());
	// 	return EXIT_FAILURE;
	// }

	return EXIT_SUCCESS;
}
