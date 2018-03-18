// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// test
#include <catch/catch.hpp>
// hpp
#include <libv/log/log.hpp>
// std
#include <sstream>

// -------------------------------------------------------------------------------------------------

bool isLogged(const std::stringstream& stream, const std::string& str) {
	return stream.str().find(str) != std::string::npos;
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Severity order") {
	CHECK(libv::Trace < libv::Debug);
	CHECK(libv::Debug < libv::Info);
	CHECK(libv::Info < libv::Warn);
	CHECK(libv::Warn < libv::Error);
	CHECK(libv::Error < libv::Fatal);
}

TEST_CASE("Logger should output to ostream") {
	std::stringstream stream;
	libv::Logger log;

	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello World!");

	CHECK(isLogged(stream, "Hello World!"));
}

TEST_CASE("Logger should format extra arguments") {
	std::stringstream stream;
	libv::Logger log;

	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello World! {}", 10);

	CHECK(isLogged(stream, "Hello World! 10"));
}

TEST_CASE("Logger should handle empty allow filter") {
	std::stringstream stream;
	libv::Logger log;

	log.allow();
	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello World!");

	CHECK(isLogged(stream, "Hello World!"));
}

TEST_CASE("Logger should handle empty deny filter") {
	std::stringstream stream;
	libv::Logger log;

	log.deny();
	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello World!");

	CHECK(stream.str().size() == 0u);
}

TEST_CASE("Logger should handle above severity filter and filter chaining") {
	std::stringstream stream;
	libv::Logger log;

	log.allow_above(libv::Trace);
	log.deny();
	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello Trace!");
	log(LIBV_POC, libv::Error, "test", "Hello Error!");

	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle below severity filter and filter chaining") {
	std::stringstream stream;
	libv::Logger log;

	log.deny_below(libv::Info);
	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello Trace!");
	log(LIBV_POC, libv::Error, "test", "Hello Error!");

	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle module filter") {
	std::stringstream stream;
	libv::Logger log;

	log.deny("test1");
	stream << log;
	log(LIBV_POC, libv::Trace, "test1", "Hello 1!");
	log(LIBV_POC, libv::Error, "test2", "Hello 2!");

	CHECK(!isLogged(stream, "Hello 1!"));
	CHECK(isLogged(stream, "Hello 2!"));
}

TEST_CASE("Logger should handle module and severity filter") {
	std::stringstream stream;
	libv::Logger log;

	log.deny_below("test1", libv::Info);
	stream << log;
	log(LIBV_POC, libv::Trace, "test1", "Hello 1 Trace!");
	log(LIBV_POC, libv::Error, "test1", "Hello 1 Error!");
	log(LIBV_POC, libv::Trace, "test2", "Hello 2 Trace!");
	log(LIBV_POC, libv::Error, "test2", "Hello 2 Error!");

	CHECK(!isLogged(stream, "Hello 1 Trace!"));
	CHECK(isLogged(stream, "Hello 1 Error!"));
	CHECK(isLogged(stream, "Hello 2 Trace!"));
	CHECK(isLogged(stream, "Hello 2 Error!"));
}

TEST_CASE("Logger should handle equal severity filter") {
	std::stringstream stream;
	libv::Logger log;

	log.deny(libv::Trace);
	stream << log;
	log(LIBV_POC, libv::Trace, "test", "Hello Trace!");
	log(LIBV_POC, libv::Error, "test", "Hello Error!");

	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle module and equal severity filter") {
	std::stringstream stream;
	libv::Logger log;

	log.deny("test1", libv::Trace);
	log.deny("test2", libv::Error);
	stream << log;
	log(LIBV_POC, libv::Trace, "test1", "Hello 1 Trace!");
	log(LIBV_POC, libv::Error, "test1", "Hello 1 Error!");
	log(LIBV_POC, libv::Trace, "test2", "Hello 2 Trace!");
	log(LIBV_POC, libv::Error, "test2", "Hello 2 Error!");

	CHECK(!isLogged(stream, "Hello 1 Trace!"));
	CHECK(isLogged(stream, "Hello 1 Error!"));
	CHECK(isLogged(stream, "Hello 2 Trace!"));
	CHECK(!isLogged(stream, "Hello 2 Error!"));
}

