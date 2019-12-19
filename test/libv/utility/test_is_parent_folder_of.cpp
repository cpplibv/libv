// File: bitflag.cpp - Created on 2018.04.12. 09:37 - Author: Vader

// hpp
#include <catch/catch.hpp>
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
