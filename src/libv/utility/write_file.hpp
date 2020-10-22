// Project: libv.utility, File: src/libv/utility/write_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/concat.hpp>
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
		throw std::system_error(errno, std::system_category(), libv::concat("Failed to open file: ", filePath.string()));

	file.write(data.data(), data.size());
	file.close();

	if (file.fail())
		throw std::system_error(errno, std::system_category(), libv::concat("Failed to write file: ", filePath.string()));
}

inline void write_file_or_throw(const std::filesystem::path& filePath, std::span<const std::byte> data) {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	write_file_or_throw(filePath, std::string_view{ptr, size});
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

inline void write_file(const std::filesystem::path& filePath, std::span<const std::byte> data, std::error_code& ec) {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	write_file(filePath, std::string_view{ptr, size}, ec);
}

template <typename = void>
[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filePath, std::string_view data) {
	std::error_code ec;
	write_file(filePath, data, ec);
	return ec;
}

[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filePath, std::span<const std::byte> data) {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	return write_file_ec(filePath, std::string_view{ptr, size});
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
