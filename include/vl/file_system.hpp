// File: FileSystem.hpp, Created on 2014. december 20. 23:24, Author: Vader

#pragma once

namespace vl {
namespace file_path {

std::string directory(const std::string& filePath) {
	std::string dir;
	std::string::size_type slashIndex = filePath.find_last_of("/");

	if (slashIndex == std::string::npos) {
		dir = "";
	} else if (slashIndex == 0) {
		dir = "/";
	} else {
		dir = filePath.substr(0, slashIndex) + "/";
	}

	return dir;
}

std::string file(const std::string& filePath) {
	std::string file;
	auto slashIndex = filePath.find_last_of("/");

	if (slashIndex == std::string::npos) {
		file = filePath;
	} else {
		file = filePath.substr(slashIndex + 1);
	}
	return file;
}

std::string removeExtension(const std::string& filePath) {
	std::string file = file_path::file(filePath);
	std::string dir = file_path::directory(filePath);
	auto dotIndex = file.find_last_of(".");

	if (dotIndex == std::string::npos || dotIndex == 0 || dotIndex == file.length() - 1) {
		file = (dir != "./" ? dir : "") + file;
	} else {
		file = (dir != "./" ? dir : "") + file.substr(0, dotIndex);
	}
	return file;
}

} //namespace file_path
} //namespace vl