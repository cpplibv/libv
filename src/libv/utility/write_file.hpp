// Project: libv.utility, File: src/libv/utility/write_file.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <span>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

void write_file_or_throw(const std::filesystem::path& filepath, std::string_view data);
void write_file_or_throw(const std::filesystem::path& filepath, std::span<const std::byte> data);

// -------------------------------------------------------------------------------------------------

void write_file(const std::filesystem::path& filepath, std::string_view data, std::error_code& ec) noexcept;
void write_file(const std::filesystem::path& filepath, std::span<const std::byte> data, std::error_code& ec) noexcept;

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filepath, std::string_view data) noexcept;
[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filepath, std::span<const std::byte> data) noexcept;

// -------------------------------------------------------------------------------------------------

} // namespace libv
