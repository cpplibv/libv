// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/parse/parse_align.hpp>
#include <libv/ui/property/align.hpp>


// -------------------------------------------------------------------------------------------------

using libv::ui::AlignHorizontal;
using libv::ui::AlignVertical;
using libv::ui::parse_align_horizontal_optional;
using libv::ui::parse_align_horizontal_or_throw;
using libv::ui::parse_align_vertical_optional;
using libv::ui::parse_align_vertical_or_throw;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Parse valid align_horizontal", "[libv.ui]") {
	CHECK(parse_align_horizontal_or_throw("left") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw("center") == AlignHorizontal::Center);
	CHECK(parse_align_horizontal_or_throw("right") == AlignHorizontal::Right);
	CHECK(parse_align_horizontal_or_throw("justify") == AlignHorizontal::Justify);
	CHECK(parse_align_horizontal_or_throw("justifyall") == AlignHorizontal::JustifyAll);
	CHECK(parse_align_horizontal_or_throw("justify-all") == AlignHorizontal::JustifyAll);
	CHECK(parse_align_horizontal_or_throw("justify_all") == AlignHorizontal::JustifyAll);

	CHECK(parse_align_horizontal_or_throw("west") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw("east") == AlignHorizontal::Right);

	CHECK(parse_align_horizontal_or_throw("W") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw("C") == AlignHorizontal::Center);
	CHECK(parse_align_horizontal_or_throw("E") == AlignHorizontal::Right);
	CHECK(parse_align_horizontal_or_throw("J") == AlignHorizontal::Justify);
	CHECK(parse_align_horizontal_or_throw("JA") == AlignHorizontal::JustifyAll);
	CHECK(parse_align_horizontal_or_throw("w") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw("c") == AlignHorizontal::Center);
	CHECK(parse_align_horizontal_or_throw("e") == AlignHorizontal::Right);
	CHECK(parse_align_horizontal_or_throw("j") == AlignHorizontal::Justify);
	CHECK(parse_align_horizontal_or_throw("ja") == AlignHorizontal::JustifyAll);

	CHECK(parse_align_horizontal_or_throw(" left") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw(" \t\nleft") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw(" \t\nleft  ") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw("leFT") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw(" leFT") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw(" \t\nlEft") == AlignHorizontal::Left);
	CHECK(parse_align_horizontal_or_throw(" \t\nlEft  ") == AlignHorizontal::Left);
}

TEST_CASE("Parse invalid align_horizontal", "[libv.ui]") {
	CHECK(not parse_align_horizontal_optional(""));
	CHECK(not parse_align_horizontal_optional(" "));
	CHECK(not parse_align_horizontal_optional(" -"));
	CHECK(not parse_align_horizontal_optional("not"));
	CHECK(not parse_align_horizontal_optional("top"));
	CHECK(not parse_align_horizontal_optional("bottom"));
	CHECK(not parse_align_horizontal_optional("n"));
	CHECK(not parse_align_horizontal_optional("N"));
	CHECK(not parse_align_horizontal_optional("s"));
	CHECK(not parse_align_horizontal_optional("S"));
}

TEST_CASE("Parse valid align_vertical", "[libv.ui]") {
	CHECK(parse_align_vertical_or_throw("top") == AlignVertical::Top);
	CHECK(parse_align_vertical_or_throw("center") == AlignVertical::Center);
	CHECK(parse_align_vertical_or_throw("bottom") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw("justify") == AlignVertical::Justify);
	CHECK(parse_align_vertical_or_throw("justifyall") == AlignVertical::JustifyAll);
	CHECK(parse_align_vertical_or_throw("justify-all") == AlignVertical::JustifyAll);
	CHECK(parse_align_vertical_or_throw("justify_all") == AlignVertical::JustifyAll);

	CHECK(parse_align_vertical_or_throw("north") == AlignVertical::Top);
	CHECK(parse_align_vertical_or_throw("south") == AlignVertical::Bottom);

	CHECK(parse_align_vertical_or_throw("N") == AlignVertical::Top);
	CHECK(parse_align_vertical_or_throw("C") == AlignVertical::Center);
	CHECK(parse_align_vertical_or_throw("S") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw("J") == AlignVertical::Justify);
	CHECK(parse_align_vertical_or_throw("JA") == AlignVertical::JustifyAll);
	CHECK(parse_align_vertical_or_throw("n") == AlignVertical::Top);
	CHECK(parse_align_vertical_or_throw("c") == AlignVertical::Center);
	CHECK(parse_align_vertical_or_throw("s") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw("j") == AlignVertical::Justify);
	CHECK(parse_align_vertical_or_throw("ja") == AlignVertical::JustifyAll);

	CHECK(parse_align_vertical_or_throw(" bottom") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nbottom") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nbottom  ") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw("boTTom") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw(" boTTom") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nboTTOm") == AlignVertical::Bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nboTTOm  ") == AlignVertical::Bottom);
}

TEST_CASE("Parse invalid align_vertical", "[libv.ui]") {
	CHECK(not parse_align_vertical_optional(""));
	CHECK(not parse_align_vertical_optional(" "));
	CHECK(not parse_align_vertical_optional(" -"));
	CHECK(not parse_align_vertical_optional("not"));
	CHECK(not parse_align_vertical_optional("left"));
	CHECK(not parse_align_vertical_optional("right"));
	CHECK(not parse_align_vertical_optional("w"));
	CHECK(not parse_align_vertical_optional("W"));
	CHECK(not parse_align_vertical_optional("e"));
	CHECK(not parse_align_vertical_optional("E"));
}
