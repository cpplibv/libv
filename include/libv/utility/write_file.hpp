// File: write_file.hpp, Created on 2018.07.14. 02:23, Author: Vader

#pragma once

// std
#include <filesystem>
#include <fstream>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename = void>
void write_file_or_throw(const std::filesystem::path& filePath, std::string_view data) {
	std::filesystem::create_directories(filePath.parent_path());

	std::ofstream file(filePath, std::ios_base::out | std::ios_base::binary);

	if (!file)
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to open file: {}", filePath.string()));

	file.write(data.data(), data.size());
	file.close();

	if (file.fail())
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to write file: {}", filePath.string()));
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
void write_file(const std::filesystem::path& filePath, std::string_view data, std::error_code& ec) {
	const auto dir = filePath.parent_path();

	if (!dir.empty()) {
		std::filesystem::create_directories(dir, ec);
		if (ec)
			return;
	}

	std::ofstream file(filePath, std::ios_base::out | std::ios_base::binary);

	if (!file) {
		ec.assign(errno, std::system_category());
		return;
	}

	file.write(data.data(), data.size());
	file.close();

	if (file.fail()) {
		ec.assign(errno, std::system_category());
		return;
	}

	ec.clear();
}

template <typename = void>
[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filePath, std::string_view data) {
	std::error_code ec;
	write_file(filePath, data, ec);
	return ec;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
