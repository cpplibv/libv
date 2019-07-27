// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/control/parse.hpp>
#include <libv/control/sequence.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

using libv::control::parse_sequence_or_throw;
using libv::control::validation_result;

auto validate(std::string_view str) {
	auto result = libv::control::parse_sequence_or_throw(str);
	return result.validate();
}

} // namespace


// -------------------------------------------------------------------------------------------------

TEST_CASE("Sequence validation", "[libv.control]") {
	CHECK(validate("A") == validation_result::success);

	CHECK(validate("A [hold], A") == validation_result::stateful_input_in_non_last_step);
	CHECK(validate("A, A [hold], A") == validation_result::stateful_input_in_non_last_step);
	CHECK(validate("A, A [hold]") == validation_result::success);

	CHECK(validate("MX, A") == validation_result::analog_input_in_non_last_step);
	CHECK(validate("A, MX, A") == validation_result::analog_input_in_non_last_step);

	CHECK(validate("A[press] + A") == validation_result::redundant_input_in_combination);
	CHECK(validate("Ctrl + LCtrl") == validation_result::redundant_input_in_combination);
	CHECK(validate("MX+ + MX") == validation_result::redundant_input_in_combination);

	CHECK(validate("A[press] + B[press]") == validation_result::multiple_action_in_combination);
	CHECK(validate("Ctrl[release] + B[press]") == validation_result::multiple_action_in_combination);

	CHECK(validate("MX+ + MX-") == validation_result::conflicting_analog_dimension);

//	CHECK(validate("MY + MSY") == validation_result::multiple_different_analog | multiple_pulse_action);
//	CHECK(validate("GA1:X + MSY") == validation_result::multiple_different_analog | multiple_pulse_action);
//
//	CHECK(validate("A [press] + MY") == validation_result::multiple_pulse_action);
}

// -------------------------------------------------------------------------------------------------
