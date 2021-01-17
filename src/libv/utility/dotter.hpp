// Project: libv.utility, File: src/libv/utility/parse_number.hpp, Author: Császár Mátyás [Vader]

// libv
//#include <libv/utility/parse_number.hpp>
// std
#include <charconv>
#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

//
//template <typename T, unsigned long N = std::numeric_limits<T>::max()>
//class MaxLen {
//public:
//	enum {
//		StringLen = MaxLen<T, N / 10>::StringLen + 1
//	};
//};
//
//template <typename T>
//class MaxLen<T, 0> {
//public:
//	enum {
//		StringLen = 1
//	};
//};
//
//MaxLen<short>::StringLen;
//
//// for double 1077
//
//void dotted(int64_t a) {
//	int64_t c = 1;
//
//	if (a < 0) {
//		a *= -1;
//		std::cout << "-";
//	}
//	while ((c *= 1000) < a);
//	while (c > 1) {
//		int64_t t = (a % c) / (c / 1000);
//		std::cout << (((c > a) || (t > 99)) ? "" : ((t > 9) ? "0" : "00")) << t;
//		std::cout << (((c /= 1000) == 1) ? "" : "'");
//	}
//}
//

static constexpr auto max_decimal_ascii_length = 1077;

void dotter(int64_t number, char thousand_separator = ' ') {
	char buffer[max_decimal_ascii_length * 4 / 3];

	const auto begin = std::begin(buffer);
	const auto end = std::to_chars(begin, std::end(buffer), number).ptr;

	const auto size = std::distance(begin, end);

	find_dot;
	every_3rd_character_from_dot_insert_new_separator;
	every_3rd_character_after_dot_insert_new_separator;

	for (auto it = begin; it != end; ++it) {
		if_++;
		if_++;
		if_++;
		add_sep;
	}
}


//TODO P4: libv.utility: Storage space size formatter: xxx'xxx.yy KB, MB, GB
//	This will absorb the dotter that I have in fsw


struct FormatFoo {
	static constexpr auto sign_positive = "";
	static constexpr auto sign_negative = "-";

	static constexpr auto max_precision = 3;
	static constexpr auto min_precision = 2;

	static constexpr auto group_size = 3;
	static constexpr auto group_separator = " ";

	static constexpr auto special_nan = "NaN";
	static constexpr auto special_n_nan = "-NaN";
	static constexpr auto special_inf = "inf";
	static constexpr auto special_n_inf = "-inf";

	static constexpr auto unit_prefix_separator = " ";
	static constexpr auto unit_prefixes = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
//	static constexpr auto unit_prefixes = {"n", "u", "m", "", "K", "M", "G", "T", "P", "E", "Z", "Y"};
};

template <typename FormatArguments>
void dotter_hu(int64_t number, const FormatArguments& fmargs) {
//	FormatArguments::group_size;
}



