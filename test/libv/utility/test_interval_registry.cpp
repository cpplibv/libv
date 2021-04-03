// Project: libv.utility, File: test/libv/utility/test_interval_registry.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/utility/interval_registry.hpp>


// -------------------------------------------------------------------------------------------------

using Interval = libv::IntervalRegistry::Interval;

TEST_CASE("IntervalRegistry default ctor, clear and empty tests", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	CHECK(ir.empty());
	ir.clear();
	CHECK(ir.empty());

	ir.mark(0, 128);

	CHECK(!ir.empty());
	CHECK(ir.next_marked() == Interval{0, 128});

	ir.clear();
	CHECK(ir.empty());

	ir.unmark(0, 128);
	CHECK(ir.empty());

	ir.unmark(50, 500);
	CHECK(ir.empty());
}

TEST_CASE("IntervalRegistry marks merge", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	SECTION("Perfect merge from front to back") {
		ir.mark(0, 50);
		ir.mark(50, 50);
	}
	SECTION("Perfect merge from back to front") {
		ir.mark(50, 50);
		ir.mark(0, 50);
	}
	SECTION("Perfect merge from middle, back to front") {
		ir.mark(40, 40);
		ir.mark(80, 20);
		ir.mark(0, 40);
	}
	SECTION("Perfect merge from middle, front to back") {
		ir.mark(40, 40);
		ir.mark(0, 40);
		ir.mark(80, 20);
	}
	SECTION("Perfect merge from middle a lot") {
		ir.mark(40, 20);
		ir.mark(10, 20);
		ir.mark(70, 20);
		ir.mark(0, 10);
		ir.mark(30, 10);
		ir.mark(60, 10);
		ir.mark(90, 10);
	}
	SECTION("Perfect merge from front to back 3") {
		ir.mark(0, 40);
		ir.mark(40, 40);
		ir.mark(80, 20);
	}
	SECTION("Perfect merge from back to front 3") {
		ir.mark(80, 20);
		ir.mark(40, 40);
		ir.mark(0, 40);
	}

	SECTION("Redundant mark of same interval") {
		ir.mark(0, 100);
		ir.mark(0, 100);
	}

	SECTION("Overlapping marks: front, center, back") {
		ir.mark(0, 30);
		ir.mark(10, 30);
		ir.mark(20, 80);
	}
	SECTION("Overlapping marks: center, front, back") {
		ir.mark(10, 30);
		ir.mark(0, 30);
		ir.mark(20, 80);
	}
	SECTION("Overlapping marks: center, center, merge-center, front, back") {
		ir.mark(10, 30);
		ir.mark(60, 30);
		ir.mark(30, 40);
		ir.mark(0, 30);
		ir.mark(70, 30);
	}
	SECTION("Overlapping marks: center, center, merge-center, all") {
		ir.mark(10, 30);
		ir.mark(60, 30);
		ir.mark(30, 40);
		ir.mark(0, 100);
	}

	CHECK(ir.interval_count() == 1);
	CHECK(ir.next_marked() == Interval{0, 100});
}

TEST_CASE("IntervalRegistry mark-unmark whole perfectly", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	ir.mark(0, 128);

	SECTION("Unmark in 1") {
		ir.unmark(0, 128);
	}
	SECTION("Unmark front then back") {
		ir.unmark(0, 64);
		CHECK(ir.next_marked() == Interval{64, 64});
		ir.unmark(64, 64);
	}
	SECTION("Unmark back then front") {
		ir.unmark(64, 64);
		CHECK(ir.next_marked() == Interval{0, 64});
		ir.unmark(0, 64);
	}
	SECTION("Unmark middle, front then back") {
		ir.unmark(32, 32);
		CHECK(ir.next_marked() == Interval{0, 32});
		ir.unmark(0, 32);
		CHECK(ir.next_marked() == Interval{64, 64});
		ir.unmark(64, 64);
	}
	SECTION("Unmark middle, back then front") {
		ir.unmark(32, 32);
		CHECK(ir.next_marked() == Interval{0, 32});
		ir.unmark(64, 64);
		CHECK(ir.next_marked() == Interval{0, 32});
		ir.unmark(0, 32);
	}

	CHECK(ir.empty());
}

