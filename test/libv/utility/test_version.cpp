// Project: libv.utility, File: test/libv/utility/test_version.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/version.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test version number", "[libv.utility.version]") {
	libv::version_number v0;

	CHECK(v0.major == 0);
	CHECK(v0.minor == 0);
	CHECK(v0.patch == 0);
	CHECK(v0.build == 0);
}

TEST_CASE("Test version number ostream", "[libv.utility.version]") {
	libv::version_number v0{1, 2};
	libv::version_number v1{1, 2, 3};
	libv::version_number v2{1, 2, 3, 4};

	std::stringstream ss;

	ss << v0; CHECK(ss.str() == "1.2.0.0"); ss.str("");
	ss << v1; CHECK(ss.str() == "1.2.3.0"); ss.str("");
	ss << v2; CHECK(ss.str() == "1.2.3.4"); ss.str("");
}

TEST_CASE("Test version number fmt", "[libv.utility.version]") {
	libv::version_number v0{1, 2};
	libv::version_number v1{1, 2, 3};
	libv::version_number v2{1, 2, 3, 4};

//	CHECK(fmt::format("{}", v0) == "1.2.0.0");
//	CHECK(fmt::format("{}", v1) == "1.2.3.0");
	CHECK(fmt::format("{}", v0) == "1.2");
	CHECK(fmt::format("{}", v1) == "1.2.3");
	CHECK(fmt::format("{}", v2) == "1.2.3.4");

//	CHECK(fmt::format("{:%M}", v2) == "1");
//	CHECK(fmt::format("{:%M}", v2) == "1");
//	CHECK(fmt::format("{:%M.%m}", v2) == "1.2");
//	CHECK(fmt::format("{:%M-%m}", v2) == "1-2");
//	CHECK(fmt::format("{:%M.%m.%p}", v2) == "1.2.3");
//	CHECK(fmt::format("{:%M-%m-%p}", v2) == "1-2-3");
//	CHECK(fmt::format("{:%M.%m.%p.%b}", v2) == "1.2.3.4");
//	CHECK(fmt::format("{:%M-%m-%p-%b}", v2) == "1-2-3-4");
//
//	CHECK(fmt::format("{:%b.%M.%p.%b - %b.%p.%m.%M}", v2) == "1.2.3.4 - ");
}

TEST_CASE("Test version number relational operators", "[libv.utility.version]") {
	libv::version_number v0;
	libv::version_number v1;

	SECTION("0") {
		v0 = libv::version_number{2, 1, 2, 3};
		v1 = libv::version_number{4, 1, 2, 3};
	}
	SECTION("1") {
		v0 = libv::version_number{1, 2, 2, 3};
		v1 = libv::version_number{1, 4, 2, 3};
	}
	SECTION("2") {
		v0 = libv::version_number{1, 1, 2, 3};
		v1 = libv::version_number{1, 1, 4, 3};
	}
	SECTION("3") {
		v0 = libv::version_number{1, 1, 2, 2};
		v1 = libv::version_number{1, 1, 2, 4};
	}

	CHECK(v0 == v0);
	CHECK(v0 != v1);
	CHECK(v0 < v1);
	CHECK(v0 <= v1);
	CHECK(v1 > v0);
	CHECK(v1 >= v0);
}
