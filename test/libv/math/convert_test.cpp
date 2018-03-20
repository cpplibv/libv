// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch/catch.hpp>

#include <libv/math/fixed_point.hpp>


using namespace libv;

// -------------------------------------------------------------------------------------------------

template <typename T>
T encode_decode(T value) {
	return convert_from_s_24_8<T>(convert_to_s_24_8(value));
}

TEST_CASE("encode decode") {
	for (float i = -56048.4f; i < 50400.6f; i += 462.2f) {
		CHECK(std::fabs(encode_decode(i) - i) < 0.1f);
	}
	for (double i = -36408.4; i < 83043.6; i += 743.2) {
		CHECK(std::fabs(encode_decode(i) - i) < 0.1);
	}

	CHECK(std::fabs(encode_decode(0.f)) < 0.1f);
	CHECK(std::fabs(encode_decode(-0.f)) < 0.1f);
}

