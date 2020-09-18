// Project: libv.parse, File: test/libv/ui/parse/test_parse_align.cpp, Author: Császár Mátyás [Vader]

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
	CHECK(parse_align_horizontal_or_throw("left") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw("center") == AlignHorizontal::center);
	CHECK(parse_align_horizontal_or_throw("right") == AlignHorizontal::right);
	CHECK(parse_align_horizontal_or_throw("justify") == AlignHorizontal::justify);
	CHECK(parse_align_horizontal_or_throw("justifyall") == AlignHorizontal::justify_all);
	CHECK(parse_align_horizontal_or_throw("justify-all") == AlignHorizontal::justify_all);
	CHECK(parse_align_horizontal_or_throw("justify_all") == AlignHorizontal::justify_all);

	CHECK(parse_align_horizontal_or_throw("west") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw("east") == AlignHorizontal::right);

	CHECK(parse_align_horizontal_or_throw("W") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw("C") == AlignHorizontal::center);
	CHECK(parse_align_horizontal_or_throw("E") == AlignHorizontal::right);
	CHECK(parse_align_horizontal_or_throw("J") == AlignHorizontal::justify);
	CHECK(parse_align_horizontal_or_throw("JA") == AlignHorizontal::justify_all);
	CHECK(parse_align_horizontal_or_throw("w") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw("c") == AlignHorizontal::center);
	CHECK(parse_align_horizontal_or_throw("e") == AlignHorizontal::right);
	CHECK(parse_align_horizontal_or_throw("j") == AlignHorizontal::justify);
	CHECK(parse_align_horizontal_or_throw("ja") == AlignHorizontal::justify_all);

	CHECK(parse_align_horizontal_or_throw(" left") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw(" \t\nleft") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw(" \t\nleft  ") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw("leFT") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw(" leFT") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw(" \t\nlEft") == AlignHorizontal::left);
	CHECK(parse_align_horizontal_or_throw(" \t\nlEft  ") == AlignHorizontal::left);
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
	CHECK(not parse_align_horizontal_optional("á"));
}

TEST_CASE("Parse valid align_vertical", "[libv.ui]") {
	CHECK(parse_align_vertical_or_throw("top") == AlignVertical::top);
	CHECK(parse_align_vertical_or_throw("center") == AlignVertical::center);
	CHECK(parse_align_vertical_or_throw("bottom") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw("justify") == AlignVertical::justify);
	CHECK(parse_align_vertical_or_throw("justifyall") == AlignVertical::justify_all);
	CHECK(parse_align_vertical_or_throw("justify-all") == AlignVertical::justify_all);
	CHECK(parse_align_vertical_or_throw("justify_all") == AlignVertical::justify_all);

	CHECK(parse_align_vertical_or_throw("north") == AlignVertical::top);
	CHECK(parse_align_vertical_or_throw("south") == AlignVertical::bottom);

	CHECK(parse_align_vertical_or_throw("N") == AlignVertical::top);
	CHECK(parse_align_vertical_or_throw("C") == AlignVertical::center);
	CHECK(parse_align_vertical_or_throw("S") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw("J") == AlignVertical::justify);
	CHECK(parse_align_vertical_or_throw("JA") == AlignVertical::justify_all);
	CHECK(parse_align_vertical_or_throw("n") == AlignVertical::top);
	CHECK(parse_align_vertical_or_throw("c") == AlignVertical::center);
	CHECK(parse_align_vertical_or_throw("s") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw("j") == AlignVertical::justify);
	CHECK(parse_align_vertical_or_throw("ja") == AlignVertical::justify_all);

	CHECK(parse_align_vertical_or_throw(" bottom") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nbottom") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nbottom  ") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw("boTTom") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw(" boTTom") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nboTTOm") == AlignVertical::bottom);
	CHECK(parse_align_vertical_or_throw(" \t\nboTTOm  ") == AlignVertical::bottom);
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
	CHECK(not parse_align_vertical_optional("á"));
}
