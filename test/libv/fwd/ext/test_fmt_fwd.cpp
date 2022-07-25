// Project: libv.utility, File: test/libv/utility/test_entity_ptr.cpp


// hpp
#include <catch2/catch_test_macros.hpp>

// -------------------------------------------------------------------------------------------------

// This test Verifies if the libv/fwd/ext/fmt_fwd.hpp has the correct inline namespace version or not

#include <libv/fwd/ext/fmt_fwd.hpp>

template <typename T>
struct TesterStruct { T x; };

template <typename T>
struct fmt::formatter<TesterStruct<T>, char, void> : fmt::formatter<T, char, void> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return formatter<T, char, void>::parse(ctx);
	}

	template <typename FormatContext>
	auto format(const TesterStruct<T>& var, FormatContext& ctx) const {
		return formatter<T, char, void>::format(var.x, ctx);
	}
};

#include <fmt/core.h>
#include <fmt/format.h>

TEST_CASE("test fmt forward formatter", "[libv.fwd.fmt]") {
	static_assert(fmt::is_formattable<TesterStruct<int>>::value);

	CHECK(fmt::is_formattable<TesterStruct<int>>::value);
}
