// File: file_system_test.cpp, Created on 2015. április 26. 20:26, Author: Vader

#include <catch.hpp>

#include "vl/file_system.hpp"

TEST_CASE("getDirFull") {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK("D:/X-Files/Cpp/test/" == vl::file_path::directory(filePath0));
	CHECK("/X-Files/Cpp/test/" == vl::file_path::directory(filePath1));
}

TEST_CASE("getDirDirOnly") {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK("D:/X-Files/Cpp/test/" == vl::file_path::directory(filePath0));
	CHECK("/X-Files/Cpp/test/" == vl::file_path::directory(filePath1));
}

TEST_CASE("getDirRelativ") {
	auto filePath0 = "../test/main.cpp";

	CHECK("../test/" == vl::file_path::directory(filePath0));
}

TEST_CASE("getDirFileOnly") {
	auto filePath0 = "main.cpp";

	CHECK("" == vl::file_path::directory(filePath0));
}

TEST_CASE("getDirSlash") {
	auto filePath0 = "/";

	CHECK("/" == vl::file_path::directory(filePath0));
}

TEST_CASE("getDirDot") {
	auto filePath0 = ".../";
	auto filePath1 = "../";
	auto filePath2 = "./";

	CHECK(".../" == vl::file_path::directory(filePath0));
	CHECK("../" == vl::file_path::directory(filePath1));
	CHECK("./" == vl::file_path::directory(filePath2));
}

TEST_CASE("getDirFolderDotDot") {
	auto filePath0 = "folder/test/../";

	CHECK("folder/test/../" == vl::file_path::directory(filePath0));
}

TEST_CASE("getDirFolder2DotDot") {
	auto filePath0 = "folder/test/../../";

	CHECK("folder/test/../../" == vl::file_path::directory(filePath0));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("getFileFull") {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK("main.cpp" == vl::file_path::file(filePath0));
	CHECK("main.cpp" == vl::file_path::file(filePath1));
}

TEST_CASE("getFileDirOnly") {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK("" == vl::file_path::file(filePath0));
	CHECK("" == vl::file_path::file(filePath1));
}

TEST_CASE("getFileRelativ") {
	auto filePath0 = "../test/main.cpp";

	CHECK("main.cpp" == vl::file_path::file(filePath0));
}

TEST_CASE("getFileFileOnly") {
	auto filePath0 = "main.cpp";

	CHECK("main.cpp" == vl::file_path::file(filePath0));
}

TEST_CASE("getFileSlash") {
	auto filePath0 = "/";

	CHECK("" == vl::file_path::file(filePath0));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("removeExtensionFull") {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK("D:/X-Files/Cpp/test/main" == vl::file_path::removeExtension(filePath0));
	CHECK("/X-Files/Cpp/test/main" == vl::file_path::removeExtension(filePath1));
}

TEST_CASE("removeExtensionDirOnly") {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK("D:/X-Files/Cpp/test/" == vl::file_path::removeExtension(filePath0));
	CHECK("/X-Files/Cpp/test/" == vl::file_path::removeExtension(filePath1));
}

TEST_CASE("removeExtensionRelativ") {
	auto filePath0 = "../test/main.cpp";

	CHECK("../test/main" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFileOnly") {
	auto filePath0 = "main.cpp";

	CHECK("main" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionSlash") {
	auto filePath0 = "/";

	CHECK("/" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFullDouble") {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp.txt";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp.txt";

	CHECK("D:/X-Files/Cpp/test/main.cpp" == vl::file_path::removeExtension(filePath0));
	CHECK("/X-Files/Cpp/test/main.cpp" == vl::file_path::removeExtension(filePath1));
}

TEST_CASE("removeExtensionRelativDouble") {
	auto filePath0 = "../test/main.cpp.txt";

	CHECK("../test/main.cpp" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFileOnlyDouble") {
	auto filePath0 = "main.cpp.txt";

	CHECK("main.cpp" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFullDot") {
	auto filePath0 = "D:/X-Files/Cpp/test/.gitignore";
	auto filePath1 = "/X-Files/Cpp/test/.gitignore";

	CHECK("D:/X-Files/Cpp/test/.gitignore" == vl::file_path::removeExtension(filePath0));
	CHECK("/X-Files/Cpp/test/.gitignore" == vl::file_path::removeExtension(filePath1));
}

TEST_CASE("removeExtensionRelativDot") {
	auto filePath0 = "../test/.gitignore";

	CHECK("../test/.gitignore" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFileOnlyDot") {
	auto filePath0 = ".gitignore";

	CHECK(".gitignore" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFullDotDot") {
	auto filePath0 = "D:/X-Files/Cpp/test/.gitignore.txt";
	auto filePath1 = "/X-Files/Cpp/test/.gitignore.txt";

	CHECK("D:/X-Files/Cpp/test/.gitignore" == vl::file_path::removeExtension(filePath0));
	CHECK("/X-Files/Cpp/test/.gitignore" == vl::file_path::removeExtension(filePath1));
}

TEST_CASE("removeExtensionRelativDotDot") {
	auto filePath0 = "../test/.gitignore.txt";

	CHECK("../test/.gitignore" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFileOnlyDotDot") {
	auto filePath0 = ".gitignore.txt";

	CHECK(".gitignore" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFolderDot0") {
	auto filePath0 = "../test/Makefile";

	CHECK("../test/Makefile" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionFolderDot1") {
	auto filePath0 = "/te.st/Makefile";

	CHECK("/te.st/Makefile" == vl::file_path::removeExtension(filePath0));
}

TEST_CASE("removeExtensionDotDotDot") {
	auto filePath0 = "...";
	auto filePath1 = "..";
	auto filePath2 = ".";

	CHECK("..." == vl::file_path::removeExtension(filePath0));
	CHECK(".." == vl::file_path::removeExtension(filePath1));
	CHECK("." == vl::file_path::removeExtension(filePath2));
}