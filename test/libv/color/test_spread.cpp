// Project: libv.color, File: test/libv/color/test_spread.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/approx.hpp>
// pro
#include <libv/color/spread.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test incremental_spread_to_01", "[libv.color.spread]") {
	using libv::color::incremental_spread_to_01;

	CHECK(incremental_spread_to_01<float>( 0) == libv::Approx(0.00000f));
	CHECK(incremental_spread_to_01<float>( 1) == libv::Approx(1.00000f));
	CHECK(incremental_spread_to_01<float>( 2) == libv::Approx(0.50000f));
	CHECK(incremental_spread_to_01<float>( 3) == libv::Approx(0.25000f));
	CHECK(incremental_spread_to_01<float>( 4) == libv::Approx(0.75000f));
	CHECK(incremental_spread_to_01<float>( 5) == libv::Approx(0.12500f));
	CHECK(incremental_spread_to_01<float>( 6) == libv::Approx(0.37500f));
	CHECK(incremental_spread_to_01<float>( 7) == libv::Approx(0.62500f));
	CHECK(incremental_spread_to_01<float>( 8) == libv::Approx(0.87500f));
	CHECK(incremental_spread_to_01<float>( 9) == libv::Approx(0.06250f));
	CHECK(incremental_spread_to_01<float>(10) == libv::Approx(0.18750f));
	CHECK(incremental_spread_to_01<float>(11) == libv::Approx(0.31250f));
	CHECK(incremental_spread_to_01<float>(12) == libv::Approx(0.43750f));
	CHECK(incremental_spread_to_01<float>(13) == libv::Approx(0.56250f));
	CHECK(incremental_spread_to_01<float>(14) == libv::Approx(0.68750f));
	CHECK(incremental_spread_to_01<float>(15) == libv::Approx(0.81250f));
	CHECK(incremental_spread_to_01<float>(16) == libv::Approx(0.93750f));
	CHECK(incremental_spread_to_01<float>(17) == libv::Approx(0.03125f));
	CHECK(incremental_spread_to_01<float>(18) == libv::Approx(0.09375f));
	CHECK(incremental_spread_to_01<float>(19) == libv::Approx(0.15625f));
	CHECK(incremental_spread_to_01<float>(20) == libv::Approx(0.21875f));
	CHECK(incremental_spread_to_01<float>(21) == libv::Approx(0.28125f));
	CHECK(incremental_spread_to_01<float>(22) == libv::Approx(0.34375f));
	CHECK(incremental_spread_to_01<float>(23) == libv::Approx(0.40625f));
	CHECK(incremental_spread_to_01<float>(24) == libv::Approx(0.46875f));
	CHECK(incremental_spread_to_01<float>(25) == libv::Approx(0.53125f));
	CHECK(incremental_spread_to_01<float>(26) == libv::Approx(0.59375f));
	CHECK(incremental_spread_to_01<float>(27) == libv::Approx(0.65625f));
	CHECK(incremental_spread_to_01<float>(28) == libv::Approx(0.71875f));
	CHECK(incremental_spread_to_01<float>(29) == libv::Approx(0.78125f));
	CHECK(incremental_spread_to_01<float>(30) == libv::Approx(0.84375f));
	CHECK(incremental_spread_to_01<float>(31) == libv::Approx(0.90625f));

	CHECK(incremental_spread_to_01<float>( 500) == libv::Approx(0.95117f));
	CHECK(incremental_spread_to_01<float>(1000) == libv::Approx(0.95215f));
	CHECK(incremental_spread_to_01<float>(2000) == libv::Approx(0.95264f));
	CHECK(incremental_spread_to_01<float>(3000) == libv::Approx(0.46460f));

	CHECK(incremental_spread_to_01<double>( 0) == libv::Approx(0.00000));
	CHECK(incremental_spread_to_01<double>( 1) == libv::Approx(1.00000));
	CHECK(incremental_spread_to_01<double>( 2) == libv::Approx(0.50000));
	CHECK(incremental_spread_to_01<double>( 3) == libv::Approx(0.25000));
	CHECK(incremental_spread_to_01<double>( 4) == libv::Approx(0.75000));
	CHECK(incremental_spread_to_01<double>( 5) == libv::Approx(0.12500));
	CHECK(incremental_spread_to_01<double>( 6) == libv::Approx(0.37500));
	CHECK(incremental_spread_to_01<double>( 7) == libv::Approx(0.62500));
	CHECK(incremental_spread_to_01<double>( 8) == libv::Approx(0.87500));
	CHECK(incremental_spread_to_01<double>( 9) == libv::Approx(0.06250));
	CHECK(incremental_spread_to_01<double>(10) == libv::Approx(0.18750));
	CHECK(incremental_spread_to_01<double>(11) == libv::Approx(0.31250));
	CHECK(incremental_spread_to_01<double>(12) == libv::Approx(0.43750));
	CHECK(incremental_spread_to_01<double>(13) == libv::Approx(0.56250));
	CHECK(incremental_spread_to_01<double>(14) == libv::Approx(0.68750));
	CHECK(incremental_spread_to_01<double>(15) == libv::Approx(0.81250));
	CHECK(incremental_spread_to_01<double>(16) == libv::Approx(0.93750));
	CHECK(incremental_spread_to_01<double>(17) == libv::Approx(0.03125));
	CHECK(incremental_spread_to_01<double>(18) == libv::Approx(0.09375));
	CHECK(incremental_spread_to_01<double>(19) == libv::Approx(0.15625));
	CHECK(incremental_spread_to_01<double>(20) == libv::Approx(0.21875));
	CHECK(incremental_spread_to_01<double>(21) == libv::Approx(0.28125));
	CHECK(incremental_spread_to_01<double>(22) == libv::Approx(0.34375));
	CHECK(incremental_spread_to_01<double>(23) == libv::Approx(0.40625));
	CHECK(incremental_spread_to_01<double>(24) == libv::Approx(0.46875));
	CHECK(incremental_spread_to_01<double>(25) == libv::Approx(0.53125));
	CHECK(incremental_spread_to_01<double>(26) == libv::Approx(0.59375));
	CHECK(incremental_spread_to_01<double>(27) == libv::Approx(0.65625));
	CHECK(incremental_spread_to_01<double>(28) == libv::Approx(0.71875));
	CHECK(incremental_spread_to_01<double>(29) == libv::Approx(0.78125));
	CHECK(incremental_spread_to_01<double>(30) == libv::Approx(0.84375));
	CHECK(incremental_spread_to_01<double>(31) == libv::Approx(0.90625));

	CHECK(incremental_spread_to_01<double>( 500) == libv::Approx(0.9511718750));
	CHECK(incremental_spread_to_01<double>(1000) == libv::Approx(0.9521484375));
	CHECK(incremental_spread_to_01<double>(2000) == libv::Approx(0.9526367188));
	CHECK(incremental_spread_to_01<double>(3000) == libv::Approx(0.4645996094));
}

