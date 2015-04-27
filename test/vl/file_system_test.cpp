// File: file_system_test.cpp, Created on 2015. április 26. 20:26, Author: Vader

#include <UnitTest++/UnitTest++.h>
#include "vl/file_system.hpp"

//SUITE(FileSystemTests) {

TEST(getDirFull) {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK_EQUAL("D:/X-Files/Cpp/test/", vl::getDir(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/", vl::getDir(filePath1));
}

TEST(getDirDirOnly) {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK_EQUAL("D:/X-Files/Cpp/test/", vl::getDir(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/", vl::getDir(filePath1));
}

TEST(getDirRelativ) {
	auto filePath0 = "../test/main.cpp";

	CHECK_EQUAL("../test/", vl::getDir(filePath0));
}

TEST(getDirFileOnly) {
	auto filePath0 = "main.cpp";

	CHECK_EQUAL("", vl::getDir(filePath0));
}

TEST(getDirSlash) {
	auto filePath0 = "/";

	CHECK_EQUAL("/", vl::getDir(filePath0));
}

TEST(getDirDot) {
	auto filePath0 = ".../";
	auto filePath1 = "../";
	auto filePath2 = "./";

	CHECK_EQUAL(".../", vl::getDir(filePath0));
	CHECK_EQUAL("../", vl::getDir(filePath1));
	CHECK_EQUAL("./", vl::getDir(filePath2));
}

TEST(getDirFolderDotDot) {
	auto filePath0 = "folder/test/../";

	CHECK_EQUAL("folder/test/../", vl::getDir(filePath0));
}

TEST(getDirFolder2DotDot) {
	auto filePath0 = "folder/test/../../";

	CHECK_EQUAL("folder/test/../../", vl::getDir(filePath0));
}

// -------------------------------------------------------------------------------------------------

TEST(getFileFull) {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK_EQUAL("main.cpp", vl::getFile(filePath0));
	CHECK_EQUAL("main.cpp", vl::getFile(filePath1));
}

TEST(getFileDirOnly) {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK_EQUAL("", vl::getFile(filePath0));
	CHECK_EQUAL("", vl::getFile(filePath1));
}

TEST(getFileRelativ) {
	auto filePath0 = "../test/main.cpp";

	CHECK_EQUAL("main.cpp", vl::getFile(filePath0));
}

TEST(getFileFileOnly) {
	auto filePath0 = "main.cpp";

	CHECK_EQUAL("main.cpp", vl::getFile(filePath0));
}

TEST(getFileSlash) {
	auto filePath0 = "/";

	CHECK_EQUAL("", vl::getFile(filePath0));
}

// -------------------------------------------------------------------------------------------------

TEST(removeExtensionFull) {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp";

	CHECK_EQUAL("D:/X-Files/Cpp/test/main", vl::removeExtension(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/main", vl::removeExtension(filePath1));
}

TEST(removeExtensionDirOnly) {
	auto filePath0 = "D:/X-Files/Cpp/test/";
	auto filePath1 = "/X-Files/Cpp/test/";

	CHECK_EQUAL("D:/X-Files/Cpp/test/", vl::removeExtension(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/", vl::removeExtension(filePath1));
}

TEST(removeExtensionRelativ) {
	auto filePath0 = "../test/main.cpp";

	CHECK_EQUAL("../test/main", vl::removeExtension(filePath0));
}

TEST(removeExtensionFileOnly) {
	auto filePath0 = "main.cpp";

	CHECK_EQUAL("main", vl::removeExtension(filePath0));
}

TEST(removeExtensionSlash) {
	auto filePath0 = "/";

	CHECK_EQUAL("/", vl::removeExtension(filePath0));
}

TEST(removeExtensionFullDouble) {
	auto filePath0 = "D:/X-Files/Cpp/test/main.cpp.txt";
	auto filePath1 = "/X-Files/Cpp/test/main.cpp.txt";

	CHECK_EQUAL("D:/X-Files/Cpp/test/main.cpp", vl::removeExtension(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/main.cpp", vl::removeExtension(filePath1));
}

TEST(removeExtensionRelativDouble) {
	auto filePath0 = "../test/main.cpp.txt";

	CHECK_EQUAL("../test/main.cpp", vl::removeExtension(filePath0));
}

TEST(removeExtensionFileOnlyDouble) {
	auto filePath0 = "main.cpp.txt";

	CHECK_EQUAL("main.cpp", vl::removeExtension(filePath0));
}

TEST(removeExtensionFullDot) {
	auto filePath0 = "D:/X-Files/Cpp/test/.gitignore";
	auto filePath1 = "/X-Files/Cpp/test/.gitignore";

	CHECK_EQUAL("D:/X-Files/Cpp/test/.gitignore", vl::removeExtension(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/.gitignore", vl::removeExtension(filePath1));
}

TEST(removeExtensionRelativDot) {
	auto filePath0 = "../test/.gitignore";

	CHECK_EQUAL("../test/.gitignore", vl::removeExtension(filePath0));
}

TEST(removeExtensionFileOnlyDot) {
	auto filePath0 = ".gitignore";

	CHECK_EQUAL(".gitignore", vl::removeExtension(filePath0));
}

TEST(removeExtensionFullDotDot) {
	auto filePath0 = "D:/X-Files/Cpp/test/.gitignore.txt";
	auto filePath1 = "/X-Files/Cpp/test/.gitignore.txt";

	CHECK_EQUAL("D:/X-Files/Cpp/test/.gitignore", vl::removeExtension(filePath0));
	CHECK_EQUAL("/X-Files/Cpp/test/.gitignore", vl::removeExtension(filePath1));
}

TEST(removeExtensionRelativDotDot) {
	auto filePath0 = "../test/.gitignore.txt";

	CHECK_EQUAL("../test/.gitignore", vl::removeExtension(filePath0));
}

TEST(removeExtensionFileOnlyDotDot) {
	auto filePath0 = ".gitignore.txt";

	CHECK_EQUAL(".gitignore", vl::removeExtension(filePath0));
}

TEST(removeExtensionFolderDot0) {
	auto filePath0 = "../test/Makefile";

	CHECK_EQUAL("../test/Makefile", vl::removeExtension(filePath0));
}

TEST(removeExtensionFolderDot1) {
	auto filePath0 = "/te.st/Makefile";

	CHECK_EQUAL("/te.st/Makefile", vl::removeExtension(filePath0));
}

TEST(removeExtensionDotDotDot) {
	auto filePath0 = "...";
	auto filePath1 = "..";
	auto filePath2 = ".";

	CHECK_EQUAL("...", vl::removeExtension(filePath0));
	CHECK_EQUAL("..", vl::removeExtension(filePath1));
	CHECK_EQUAL(".", vl::removeExtension(filePath2));
}

//}