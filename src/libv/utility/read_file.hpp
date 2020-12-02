// Project: libv.utility, File: src/libv/utility/read_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/log/log.hpp>
#include <libv/utility/concat.hpp>
// std
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Fallback = std::string_view>
[[nodiscard]] std::string read_file_or(const std::filesystem::path& filePath, Fallback&& fallback = "") {
	std::ifstream file(filePath, std::ios_base::binary | std::ios_base::in);

	if (!file) {
		const auto err = errno;
		log.error("Failed to open file: {} - {}: {}", filePath.generic_string(), err, std::system_category().message(err));
		return std::string{std::forward<Fallback>(fallback)};
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail()) {
		const auto err = errno;
		log.error("Failed to read file: {} - {}: {}", filePath.generic_string(), err, std::system_category().message(err));
		return std::string{std::forward<Fallback>(fallback)};
	}

	return std::move(buffer).str();
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file_or_throw(const std::filesystem::path& filePath) {
	std::ifstream file(filePath, std::ios_base::binary | std::ios_base::in);

	if (!file)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filePath.generic_string()));

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", filePath.generic_string()));

	return std::move(buffer).str();
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file(const std::filesystem::path& filePath, std::error_code& ec) {
	std::ifstream file(filePath, std::ios_base::binary | std::ios_base::in);

	if (!file) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return "";
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail()) {
		ec = std::make_error_code(static_cast<std::errc>(errno));
		return "";
	}

	ec.clear();
	return std::move(buffer).str();
}

template <typename = void>
[[nodiscard]] auto read_file_ec(const std::filesystem::path& filePath) {
	struct Result {
		std::string data;
		std::error_code ec;
	};

	Result result;
	result.data = read_file(filePath, result.ec);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
