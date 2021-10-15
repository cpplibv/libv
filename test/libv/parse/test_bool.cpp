// Project: libv.parse, File: test/libv/parse/test_bool.cpp

#include <catch/catch.hpp>

#include <libv/parse/bool.hpp>
#include <libv/utility/approx.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

using libv::parse::parse_bool_or_throw;
using libv::parse::parse_bool_optional;

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("parse bool: test everything", "[libv.parse]") {
	CHECK(false == parse_bool_or_throw("false"));
	CHECK(false == parse_bool_or_throw("False"));
	CHECK(false == parse_bool_or_throw("FALSE"));
	CHECK(false == parse_bool_or_throw("FaLsE"));
	CHECK(false == parse_bool_or_throw("  false"));
	CHECK(false == parse_bool_or_throw("false  "));
	CHECK(false == parse_bool_or_throw(" false "));
	CHECK(false == parse_bool_or_throw("\tfalse \t"));
	CHECK(false == parse_bool_or_throw("\n\tfalse\t"));
	CHECK(false == parse_bool_or_throw("\tfalse\t\n"));

	CHECK(false == parse_bool_or_throw("+0"));
	CHECK(false == parse_bool_or_throw("-0"));
	CHECK(false == parse_bool_or_throw("0"));
	CHECK(false == parse_bool_or_throw(""));
	CHECK(false == parse_bool_or_throw(" "));
	CHECK(false == parse_bool_or_throw("   "));
	CHECK(false == parse_bool_or_throw(" \t"));
	CHECK(false == parse_bool_or_throw("\n"));
	CHECK(false == parse_bool_or_throw("\n "));
	CHECK(false == parse_bool_or_throw(" \n"));
	CHECK(false == parse_bool_or_throw("\t"));
	CHECK(false == parse_bool_or_throw("\n"));

	CHECK(true == parse_bool_or_throw("true"));
	CHECK(true == parse_bool_or_throw("True"));
	CHECK(true == parse_bool_or_throw("TRUE"));
	CHECK(true == parse_bool_or_throw("TrUe"));
	CHECK(true == parse_bool_or_throw("  true"));
	CHECK(true == parse_bool_or_throw("true  "));
	CHECK(true == parse_bool_or_throw(" true "));
	CHECK(true == parse_bool_or_throw("\ttrue \t"));
	CHECK(true == parse_bool_or_throw("\n\ttrue\t"));
	CHECK(true == parse_bool_or_throw("\ttrue\t\n"));

	CHECK(true == parse_bool_or_throw("+1"));
	CHECK(true == parse_bool_or_throw("-1"));
	CHECK(true == parse_bool_or_throw("1"));

	CHECK(not parse_bool_optional("true ."));
	CHECK(not parse_bool_optional("false ."));
	CHECK(not parse_bool_optional("true."));
	CHECK(not parse_bool_optional("false."));

	CHECK(not parse_bool_optional("+"));
	CHECK(not parse_bool_optional("-"));

	CHECK(not parse_bool_optional("0.0"));
	CHECK(not parse_bool_optional("2"));

	CHECK(not parse_bool_optional("fa lse"));
	CHECK(not parse_bool_optional("ffalse"));
	CHECK(not parse_bool_optional("tr ue"));

	CHECK(not parse_bool_optional("á"));
}
