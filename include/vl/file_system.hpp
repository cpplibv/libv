// File: FileSystem.hpp, Created on 2014. december 20. 23:24, Author: Vader

#pragma once

namespace vl {

std::string getDir(const std::string& filePath) {
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

std::string getFile(const std::string& filePath) {
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
	std::string file = getFile(filePath);
	std::string dir = getDir(filePath);
	auto dotIndex = file.find_last_of(".");

	if (dotIndex == std::string::npos || dotIndex == 0 || dotIndex == file.length() - 1) {
		file = (dir != "./" ? dir : "") + file;
	} else {
		file = (dir != "./" ? dir : "") + file.substr(0, dotIndex);
	}
	return file;
}

} //namespace vl