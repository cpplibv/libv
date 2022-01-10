// Project: libv.utility, File: src/libv/utility/hex_dump.hpp

#pragma once

// std
#include <span>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string hex_dump(const std::string_view& s);

[[nodiscard]] inline std::string hex_dump(const void* data, std::size_t size) {
	return hex_dump(std::string_view(reinterpret_cast<const char*>(data), size));
}

[[nodiscard]] inline std::string hex_dump(const std::span<const std::byte> s) {
	return hex_dump(std::string_view(reinterpret_cast<const char*>(s.data()), s.size()));
}

template <typename T>
[[nodiscard]] inline std::string hex_dump_var(const T& var) {
	return hex_dump(std::string_view(reinterpret_cast<const char*>(&var), sizeof(T)));
}

[[nodiscard]] std::string hex_dump_with_ascii(const std::string_view& s);

[[nodiscard]] inline std::string hex_dump_with_ascii(const void* data, std::size_t size) {
	return hex_dump_with_ascii(std::string_view(reinterpret_cast<const char*>(data), size));
}

[[nodiscard]] inline std::string hex_dump_with_ascii(const std::span<const std::byte> s) {
	return hex_dump_with_ascii(std::string_view(reinterpret_cast<const char*>(s.data()), s.size()));
}

template <typename T>
[[nodiscard]] inline std::string hex_dump_var_with_ascii(const T& var) {
	return hex_dump_with_ascii(std::string_view(reinterpret_cast<const char*>(&var), sizeof(T)));
}


// -------------------------------------------------------------------------------------------------

} // namespace libv
