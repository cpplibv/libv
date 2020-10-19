// Project: libv.utility, File: src/libv/utility/read_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <fmt/format.h>
// libv
#include <libv/log/log.hpp>
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
	std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		const auto err = errno;
		log.error("Failed to open file: {} - {}: {}", filePath, err, std::system_category().message(err));
		return std::string{std::forward<Fallback>(fallback)};
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail()) {
		const auto err = errno;
		log.error("Failed to read file: {} - {}: {}", filePath, err, std::system_category().message(err));
		return std::string{std::forward<Fallback>(fallback)};
	}

	return std::move(buffer).str();
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file_or_throw(const std::filesystem::path& filePath) {
	std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);

	if (!file)
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to open file: {}", filePath));

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail())
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to read file: {}", filePath));

	return std::move(buffer).str();
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file(const std::filesystem::path& filePath, std::error_code& ec) {
	std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		ec.assign(errno, std::system_category());
		return "";
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	if (file.fail()) {
		ec.assign(errno, std::system_category());
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