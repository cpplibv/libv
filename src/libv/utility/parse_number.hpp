// Project: libv.utility, File: src/libv/utility/parse_number.hpp

// libv
#include <libv/algo/trim.hpp>
// std
#include <charconv>
#include <string_view>
#include <system_error>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
		requires std::is_arithmetic_v<T>
[[nodiscard]] constexpr T parse_number(std::string_view str, std::error_code& ec) noexcept {
	str = libv::trim(str);

	T value{};

	std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), value);

	if (result.ec != std::errc())
		ec = std::make_error_code(result.ec);

	else if (result.ptr != str.end())
		ec = std::make_error_code(std::errc::invalid_argument);

	return value;
}

template <typename T>
		requires std::is_arithmetic_v<T>
[[nodiscard]] constexpr inline auto parse_number_ec(std::string_view str) noexcept {
	struct Result {
		T value;
		std::error_code ec;
	};

	Result result;
	result.value = parse_number<T>(str, result.ec);
	return result;
}

template <typename T>
		requires std::is_arithmetic_v<T>
[[nodiscard]] constexpr inline T parse_number_or(std::string_view str, T fallback) noexcept {
	std::error_code ec;
	T value = parse_number<T>(str, ec);
	return ec ? fallback : value;
}

template <typename T>
		requires std::is_arithmetic_v<T>
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