TEST_CASE("IntervalRegistry mark-unmark whole imperfect", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;
	ir.mark(0, 100);
	CHECK(ir.interval_count() == 1);

	SECTION("Unmark in multiple times") {
		ir.unmark(0, 100);
		CHECK(ir.interval_count() == 0);
		ir.unmark(0, 100);
		CHECK(ir.interval_count() == 0);
		ir.unmark(0, 50);
		CHECK(ir.interval_count() == 0);
		ir.unmark(50, 50);
	}
	SECTION("Unmark with overlap on center, front, back") {
		ir.unmark(20, 20);
		CHECK(ir.interval_count() == 2);
		ir.unmark(5, 20);
		CHECK(ir.interval_count() == 2);
		ir.unmark(35, 20);
		CHECK(ir.interval_count() == 2);

		ir.unmark(0, 5);
		CHECK(ir.interval_count() == 1);
		ir.unmark(55, 45);
	}
	SECTION("Unmark with overlap center, front, back") {
		ir.unmark(20, 20);
		CHECK(ir.interval_count() == 2);
		ir.unmark(0, 25);
		CHECK(ir.interval_count() == 1);
		ir.unmark(35, 65);
	}
	SECTION("Unmark with overlap front, back, center") {
		ir.unmark(0, 25);
		CHECK(ir.interval_count() == 1);
		ir.unmark(35, 65);
		CHECK(ir.interval_count() == 1);
		ir.unmark(20, 20);
	}
	SECTION("Unmark with overlap back, front, center") {
		ir.unmark(35, 65);
		CHECK(ir.interval_count() == 1);
		ir.unmark(0, 25);
		CHECK(ir.interval_count() == 1);
		ir.unmark(20, 20);
	}
	SECTION("Unmark with multiple inner segment") {
		ir.unmark(10, 5);
		CHECK(ir.interval_count() == 2);
		ir.unmark(20, 5);
		CHECK(ir.interval_count() == 3);
		ir.unmark(30, 5);
		CHECK(ir.interval_count() == 4);
		ir.unmark(40, 5);
		CHECK(ir.interval_count() == 5);
		ir.unmark(25, 5);
		CHECK(ir.interval_count() == 4);
		ir.unmark(0, 100);
	}
	SECTION("Unmark with multiple inner segment 2") {
		ir.unmark(10, 5);
		CHECK(ir.interval_count() == 2);
		ir.unmark(20, 5);
		CHECK(ir.interval_count() == 3);
		ir.unmark(30, 5);
		CHECK(ir.interval_count() == 4);
		ir.unmark(40, 5);
		CHECK(ir.interval_count() == 5);
		ir.unmark(25, 5);
		CHECK(ir.interval_count() == 4);
		ir.unmark(0, 30);
		CHECK(ir.interval_count() == 2);
		ir.unmark(30, 30);
		CHECK(ir.interval_count() == 1);
		ir.unmark(60, 40);
	}

	CHECK(ir.empty());
}

TEST_CASE("IntervalRegistry off-by-one tests", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	SECTION("0") {
		ir.mark(0, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.mark(1, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 2});
		ir.mark(2, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 3});
		ir.unmark(2, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 2});
		ir.unmark(1, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.unmark(0, 1);
	}

	SECTION("1") {
		ir.mark(0, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.mark(2, 1);
		CHECK(ir.interval_count() == 2);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.mark(1, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 3});
		ir.unmark(1, 1);
		CHECK(ir.interval_count() == 2);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.unmark(2, 1);
		CHECK(ir.interval_count() == 1);
		CHECK(ir.next_marked() == Interval{0, 1});
		ir.unmark(0, 1);
	}

	CHECK(ir.empty());
}

TEST_CASE("IntervalRegistry empty interval is not stored", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	ir.mark(0, 0);
	CHECK(ir.empty());

	ir.mark(100, 0);
	CHECK(ir.empty());

	ir.mark(0, 100);
	CHECK(ir.interval_count() == 1);
	CHECK(ir.next_marked() == Interval{0, 100});

	ir.mark(0, 0);
	ir.mark(50, 0);
	ir.mark(100, 0);
	CHECK(ir.interval_count() == 1);
	CHECK(ir.next_marked() == Interval{0, 100});
}

TEST_CASE("IntervalRegistry empty interval does not splits", "[libv.utility.interval_registry]") {
	libv::IntervalRegistry ir;

	ir.mark(0, 100);
	CHECK(ir.interval_count() == 1);
	CHECK(ir.next_marked() == Interval{0, 100});

	ir.unmark(0, 0);
	ir.unmark(50, 0);
	ir.unmark(100, 0);

	CHECK(ir.interval_count() == 1);
	CHECK(ir.next_marked() == Interval{0, 100});
}

// -------------------------------------------------------------------------------------------------
