// Project: libv, File: app/binary_to_source.cpp, Author: Császár Mátyás [Vader]

// ext
#include <fmt/format.h>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/utility/generic_path.hpp>
// std
#include <sstream>
#include <iostream>


// -------------------------------------------------------------------------------------------------

const auto payload_max_line_lenght = 255;
const auto file_template_hpp =
R"(// File: {output_file}.hpp - Generated on YYYY.MM.DD. HH:mm from {input_file}

// std
#include <cstdint>
#include <utility>


// -------------------------------------------------------------------------------------------------

std::pair<void*, size_t> {function_name}();

// -------------------------------------------------------------------------------------------------
)";

const auto file_template_cpp =
R"(// File: {output_file}.cpp - Generated on YYYY.MM.DD. HH:mm from {input_file}

// pro
#include <{output_file}.hpp>


// -------------------------------------------------------------------------------------------------

std::pair<void*, size_t> {function_name}() {{
	static constexpr unsigned char data[] = {{
{payload}
	}};
	return {{data, {size}}};
}}

// -------------------------------------------------------------------------------------------------
)";

int main(int argc, const char** argv) {
	if (argc != 3 && argc != 4) {
		fmt::print("Expected 2 or 3 argument but received {}\n", argc);
		fmt::print("Usage: {} <input_file> <output_file> [function_name]\n", libv::generic_path(argv[0]));
		return EXIT_FAILURE;
	}

	const auto input_file = argv[1];
	const auto output_file = argv[2];
	const auto function_name = argc < 4 ? "get_raw_data" : argv[3];

	fmt::print("Input   : {}\n", input_file);
	fmt::print("Output  : {}\n", output_file);
	fmt::print("Function: {}\n", function_name);

	const auto [content, ec_read] = libv::read_file_ec(input_file);
	if (ec_read) {
		fmt::print("Failed to read file: {}: {} - {}\n", input_file, ec_read, ec_read.message());
		return EXIT_FAILURE;
	}

	const auto size = content.size();

	std::ostringstream paylaod;

	auto lineLenght = size_t{0};
	for (const auto& btye_ : content) {
		const auto btyestr = fmt::format("{:d},", static_cast<uint8_t>(btye_));

		if (lineLenght + btyestr.size() > payload_max_line_lenght - 1) {
			paylaod << '\n';
			lineLenght = 0;
		}
		paylaod << btyestr;
		lineLenght += btyestr.size();
	}

	auto ec_write_hpp = libv::write_file_ec(libv::concat(output_file, ".hpp"), fmt::format(file_template_hpp,
			fmt::arg("input_file", input_file),
			fmt::arg("output_file", output_file),
			fmt::arg("function_name", function_name),
			fmt::arg("size", size)
	));

	if (ec_write_hpp) {
		fmt::print("Failed to write file: {}: {} - {}\n", output_file, ec_write_hpp, ec_write_hpp.message());
		return EXIT_FAILURE;
	}

	auto ec_write_cpp = libv::write_file_ec(libv::concat(output_file, ".cpp"), fmt::format(file_template_cpp,
			fmt::arg("input_file", input_file),
			fmt::arg("output_file", output_file),
			fmt::arg("function_name", function_name),
			fmt::arg("payload", paylaod.str()),
			fmt::arg("size", size)
	));

	if (ec_write_cpp) {
		fmt::print("Failed to write file: {}: {} - {}\n", output_file, ec_write_cpp, ec_write_cpp.message());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
