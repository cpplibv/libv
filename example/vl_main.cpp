// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader


#include <iostream>
//#include <string>
//#include <regex>
//#include <vector>

//#include 

//using namespace vl;

//bool isLogMatch(const std::string& log, int severity, std::vector<std::string> modul, bool needLocation, const std::string& msg) {
//	std::string modulMatch = std::accumulate(modul.begin(), modul.end(), std::string(""), [](const std::string& a, const std::string & b) {
//		return a + "\\[" + b + "\\]"; });
//	std::string timeMatch = "^\\[([0-9]{2}):([0-9]{2}):([0-9]{2})\\]";
//	std::string severityMatch = std::string("\\[") + std::to_string(severity) + "\\]";
//	std::string locationMatch = needLocation ? "\\[[a-zA-Z0-9]+\\.(cpp|c|h|hpp|cxx)@[a-zA-Z0-9]+\\:[0-9]+\\]" : "";
//	std::string fullMatch = timeMatch + severityMatch + modulMatch + locationMatch + " " + msg + "(\n.*)?";
//
//	return std::regex_match(log, std::regex(fullMatch));
//}

int main(int, char **) {

	std::cout << "VL is header only." << std::endl;
	
//	std::cout << isLogMatch("[15:01:39][1][test][LogTest.cpp@RunImpl:34] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[15:01:39][1][test][LogTest.cpp@RunImpl:34] Hi\n", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.cpp@RunImpl:26] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.cpp@RunImpl:26] Hi\n", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.c@RunImpl:26] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.h@RunImpl:26] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.hpp@RunImpl:26] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test][LogTest.hfp@RunImpl:26] Hi", 1,{"test"}, true, "Hi") << std::endl;
//	std::cout << isLogMatch("[13:36:53][1][test] Hi\nfd", 1,{"test"}, false, "Hi") << std::endl;

	return 0;
}