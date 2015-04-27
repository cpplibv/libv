// File: LogTest.cpp, Created on 2014. december 1. 12:44, Author: Vader

#include <UnitTest++/UnitTest++.h>

#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "vl/log.hpp"

SUITE(LogTests) {

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

	TEST(basicLog) {
		vl::Log log0("test");
		std::stringstream target;
		log0.output(target);

		VLOG_TRACE(log0, "Hi");

		CHECK(isLogMatch(target.str().c_str(), 1, {
			"test"
		}, "Hi"));
	}

	TEST(chainLog) {
		vl::Log log0("gl");
		vl::Log log1("vl");
		std::stringstream target;
		log0.output(log1);
		log1.output(target);

		VLOG_TRACE(log0, "Hi");

		CHECK(isLogMatch(target.str().c_str(), 1, {
			"vl", "gl"
		}, "Hi"));
	}

	TEST(chainCtor) {
		vl::Log log0("vl");
		vl::Log log1("gl", log0);
		std::stringstream target;
		log0.output(target);

		VLOG_TRACE(log1, "Hi");

		CHECK(isLogMatch(target.str().c_str(), 1, {
			"vl", "gl"
		}, "Hi"));
	}

	TEST(multipleLog) {
		vl::Log log0("vl");
		vl::Log log1("gl", log0);
		std::stringstream target;
		log0.output(target);

		VLOG_TRACE(log1, "Hello");
		VLOG_ERROR(log0, "World!");

		std::string line;
		std::getline(target, line);

		CHECK(isLogMatch(line.c_str(), VLOG_SEVERITY_TRACE, {
			"vl", "gl"
		}, "Hello"));
		std::getline(target, line);

		CHECK(isLogMatch(line.c_str(), VLOG_SEVERITY_ERROR, {
			"vl"
		}, "World!"));
	}

	TEST(macro) {
		vl::Log log0("vl");
		VLOG_TRACE(log0, "Hello World!");
		VLOG_DEBUG(log0, "Hello World!");
		VLOG_INFO(log0, "Hello World!");
		VLOG_NOTIF(log0, "Hello World!");
		VLOG_WARN(log0, "Hello World!");
		VLOG_ERROR(log0, "Hello World!");
		VLOG_FATAL(log0, "Hello World!");
	}

	TEST(vlLog) {
		VLOG_INFO(vl::log(), "Hello World!");
	}

	TEST(coopWithFormatString) {
		vl::Log log0("test");
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
}