TEST("....................") {
	FormatFoo fmargs;

	CHECK(dotter_foo(0, fmargs) == "0");
	CHECK(dotter_foo(0, fmargs) == "0.1");
	CHECK(dotter_foo(0, fmargs) == "0.01");
	CHECK(dotter_foo(0, fmargs) == "0.001");
	CHECK(dotter_foo(0, fmargs) == "0.0001");
	CHECK(dotter_foo(0, fmargs) == "0.00001");

	CHECK(dotter_foo(0.00123456789012345, fmargs) ==    "0.00");
	CHECK(dotter_foo(00.0123456789012345, fmargs) ==    "0.01");
	CHECK(dotter_foo(000.123456789012345, fmargs) ==    "0.12");
	CHECK(dotter_foo(0001.23456789012345, fmargs) ==    "1.23");
	CHECK(dotter_foo(00012.3456789012345, fmargs) ==   "12.34");
	CHECK(dotter_foo(000123.456789012345, fmargs) ==  "123.45");
	CHECK(dotter_foo(0001234.56789012345, fmargs) ==    "1.23 K");
	CHECK(dotter_foo(00012345.6789012345, fmargs) ==   "12.34 K");
	CHECK(dotter_foo(000123456.789012345, fmargs) ==  "123.45 K");
	CHECK(dotter_foo(0001234567.89012345, fmargs) ==    "1.23 M");
	CHECK(dotter_foo(00012345678.9012345, fmargs) ==   "12.34 M");
	CHECK(dotter_foo(000123456789.012345, fmargs) ==  "123.45 M");
	CHECK(dotter_foo(0001234567890.12345, fmargs) ==    "1.23 G");
	CHECK(dotter_foo(00012345678901.2345, fmargs) ==   "12.34 G");
	CHECK(dotter_foo(000123456789012.345, fmargs) ==  "123.45 G");
	CHECK(dotter_foo(0001234567890123.45, fmargs) ==    "1.23 T");
	CHECK(dotter_foo(00012345678901234.5, fmargs) ==   "12.34 T");
	CHECK(dotter_foo(000123456789012345., fmargs) ==  "123.45 T");

	CHECK(foo(0, 3, " ",  2, {"", " K", " M", " G", " T", " P"}) == "0");
	CHECK(foo(0, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "0.00");
	CHECK(foo(1000, 3, " ",  2, {"", " K", " M", " G", " T", " P"}) == "1 000");
	CHECK(foo(1000, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "1 000.00");
	CHECK(foo(1000, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "1 K");
	CHECK(foo(1000, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "1.00 K");
	CHECK(foo(1000, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "1 000 K");
	CHECK(foo(1000, 3, " ", -2, {"", " K", " M", " G", " T", " P"}) == "1 000.00 K");
	CHECK(foo(100, -2, {"", " K", " M", " G", " T", " P"}) == "100");
}

//TEST (quickAddThousandSeparators, basicNumbers) {
//	EXPECT_EQ("", quickAddThousandSeparators(""));
//	EXPECT_EQ("1", quickAddThousandSeparators("1"));
//	EXPECT_EQ("100", quickAddThousandSeparators("100"));
//	EXPECT_EQ("1,000", quickAddThousandSeparators("1000"));
//	EXPECT_EQ("10,000", quickAddThousandSeparators("10000"));
//	EXPECT_EQ("100,000", quickAddThousandSeparators("100000"));
//	EXPECT_EQ("1,000,000", quickAddThousandSeparators("1000000"));
//	EXPECT_EQ("1,000,000,000", quickAddThousandSeparators("1000000000"));
//	EXPECT_EQ("1,012,789,345,456,123,678,456,345,809", quickAddThousandSeparators("1012789345456123678456345809"));
//}
//
//TEST (quickAddThousandSeparators, changeThousandSeparator) {
//	EXPECT_EQ("", quickAddThousandSeparators("",'.'));
//	EXPECT_EQ("1", quickAddThousandSeparators("1",'.'));
//	EXPECT_EQ("100", quickAddThousandSeparators("100", '.'));
//	EXPECT_EQ("1.000", quickAddThousandSeparators("1000", '.'));
//	EXPECT_EQ("1.000.000.000", quickAddThousandSeparators("1000000000", '.'));
//	EXPECT_EQ("1.012.789.345.456.123.678.456.345.809", quickAddThousandSeparators("1012789345456123678456345809", '.'));
//}
//
//TEST (addThousandSeparators, basicNumbers) {
//	EXPECT_EQ("", addThousandSeparators(""));
//	EXPECT_EQ("1", addThousandSeparators("1"));
//	EXPECT_EQ("100", addThousandSeparators("100"));
//	EXPECT_EQ("1,000", addThousandSeparators("1000"));
//	EXPECT_EQ("10,000", addThousandSeparators("10000"));
//	EXPECT_EQ("100,000", addThousandSeparators("100000"));
//	EXPECT_EQ("1,000,000", addThousandSeparators("1000000"));
//	EXPECT_EQ("1,000,000,000", addThousandSeparators("1000000000"));
//	EXPECT_EQ("1,012,789,345,456,123,678,456,345,809", addThousandSeparators("1012789345456123678456345809"));
//}
//
//TEST (addThousandSeparators, negativeBasicNumbers) {
//	EXPECT_EQ("", addThousandSeparators(""));
//	EXPECT_EQ("-1", addThousandSeparators("-1"));
//	EXPECT_EQ("-100", addThousandSeparators("-100"));
//	EXPECT_EQ("-1,000", addThousandSeparators("-1000"));
//	EXPECT_EQ("-10,000", addThousandSeparators("-10000"));
//	EXPECT_EQ("-100,000", addThousandSeparators("-100000"));
//	EXPECT_EQ("-1,000,000", addThousandSeparators("-1000000"));
//	EXPECT_EQ("-1,000,000,000", addThousandSeparators("-1000000000"));
//	EXPECT_EQ("-1,012,789,345,456,123,678,456,345,809", addThousandSeparators("-1012789345456123678456345809"));
//}
//
//TEST (addThousandSeparators, changeThousandSeparator) {
//	EXPECT_EQ("", addThousandSeparators("",'.'));
//	EXPECT_EQ("-1", addThousandSeparators("-1",'.'));
//	EXPECT_EQ("100", addThousandSeparators("100", '.'));
//	EXPECT_EQ("-1.000", addThousandSeparators("-1000", '.'));
//	EXPECT_EQ("-1.000.000.000", addThousandSeparators("-1000000000", '.'));
//	EXPECT_EQ("1.012.789.345.456.123.678.456.345.809", addThousandSeparators("1012789345456123678456345809", '.'));
//}


//template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
//[[nodiscard]] constexpr T parse_number(std::string_view str, std::error_code& ec) {
//	str = libv::trim(str);
//
//	T value;
//
//	// C++20 Support Workaround: This branching could be removed once Clang/GCC implements from_chars to floating point types
//	// Implementation is expected to land with GCC 11.0
//	if constexpr (std::is_integral_v<T>) {
//		std::from_chars_result result = std::from_chars<T>(str.begin(), str.end(), value);
//		ec = std::make_error_code(result.ec);
//
//		if (!ec && result.ptr != str.end())
//			ec = std::make_error_code(std::errc::invalid_argument);
//
//	} else { // This branch will be removed
//		size_t pos = 0;
//
//		try {
//			if constexpr (std::is_same_v<T, double>) {
//				value = std::stof(std::string(str), &pos);
//			} else {
//				value = std::stod(std::string(str), &pos);
//			}
//		} catch (const std::invalid_argument& e) {
//			// no conversion could be performed
//			ec = std::make_error_code(std::errc::invalid_argument);
//		} catch (const std::out_of_range& e) {
//			// the converted value would fall out of the range of the result type or if the underlying function sets errno
//			ec = std::make_error_code(std::errc::result_out_of_range);
//		}
//
//		if (pos != str.size())
//			ec = std::make_error_code(std::errc::invalid_argument);
//	}
//
//	return value;
//}
//
//template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
//[[nodiscard]] constexpr inline auto parse_number_ec(std::string_view str) {
//	struct Result {
//		T value;
//		std::error_code ec;
//	};
//
//	Result result;
//	result.value = parse_number<T>(str, result.ec);
//	return result;
//}
//
//template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
//[[nodiscard]] constexpr inline T parse_number_or(std::string_view str, T fallback) {
//	std::error_code ec;
//	T value = parse_number<T>(str, ec);
//	return ec ? fallback : value;
//}
//
//template <typename T> WISH_REQUIRES(std::is_arithmetic_v<T>)
//[[nodiscard]] constexpr T parse_number_or_throw(std::string_view str) {
//	std::error_code ec;
//	T value = parse_number<T>(str, ec);
//
//	// Only two errc possible: its out of range or its not a number
//	if (!ec)
//		return value;
//	else if (ec == std::make_error_code(std::errc::result_out_of_range))
//		throw std::out_of_range("out of range");
//	else if (ec == std::make_error_code(std::errc::invalid_argument))
//		throw std::invalid_argument("invalid argument");
//	else // unknown error code
//		throw std::system_error(ec);
//}

// -------------------------------------------------------------------------------------------------

} // namespace libv
