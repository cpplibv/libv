// Project: libv.utility, File: test/libv/utility/test_is_parent_folder_of.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/is_parent_folder_of.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("scenario 0", "[libv.fsw.watcher]") {
	CHECK(libv::is_parent_folder_of("/dev/a/b/c/", "/dev/a/b/c/"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c/", "/dev/a/b/c"));

	CHECK(libv::is_parent_folder_of("/dev/a/b/c", "/dev/a/b/c/"));
	CHECK(libv::is_parent_folder_of("/dev/a/b/c", "/dev/a/b/c"));

	CHECK(libv::is_parent_folder_of("/dev/a/b", "/dev/a/b/c"));
	CHECK(!libv::is_parent_folder_of("/dev/a/", "/dev/a/b/c"));
	CHECK(libv::is_parent_folder_of("/dev/a", "/dev/a/b/c"));
	CHECK(!libv::is_parent_folder_of("/dev/", "/dev/a/b/c"));
	CHECK(libv::is_parent_folder_of("/dev", "/dev/a/b/c"));
	CHECK(!libv::is_parent_folder_of("/de", "/dev/a/b/c"));
	CHECK(!libv::is_parent_folder_of("/d", "/dev/a/b/c"));
	CHECK(libv::is_parent_folder_of("/", "/dev/a/b/c"));
	CHECK(libv::is_parent_folder_of("", "/dev/a/b/c"));

	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev/a/b/"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev/a/b"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev/a/"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev/a"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev/"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/dev"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/de"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/d"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", "/"));
	CHECK(!libv::is_parent_folder_of("/dev/a/b/c", ""));
}

// -------------------------------------------------------------------------------------------------
