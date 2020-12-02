// Project: libv.utility, File: src/libv/utility/read_file.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/utility/read_file.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <filesystem>
#include <fstream>
#include <string_view>
#include <system_error>


namespace libv {
namespace {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] T aux_read_file_or_throw(const std::filesystem::path& filepath) {
	T result;
	std::ifstream file(filepath, std::ios::binary | std::ios::in | std::ios::ate);

	if (!file)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filepath.generic_string()));

	const auto size = file.tellg();
	file.seekg(0);

	if (file.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", filepath.generic_string()));

	result.resize(size);

	file.read(reinterpret_cast<char*>(result.data()), size);

	if (file.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", filepath.generic_string()));

	return result;
}

template <typename T>
[[nodiscard]] T aux_read_file(const std::filesystem::path& filepath, std::error_code& ec) {
	T result;
	std::ifstream file(filepath, std::ios::binary | std::ios::in | std::ios::ate);

	if (!file) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		result.clear();
		return result;
	}

	const auto size = file.tellg();
	file.seekg(0);

	if (file.fail()) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		result.clear();
		return result;
	}

	result.resize(size);

	file.read(reinterpret_cast<char*>(result.data()), size);

	if (file.fail()) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		result.clear();
		return result;
	}

	ec.clear();
	return result;
}

} // namespace -------------------------------------------------------------------------------------


std::string read_file_or(const std::filesystem::path& filepath, const std::string_view fallback) {
	auto r = read_file_ec(filepath);
	return r.ec ? std::string(fallback) : std::move(r.data);
}

std::string read_file_str_or(const std::filesystem::path& filepath, const std::string_view fallback) {
	auto r = read_file_str_ec(filepath);
	return r.ec ? std::string(fallback) : std::move(r.data);
}

std::vector<std::byte> read_file_bin_or(const std::filesystem::path& filepath, const std::vector<std::byte>& fallback) {
	auto r = read_file_bin_ec(filepath);
	return r.ec ? fallback : std::move(r.data);
}

// ---

std::string read_file_or_throw(const std::filesystem::path& filepath) {
	return aux_read_file_or_throw<std::string>(filepath);
}

std::string read_file_str_or_throw(const std::filesystem::path& filepath) {
	return aux_read_file_or_throw<std::string>(filepath);
}

std::vector<std::byte> read_file_bin_or_throw(const std::filesystem::path& filepath) {
	return aux_read_file_or_throw<std::vector<std::byte>>(filepath);
}

// ---

std::string read_file(const std::filesystem::path& filepath, std::error_code& ec) {
	return aux_read_file<std::string>(filepath, ec);
}

std::string read_file_str(const std::filesystem::path& filepath, std::error_code& ec) {
	return aux_read_file<std::string>(filepath, ec);
}

std::vector<std::byte> read_file_bin(const std::filesystem::path& filepath, std::error_code& ec) {
	return aux_read_file<std::vector<std::byte>>(filepath, ec);
}


// -------------------------------------------------------------------------------------------------

} // namespace libv