TEST_CASE("Logger should handle different but prefixed modules") {
	std::stringstream stream;
	libv::Logger log;

	log.deny("a", libv::Trace);
	log.deny("a.a", libv::Trace);
	stream << log;
	log(LIBV_POC, libv::Trace, "aa", "Hello aa Trace!");
	log(LIBV_POC, libv::Trace, "aa.aa", "Hello aa.aa Trace!");
	log(LIBV_POC, libv::Trace, "a", "Hello a Trace!");
	log(LIBV_POC, libv::Trace, "a.a", "Hello a.a Trace!");

	CHECK(isLogged(stream, "Hello aa Trace!"));
	CHECK(isLogged(stream, "Hello aa.aa Trace!"));
	CHECK(!isLogged(stream, "Hello a Trace!"));
	CHECK(!isLogged(stream, "Hello a.a Trace!"));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Logger integrity test 00") {

	std::stringstream stream;
	libv::Logger log;

	log.allow("test1.1.1");
	log.deny("test1.1");
	log.allow_above(libv::Info);
	log.deny();
	stream << log;

	log(LIBV_POC, libv::Trace, "test1.1", "Hello 1.1 Trace!");
	log(LIBV_POC, libv::Error, "test1.1", "Hello 1.1 Error!");
	log(LIBV_POC, libv::Trace, "test1.1.1", "Hello 1.1.1 Trace!");
	log(LIBV_POC, libv::Error, "test1.1.1", "Hello 1.1.2 Error!");
	log(LIBV_POC, libv::Trace, "test1.2", "Hello 1.2 Trace!");
	log(LIBV_POC, libv::Error, "test1.2", "Hello 1.2 Error!");
	log(LIBV_POC, libv::Trace, "test2.1", "Hello 2.1 Trace!");
	log(LIBV_POC, libv::Trace, "test2.1", "Hello 2.1 Error!");
	log(LIBV_POC, libv::Error, "test2.2", "Hello 2.2 Trace!");
	log(LIBV_POC, libv::Error, "test2.2", "Hello 2.2 Error!");

	CHECK(!isLogged(stream, "Hello 1.1 Trace!"));
	CHECK(!isLogged(stream, "Hello 1.1 Error!"));
	CHECK(isLogged(stream, "Hello 1.1.1 Trace!"));
	CHECK(isLogged(stream, "Hello 1.1.2 Error!"));
	CHECK(!isLogged(stream, "Hello 1.2 Trace!"));
	CHECK(isLogged(stream, "Hello 1.2 Error!"));
	CHECK(!isLogged(stream, "Hello 2.1 Trace!"));
	CHECK(!isLogged(stream, "Hello 2.1 Error!"));
	CHECK(isLogged(stream, "Hello 2.2 Trace!"));
	CHECK(isLogged(stream, "Hello 2.2 Error!"));
}

TEST_CASE("Logger integrity test 01") {
	std::stringstream stream;
	libv::Logger log;

	log.deny("test2.1");
	log.allow("test2");
	log.deny_below(libv::Info);
	stream << log;

	log(LIBV_POC, libv::Trace, "test1.1", "Hello 1.1 Trace!");
	log(LIBV_POC, libv::Error, "test1.1", "Hello 1.1 Error!");
	log(LIBV_POC, libv::Trace, "test1.1.1", "Hello 1.1.1 Trace!");
	log(LIBV_POC, libv::Error, "test1.1.1", "Hello 1.1.2 Error!");
	log(LIBV_POC, libv::Trace, "test1.2", "Hello 1.2 Trace!");
	log(LIBV_POC, libv::Error, "test1.2", "Hello 1.2 Error!");
	log(LIBV_POC, libv::Trace, "test2.1", "Hello 2.1 Trace!");
	log(LIBV_POC, libv::Trace, "test2.1", "Hello 2.1 Error!");
	log(LIBV_POC, libv::Error, "test2.2", "Hello 2.2 Trace!");
	log(LIBV_POC, libv::Error, "test2.2", "Hello 2.2 Error!");

	CHECK(!isLogged(stream, "Hello 1.1 Trace!"));
	CHECK(isLogged(stream, "Hello 1.1 Error!"));
	CHECK(!isLogged(stream, "Hello 1.1.1 Trace!"));
	CHECK(isLogged(stream, "Hello 1.1.2 Error!"));
	CHECK(!isLogged(stream, "Hello 1.2 Trace!"));
	CHECK(isLogged(stream, "Hello 1.2 Error!"));
	CHECK(!isLogged(stream, "Hello 2.1 Trace!"));
	CHECK(!isLogged(stream, "Hello 2.1 Error!"));
	CHECK(isLogged(stream, "Hello 2.2 Trace!"));
	CHECK(isLogged(stream, "Hello 2.2 Error!"));
}