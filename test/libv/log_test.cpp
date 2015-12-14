// File: LogTest.cpp, Created on 2014. december 1. 12:44, Author: Vader

#include <catch.hpp>

#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <libv/log.hpp>

bool isLogMatch(const std::string&, int, std::vector<std::string>, const std::string &) {
	//	bool isLogMatch(const std::string& log, int severity, std::vector<std::string> modul, const std::string & msg) {
	//		std::string modulMatch = std::accumulate(modul.begin(), modul.end(), std::string(""), [](const std::string& a, const std::string & b) {
	//			return a + "\\[" + b + "\\]"; });
	//		std::string timeMatch = "\\[([0-9]{2}):([0-9]{2}):([0-9]{2})\\]";
	//		std::string severityMatch = std::string("\\[") + std::to_string(severity) + "\\]";
	//		std::string locationMatch = (severity == VLOG_SEVERITY_TRACE || severity >= VLOG_SEVERITY_NOTIF) ? "\\[[a-zA-Z0-9]+\\.(cpp|c|h|hpp|cxx)@[a-zA-Z0-9]+\\:[0-9]+\\]" : "";
	//		std::string fullMatch = timeMatch + severityMatch + modulMatch + locationMatch + " " + msg;
	//
	//		return std::regex_match(log, std::regex(fullMatch));
	return true;
}

TEST_CASE("basicLog") {
	libv::Log log0("test");
	std::stringstream target;
	log0.output(target);

	VLOG_TRACE(log0, "Hi");

	CHECK(isLogMatch(target.str().c_str(), 1, {
		"test"
	}, "Hi"));
}

TEST_CASE("chainLog") {
	libv::Log log0("gl");
	libv::Log log1("libv");
	std::stringstream target;
	log0.output(log1);
	log1.output(target);

	VLOG_TRACE(log0, "Hi");

	CHECK(isLogMatch(target.str().c_str(), 1, {
		"libv", "gl"
	}, "Hi"));
}

TEST_CASE("chainCtor") {
	libv::Log log0("libv");
	libv::Log log1("gl", log0);
	std::stringstream target;
	log0.output(target);

	VLOG_TRACE(log1, "Hi");

	CHECK(isLogMatch(target.str().c_str(), 1, {
		"libv", "gl"
	}, "Hi"));
}

TEST_CASE("multipleLog") {
	libv::Log log0("libv");
	libv::Log log1("gl", log0);
	std::stringstream target;
	log0.output(target);

	VLOG_TRACE(log1, "Hello");
	VLOG_ERROR(log0, "World!");

	std::string line;
	std::getline(target, line);

	CHECK(isLogMatch(line.c_str(), VLOG_SEVERITY_TRACE, {
		"libv", "gl"
	}, "Hello"));
	std::getline(target, line);

	CHECK(isLogMatch(line.c_str(), VLOG_SEVERITY_ERROR, {
		"libv"
	}, "World!"));
}

TEST_CASE("macro") {
	libv::Log log0("libv");
	VLOG_TRACE(log0, "Hello World!");
	VLOG_DEBUG(log0, "Hello World!");
	VLOG_INFO(log0, "Hello World!");
	VLOG_NOTIF(log0, "Hello World!");
	VLOG_WARN(log0, "Hello World!");
	VLOG_ERROR(log0, "Hello World!");
	VLOG_FATAL(log0, "Hello World!");
}

TEST_CASE("coopWithFormatString") {
	libv::Log log0("test");
	{
		std::stringstream target;
		log0.output(target);

		VLOG_INFO(log0, "%s %04d", "Test", 42);

		CHECK(isLogMatch(target.str().c_str(), VLOG_SEVERITY_INFO, {
			"test"
		}, "Test 0042"));
	}
	{
		std::stringstream target;
		log0.output(target);
		std::string strFormat = "%s %04d";
		std::string strArg = "Test";

		VLOG_TRACE(log0, strFormat, strArg, 42);

		CHECK(isLogMatch(target.str().c_str(), VLOG_SEVERITY_TRACE, {
			"test"
		}, "Test 0042"));
	}
}