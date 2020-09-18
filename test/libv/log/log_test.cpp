// Project: libv.log, File: test/libv/log/log_test.cpp, Author: Császár Mátyás [Vader]

// test
#include <catch/catch.hpp>
// hpp
#include <libv/log/log.hpp>
// libv
#include <libv/utility/enum.hpp>
// std
#include <sstream>


// -------------------------------------------------------------------------------------------------

bool isLogged(const std::stringstream& stream, const std::string& str) {
	return stream.str().find(str) != std::string::npos;
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Severity order") {
	CHECK(libv::to_value(libv::Logger::Severity::Trace) < libv::to_value(libv::Logger::Severity::Debug));
	CHECK(libv::to_value(libv::Logger::Severity::Debug) < libv::to_value(libv::Logger::Severity::Info));
	CHECK(libv::to_value(libv::Logger::Severity::Info) < libv::to_value(libv::Logger::Severity::Warn));
	CHECK(libv::to_value(libv::Logger::Severity::Warn) < libv::to_value(libv::Logger::Severity::Error));
	CHECK(libv::to_value(libv::Logger::Severity::Error) < libv::to_value(libv::Logger::Severity::Fatal));
}

TEST_CASE("Logger should output to ostream") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello World!");
	CHECK(isLogged(stream, "Hello World!"));
}

TEST_CASE("Logger should format extra arguments") {
	std::stringstream stream;
	libv::Logger log;
	log.color(false);
	stream << log;

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello World! {}", 10);
	CHECK(isLogged(stream, "Hello World! 10"));
}

TEST_CASE("Logger should handle empty allow filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.allow();

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello World!");
	CHECK(isLogged(stream, "Hello World!"));
}

TEST_CASE("Logger should handle empty deny filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny();

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello World!");
	CHECK(stream.str().size() == 0u);
}

TEST_CASE("Logger should handle above severity filter and filter chaining") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.allow_above(libv::Logger::Severity::Trace);
	log.deny();

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello Trace!");
	log.log("test", libv::Logger::Severity::Error, libv::source_location::current(), "Hello Error!");
	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle below severity filter and filter chaining") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny_below(libv::Logger::Severity::Info);

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello Trace!");
	log.log("test", libv::Logger::Severity::Error, libv::source_location::current(), "Hello Error!");
	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle module filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny("test1");

	log.log("test1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1!");
	log.log("test2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2!");
	CHECK(!isLogged(stream, "Hello 1!"));
	CHECK(isLogged(stream, "Hello 2!"));
}

TEST_CASE("Logger should handle module and severity filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny_below("test1", libv::Logger::Severity::Info);

	log.log("test1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1 Trace!");
	log.log("test1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1 Error!");
	log.log("test2", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2 Trace!");
	log.log("test2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2 Error!");
	CHECK(!isLogged(stream, "Hello 1 Trace!"));
	CHECK(isLogged(stream, "Hello 1 Error!"));
	CHECK(isLogged(stream, "Hello 2 Trace!"));
	CHECK(isLogged(stream, "Hello 2 Error!"));
}

TEST_CASE("Logger should handle equal severity filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny(libv::Logger::Severity::Trace);

	log.log("test", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello Trace!");
	log.log("test", libv::Logger::Severity::Error, libv::source_location::current(), "Hello Error!");
	CHECK(!isLogged(stream, "Hello Trace!"));
	CHECK(isLogged(stream, "Hello Error!"));
}

TEST_CASE("Logger should handle module and equal severity filter") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny("test1", libv::Logger::Severity::Trace);
	log.deny("test2", libv::Logger::Severity::Error);

	log.log("test1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1 Trace!");
	log.log("test1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1 Error!");
	log.log("test2", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2 Trace!");
	log.log("test2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2 Error!");
	CHECK(!isLogged(stream, "Hello 1 Trace!"));
	CHECK(isLogged(stream, "Hello 1 Error!"));
	CHECK(isLogged(stream, "Hello 2 Trace!"));
	CHECK(!isLogged(stream, "Hello 2 Error!"));
}

TEST_CASE("Logger should handle different but prefixed modules") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.deny("a", libv::Logger::Severity::Trace);
	log.deny("a.a", libv::Logger::Severity::Trace);

	log.log("aa", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello aa Trace!");
	log.log("aa.aa", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello aa.aa Trace!");
	log.log("a", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello a Trace!");
	log.log("a.a", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello a.a Trace!");
	CHECK(isLogged(stream, "Hello aa Trace!"));
	CHECK(isLogged(stream, "Hello aa.aa Trace!"));
	CHECK(!isLogged(stream, "Hello a Trace!"));
	CHECK(!isLogged(stream, "Hello a.a Trace!"));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Logger integrity test 00") {
	std::stringstream stream;
	libv::Logger log;
	stream << log;

	log.allow("test1.1.1");
	log.deny("test1.1");
	log.allow_above(libv::Logger::Severity::Info);
	log.deny();

	log.log("test1.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.1 Trace!");
	log.log("test1.1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.1 Error!");
	log.log("test1.1.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.1.1 Trace!");
	log.log("test1.1.1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.1.2 Error!");
	log.log("test1.2", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.2 Trace!");
	log.log("test1.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.2 Error!");
	log.log("test2.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2.1 Trace!");
	log.log("test2.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2.1 Error!");
	log.log("test2.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2.2 Trace!");
	log.log("test2.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2.2 Error!");

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
	stream << log;

	log.deny("test2.1");
	log.allow("test2");
	log.deny_below(libv::Logger::Severity::Info);

	log.log("test1.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.1 Trace!");
	log.log("test1.1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.1 Error!");
	log.log("test1.1.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.1.1 Trace!");
	log.log("test1.1.1", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.1.2 Error!");
	log.log("test1.2", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 1.2 Trace!");
	log.log("test1.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 1.2 Error!");
	log.log("test2.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2.1 Trace!");
	log.log("test2.1", libv::Logger::Severity::Trace, libv::source_location::current(), "Hello 2.1 Error!");
	log.log("test2.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2.2 Trace!");
	log.log("test2.2", libv::Logger::Severity::Error, libv::source_location::current(), "Hello 2.2 Error!");

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
