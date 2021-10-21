// Project: libv.utility, File: src/libv/utility/parse_number.hpp

// libv
#include <libv/utility/trim.hpp>
// std
#include <charconv>
#include <string_view>
#include <system_error>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
[[nodiscard]] constexpr T parse_number(std::string_view str, std::error_code& ec) noexcept {
	str = libv::trim(str);

	T value{};

	// C++20 and GCC 11.2 Should support this, but MSYS2 MinGW GCC might be lagging behind
	// Workaround: This branching could be removed once MSYS2 MinGW GCC implements from_chars to floating point types
	if constexpr (std::is_integral_v<T>) {

		std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), value);

		if (result.ec != std::errc())
			ec = std::make_error_code(result.ec);

		else if (result.ptr != str.end())
			ec = std::make_error_code(std::errc::invalid_argument);

	} else { // --- This branch will be removed ---
		size_t pos = 0;

		try {
			if constexpr (std::is_same_v<T, double>) {
				value = std::stof(std::string(str), &pos);
			} else {
				value = std::stod(std::string(str), &pos);
			}
		} catch (const std::invalid_argument& e) {
			// no conversion could be performed
			ec = std::make_error_code(std::errc::invalid_argument);
		} catch (const std::out_of_range& e) {
			// the converted value would fall out of the range of the result type or if the underlying function sets errno
			ec = std::make_error_code(std::errc::result_out_of_range);
		}

		if (pos != str.size())
			ec = std::make_error_code(std::errc::invalid_argument);
	} // --- Up until here ---

	return value;
}

template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
[[nodiscard]] constexpr inline auto parse_number_ec(std::string_view str) noexcept {
	struct Result {
		T value;
		std::error_code ec;
	};

	Result result;
	result.value = parse_number<T>(str, result.ec);
	return result;
}

template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
[[nodiscard]] constexpr inline T parse_number_or(std::string_view str, T fallback) noexcept {
	std::error_code ec;
	T value = parse_number<T>(str, ec);
	return ec ? fallback : value;
}

template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
[[nodiscard]] constexpr T parse_number_or_throw(std::string_view str) {
	std::error_code ec;
	T value = parse_number<T>(str, ec);

	// Only two errc possible: it's either out of range or it's not a number
	if (!ec)
		return value;
	else if (ec == std::make_error_code(std::errc::result_out_of_range))
		throw std::out_of_range("out of range");
	else if (ec == std::make_error_code(std::errc::invalid_argument))
		throw std::invalid_argument("invalid argument");
	else // unknown error code
		throw std::system_error(ec);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
