// Project: libv.utility, File: src/libv/utility/read_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string read_file_or(const std::filesystem::path& filepath, const std::string_view fallback);
[[nodiscard]] std::string read_file_str_or(const std::filesystem::path& filepath, const std::string_view fallback);
[[nodiscard]] std::vector<std::byte> read_file_bin_or(const std::filesystem::path& filepath, const std::vector<std::byte>& fallback);

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string read_file_or_throw(const std::filesystem::path& filepath);
[[nodiscard]] std::string read_file_str_or_throw(const std::filesystem::path& filepath);
[[nodiscard]] std::vector<std::byte> read_file_bin_or_throw(const std::filesystem::path& filepath);

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string read_file(const std::filesystem::path& filepath, std::error_code& ec);
[[nodiscard]] std::string read_file_str(const std::filesystem::path& filepath, std::error_code& ec);
[[nodiscard]] std::vector<std::byte> read_file_bin(const std::filesystem::path& filepath, std::error_code& ec);

// -------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto read_file_ec(const std::filesystem::path& filepath) {
	struct Result {
		std::string data;
		std::error_code ec;
	};

	Result result;
	result.data = read_file(filepath, result.ec);
	return result;
}

[[nodiscard]] inline auto read_file_str_ec(const std::filesystem::path& filepath) {
	return read_file_ec(filepath);
}

[[nodiscard]] inline auto read_file_bin_ec(const std::filesystem::path& filepath) {
	struct Result {
		std::vector<std::byte> data;
		std::error_code ec;
	};

	Result result;
	result.data = read_file_bin(filepath, result.ec);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
