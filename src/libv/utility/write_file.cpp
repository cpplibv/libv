// Project: libv.utility, File: src/libv/utility/write_file.cpp

// hpp
#include <libv/utility/write_file.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <filesystem>
#include <fstream>
#include <span>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

void write_file_or_throw(const std::filesystem::path& filepath, std::string_view data) {
	const auto dir = filepath.parent_path();

	if (!dir.empty()) {
		std::error_code cd_ec;
		std::filesystem::create_directories(filepath.parent_path(), cd_ec);
		if (cd_ec)
			throw std::system_error(cd_ec, libv::concat("Failed to create directory: ", dir.generic_string(), " for writing file: ", filepath.generic_string()));
	}

	std::ofstream file(filepath, std::ios::binary | std::ios::out);

	if (!file)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filepath.generic_string()));

	file.write(data.data(), data.size());
	file.close();

	if (file.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to write file: ", filepath.generic_string()));
}

void write_file_or_throw(const std::filesystem::path& filepath, std::span<const std::byte> data) {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	write_file_or_throw(filepath, std::string_view{ptr, size});
}

// -------------------------------------------------------------------------------------------------

void write_file(const std::filesystem::path& filepath, std::string_view data, std::error_code& ec) noexcept {
	const auto dir = filepath.parent_path();

	if (!dir.empty()) {
		std::filesystem::create_directories(dir, ec);
		if (ec)
			return;
	}

	std::ofstream file(filepath, std::ios::binary | std::ios::out);

	if (!file) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return;
	}

	file.write(data.data(), data.size());
	file.close();

	if (file.fail()) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return;
	}

	ec.clear();
}

void write_file(const std::filesystem::path& filepath, std::span<const std::byte> data, std::error_code& ec) noexcept {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	write_file(filepath, std::string_view{ptr, size}, ec);
}

// -------------------------------------------------------------------------------------------------

std::error_code write_file_ec(const std::filesystem::path& filepath, std::string_view data) noexcept {
	std::error_code ec;
	write_file(filepath, data, ec);
	return ec;
}

std::error_code write_file_ec(const std::filesystem::path& filepath, std::span<const std::byte> data) noexcept {
	const auto ptr = reinterpret_cast<const char*>(data.data());
	const auto size = data.size();
	return write_file_ec(filepath, std::string_view{ptr, size});
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
