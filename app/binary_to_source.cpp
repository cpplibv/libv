// Project: libv, File: app/binary_to_source.cpp

// ext
#include <fmt/format.h>
#include <fmt/ostream.h>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/utility/generic_path.hpp>
// std
#include <iostream>
#include <sstream>


// -------------------------------------------------------------------------------------------------

const auto payload_max_line_Length = 255;
const auto file_template_hpp =
R"(// File: {output_file}.hpp - Generated on YYYY.MM.DD. HH:mm from {input_file}

// std
#include <cstdint>
#include <utility>


// -------------------------------------------------------------------------------------------------

std::pair<void*, std::size_t> {function_name}();

// -------------------------------------------------------------------------------------------------
)";

const auto file_template_cpp =
R"(// File: {output_file}.cpp - Generated on YYYY.MM.DD. HH:mm from {input_file}

// pro
#include <{output_file}.hpp>


// -------------------------------------------------------------------------------------------------

std::pair<void*, std::size_t> {function_name}() {{
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

	std::ostringstream payload;

	auto lineLength = 0uz;
	for (const auto& byte_ : content) {
		const auto byteStr = fmt::format("{:d},", static_cast<uint8_t>(byte_));

		if (lineLength + byteStr.size() > payload_max_line_Length - 1) {
			payload << '\n';
			lineLength = 0;
		}
		payload << byteStr;
		lineLength += byteStr.size();
	}

	auto ec_write_hpp = libv::write_file_ec(libv::concat(output_file, ".hpp"), fmt::format(fmt::runtime(file_template_hpp),
			fmt::arg("input_file", input_file),
			fmt::arg("output_file", output_file),
			fmt::arg("function_name", function_name),
			fmt::arg("size", size)
	));

	if (ec_write_hpp) {
		fmt::print("Failed to write file: {}: {} - {}\n", output_file, ec_write_hpp, ec_write_hpp.message());
		return EXIT_FAILURE;
	}

	auto ec_write_cpp = libv::write_file_ec(libv::concat(output_file, ".cpp"), fmt::format(fmt::runtime(file_template_cpp),
			fmt::arg("input_file", input_file),
			fmt::arg("output_file", output_file),
			fmt::arg("function_name", function_name),
			fmt::arg("payload", payload.str()),
			fmt::arg("size", size)
	));

	if (ec_write_cpp) {
		fmt::print("Failed to write file: {}: {} - {}\n", output_file, ec_write_cpp, ec_write_cpp.message());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// =================================================================================================
//
// // Image support
//
//#include <libv/gl/image.hpp>
//#include <libv/gl/load_image.hpp>
//#include <libv/utility/read_file.hpp>
//
//std::vector<libv::frame::Icon> generate_icons() {
//	std::vector<libv::frame::Icon> icons;
//
//
//	auto gen = [&](auto path, auto test_r, auto test_g, auto test_b){
//		const auto image = libv::gl::load_image_or_throw(libv::read_file_str_or_throw(path));
//		std::cout << path << std::endl;
//
//		libv::frame::Icon icon;
//
//		icon.size = image.size();
//		icon.pixels.resize(icon.size.x * icon.size.y);
//		for (int y = 0; y < image.size().y; ++y) {
//			for (int x = 0; x < image.size().x; ++x) {
//				icon.pixels[icon.size.x * y + x] = image.pixel(0, x, y).template cast<uint8_t>();
//
//				std::cout << "{";
//				std::cout << image.pixel(0, x, y).x << ", ";
//				std::cout << image.pixel(0, x, y).y << ", ";
//				std::cout << image.pixel(0, x, y).z << ", ";
//				std::cout << image.pixel(0, x, y).w << "}, ";
//			}
//			std::cout << std::endl;
//		}
//
//		icons.push_back(icon);
//	};
//
//	gen("d:\\project\\iris\\texture\\ico\\cyan_16.png");
////	gen("d:\\project\\iris\\texture\\ico\\cyan_24.png");
//	gen("d:\\project\\iris\\texture\\ico\\cyan_32.png");
//	gen("d:\\project\\iris\\texture\\ico\\cyan_48.png");
////	gen("d:\\project\\iris\\texture\\ico\\cyan_64.png");
//
//	return icons;
//}
