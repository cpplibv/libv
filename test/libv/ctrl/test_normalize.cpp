// Project: libv.ctrl, File: test/libv/ctrl/test_normalize.cpp

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/sequence.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

auto normal(std::string_view str) {
	auto result = libv::ctrl::parse_sequence_or_throw(str);
	result.normalize();
	return result;
}

auto parse(std::string_view str) {
	return libv::ctrl::parse_sequence_or_throw(str);
}

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("Sequence normalization", "[libv.control]") {
	// Normalized input is unchanged
	CHECK(normal("A") == parse("A"));
	CHECK(normal("A + B") == parse("A + B"));

	// Reorder inputs
	CHECK(normal("B + A") == parse("A + B"));
	CHECK(normal("A + Ctrl") == parse("Ctrl + A"));

	// Merge duplicates
	CHECK(normal("A + A") == parse("A"));

	// Merge digital input actions
	CHECK(normal("A + A [press]") == parse("A [press]"));
	CHECK(normal("A + A [repeat]") == parse("A [repeat]"));
	CHECK(normal("A + A [release]") == parse("A [release]"));
	CHECK(normal("A + A [any]") == parse("A [any]"));
	CHECK(normal("A + A [hold]") == parse("A [hold]"));
	CHECK(normal("A [any] + A [press]") == parse("A [press]"));
	CHECK(normal("A [any] + A [repeat]") == parse("A [repeat]"));
	CHECK(normal("A [any] + A [release]") == parse("A [release]"));
	CHECK(normal("A [repeat] + A [press]") == parse("A [press]"));

	// Merge analog input actions
	CHECK(normal("MX+ + MX") == parse("MX+"));
	CHECK(normal("MX- + MX") == parse("MX-"));
	CHECK(normal("MY+ + MY") == parse("MY+"));
	CHECK(normal("MY- + MY") == parse("MY-"));
	CHECK(normal("MX- + MY") == parse("MX- + MY"));

	// Choose more specialized alias
	CHECK(normal("Ctrl + LCtrl") == parse("LCtrl"));
	CHECK(normal("GP0BA + GPBA") == parse("GP0BA"));
	CHECK(normal("GP2A1:X+ + GPA1:X+") == parse("GP2A1:X+"));
	CHECK(normal("JS0B0 + JSB0") == parse("JS0B0"));
	CHECK(normal("JS2A1:X+ + JSA1:X+") == parse("JS2A1:X+"));

	CHECK(normal("JS0B0 + JS2B0") == parse("JS0B0 + JS2B0"));
	CHECK(normal("GP0BA + GPBB") == parse("GP0BA + GPBB"));
	CHECK(normal("JS2A1:X+ + JSA1:Y+") == parse("JS2A1:X+ + JSA1:Y+"));

	// Fixing not valid inputs

//	CHECK(normal("A [hold], A") == parse("A"));
//	CHECK(normal("A, A [hold], A") == parse("A"));
//	CHECK(normal("A, A [hold]") == parse("A"));
//
//	CHECK(normal("MX, A") == parse("A"));
//	CHECK(normal("A, MX, A") == parse("A"));
//
//	CHECK(normal("A[press] + A") == parse("A"));
//	CHECK(normal("Ctrl + LCtrl") == parse("A"));
//	CHECK(normal("MX+ + MX") == parse("A"));
//
//	CHECK(normal("A[press] + B[press]") == parse("A"));
//	CHECK(normal("Ctrl[release] + B[press]") == parse("A"));
//
//	CHECK(normal("MX+ + MX-") == parse("A"));
}


// -------------------------------------------------------------------------------------------------
