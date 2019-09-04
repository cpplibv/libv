// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// std
#include <iostream>
// pro
#include <libv/log/log.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

int main(int, char **) {
	std::cout << libv::logger_stream;

	log_sandbox.trace("Hello World!");
	log_sandbox.trace("Hello {}!", "World");
	log_sandbox.trace("Hello {{}}!", "World");
	log_sandbox.trace("Hello {{{}}}!", "World");
	log_sandbox.trace("Hello {{{{{}!", "World");
	log_sandbox.trace("Hello {:20}!", 2.543);
	log_sandbox.trace("Hello {{{:.2}}}!", 2.543);
	log_sandbox.trace("Hello {{{:^ 30.6}}}{{{:X}}}!", 12.5454323, 43);
	log_sandbox.trace("Hello {{{:^ 30.6}}}{{{:X}}}!", -12.5454323, 43);

	const auto format = "Hello {{{:^ 30.6}}}{{{:X}}}!";
	log_sandbox.trace(format, -12.5454323, 43);
	std::string format2 = "Hello {{{:^ 30.6}}}{{{:X}}}!";
	log_sandbox.trace(format2, -12.5454323, 43);

	log_sandbox.trace("Bad format {", -12.5454323, 43);
	log_sandbox.trace("Bad format { 2 { text text", -12.5454323, 43);
	log_sandbox.trace("Bad format { 2 { text } text", 43);
	log_sandbox.trace("Bad format {}");
	log_sandbox.trace("Bad format {:.3f}", "x");

	log_sandbox.trace("Hello World!");
	log_sandbox.debug("Hello World!");
	log_sandbox.info("Hello World!");
	log_sandbox.warn("Hello World!");
	log_sandbox.error("Hello World!");
	log_sandbox.fatal("Hello World!");

	return 0;
}