TEST_CASE("Test incremental_spread", "[libv.color.spread]") {
	using libv::color::incremental_spread;

	using vecf = libv::vec3_t<float>;
	using appf = libv::vec3_t<libv::Approx<float, -3>>;
	using vecd = libv::vec3_t<double>;
	using appd = libv::vec3_t<libv::Approx<double, -3>>;

	const auto incf = [](std::size_t r, std::size_t g, std::size_t b) {
		return appf(
				libv::color::incremental_spread_to_01<float>(r),
				libv::color::incremental_spread_to_01<float>(g),
				libv::color::incremental_spread_to_01<float>(b)
		);
	};

	const auto incd = [](std::size_t r, std::size_t g, std::size_t b) {
		return appd(
				libv::color::incremental_spread_to_01<double>(r),
				libv::color::incremental_spread_to_01<double>(g),
				libv::color::incremental_spread_to_01<double>(b)
		);
	};

	CHECK(incremental_spread<vecf>( 0) == incf(0, 0, 0));

	CHECK(incremental_spread<vecf>( 1) == incf(1, 0, 0));
	CHECK(incremental_spread<vecf>( 2) == incf(1, 0, 1));
	CHECK(incremental_spread<vecf>( 3) == incf(1, 1, 0));
	CHECK(incremental_spread<vecf>( 4) == incf(1, 1, 1));
	CHECK(incremental_spread<vecf>( 5) == incf(0, 1, 0));
	CHECK(incremental_spread<vecf>( 6) == incf(0, 1, 1));
	CHECK(incremental_spread<vecf>( 7) == incf(0, 0, 1));

	CHECK(incremental_spread<vecf>( 8) == incf(2, 0, 0));
	CHECK(incremental_spread<vecf>( 9) == incf(2, 0, 1));
	CHECK(incremental_spread<vecf>(10) == incf(2, 0, 2));
	CHECK(incremental_spread<vecf>(11) == incf(2, 1, 0));
	CHECK(incremental_spread<vecf>(12) == incf(2, 1, 1));
	CHECK(incremental_spread<vecf>(13) == incf(2, 1, 2));
	CHECK(incremental_spread<vecf>(14) == incf(2, 2, 0));
	CHECK(incremental_spread<vecf>(15) == incf(2, 2, 1));
	CHECK(incremental_spread<vecf>(16) == incf(2, 2, 2));
	CHECK(incremental_spread<vecf>(17) == incf(0, 2, 0));
	CHECK(incremental_spread<vecf>(18) == incf(0, 2, 1));
	CHECK(incremental_spread<vecf>(19) == incf(0, 2, 2));
	CHECK(incremental_spread<vecf>(20) == incf(1, 2, 0));
	CHECK(incremental_spread<vecf>(21) == incf(1, 2, 1));
	CHECK(incremental_spread<vecf>(22) == incf(1, 2, 2));
	CHECK(incremental_spread<vecf>(23) == incf(0, 0, 2));
	CHECK(incremental_spread<vecf>(24) == incf(0, 1, 2));
	CHECK(incremental_spread<vecf>(25) == incf(1, 0, 2));
	CHECK(incremental_spread<vecf>(26) == incf(1, 1, 2));

	CHECK(incremental_spread<vecf>(27) == incf(3, 0, 0));
	CHECK(incremental_spread<vecf>(28) == incf(3, 0, 1));
	CHECK(incremental_spread<vecf>(29) == incf(3, 0, 2));
	CHECK(incremental_spread<vecf>(30) == incf(3, 0, 3));
	CHECK(incremental_spread<vecf>(31) == incf(3, 1, 0));

	CHECK(incremental_spread<vecf>( 500) == incf(5,  2,  7));
	CHECK(incremental_spread<vecf>(1000) == incf(9,  0,  9));
	CHECK(incremental_spread<vecf>(2000) == incf(7, 12, 12));
	CHECK(incremental_spread<vecf>(3000) == incf(2, 14,  1));

	//

	CHECK(incremental_spread<vecd>( 0) == incd(0, 0, 0));

	CHECK(incremental_spread<vecd>( 1) == incd(1, 0, 0));
	CHECK(incremental_spread<vecd>( 2) == incd(1, 0, 1));
	CHECK(incremental_spread<vecd>( 3) == incd(1, 1, 0));
	CHECK(incremental_spread<vecd>( 4) == incd(1, 1, 1));
	CHECK(incremental_spread<vecd>( 5) == incd(0, 1, 0));
	CHECK(incremental_spread<vecd>( 6) == incd(0, 1, 1));
	CHECK(incremental_spread<vecd>( 7) == incd(0, 0, 1));

	CHECK(incremental_spread<vecd>( 8) == incd(2, 0, 0));
	CHECK(incremental_spread<vecd>( 9) == incd(2, 0, 1));
	CHECK(incremental_spread<vecd>(10) == incd(2, 0, 2));
	CHECK(incremental_spread<vecd>(11) == incd(2, 1, 0));
	CHECK(incremental_spread<vecd>(12) == incd(2, 1, 1));
	CHECK(incremental_spread<vecd>(13) == incd(2, 1, 2));
	CHECK(incremental_spread<vecd>(14) == incd(2, 2, 0));
	CHECK(incremental_spread<vecd>(15) == incd(2, 2, 1));
	CHECK(incremental_spread<vecd>(16) == incd(2, 2, 2));
	CHECK(incremental_spread<vecd>(17) == incd(0, 2, 0));
	CHECK(incremental_spread<vecd>(18) == incd(0, 2, 1));
	CHECK(incremental_spread<vecd>(19) == incd(0, 2, 2));
	CHECK(incremental_spread<vecd>(20) == incd(1, 2, 0));
	CHECK(incremental_spread<vecd>(21) == incd(1, 2, 1));
	CHECK(incremental_spread<vecd>(22) == incd(1, 2, 2));
	CHECK(incremental_spread<vecd>(23) == incd(0, 0, 2));
	CHECK(incremental_spread<vecd>(24) == incd(0, 1, 2));
	CHECK(incremental_spread<vecd>(25) == incd(1, 0, 2));
	CHECK(incremental_spread<vecd>(26) == incd(1, 1, 2));

	CHECK(incremental_spread<vecd>(27) == incd(3, 0, 0));
	CHECK(incremental_spread<vecd>(28) == incd(3, 0, 1));
	CHECK(incremental_spread<vecd>(29) == incd(3, 0, 2));
	CHECK(incremental_spread<vecd>(30) == incd(3, 0, 3));
	CHECK(incremental_spread<vecd>(31) == incd(3, 1, 0));

	CHECK(incremental_spread<vecd>( 500) == incd(5,  2,  7));
	CHECK(incremental_spread<vecd>(1000) == incd(9,  0,  9));
	CHECK(incremental_spread<vecd>(2000) == incd(7, 12, 12));
	CHECK(incremental_spread<vecd>(3000) == incd(2, 14,  1));
}
