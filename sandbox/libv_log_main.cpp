// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// std
#include <iostream>
// pro
#include <libv/log/log.hpp>


inline libv::LoggerModule log_sandbox{libv::logger, "libv.sandbox"};

#define LIBV_LOG_SANDBOX_TRACE(...) log_sandbox.trace(__VA_ARGS__)
#define LIBV_LOG_SANDBOX_DEBUG(...) log_sandbox.debug(__VA_ARGS__)
#define LIBV_LOG_SANDBOX_INFO( ...) log_sandbox.info (__VA_ARGS__)
#define LIBV_LOG_SANDBOX_WARN( ...) log_sandbox.warn (__VA_ARGS__)
#define LIBV_LOG_SANDBOX_ERROR(...) log_sandbox.error(__VA_ARGS__)
#define LIBV_LOG_SANDBOX_FATAL(...) log_sandbox.fatal(__VA_ARGS__)

int main(int, char **) {
	std::cout << libv::logger;

	{
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
	}

	{
		LIBV_LOG_SANDBOX_TRACE("Hello World!");
		LIBV_LOG_SANDBOX_TRACE("Hello {}!", "World");
		LIBV_LOG_SANDBOX_TRACE("Hello {{}}!", "World");
		LIBV_LOG_SANDBOX_TRACE("Hello {{{}}}!", "World");
		LIBV_LOG_SANDBOX_TRACE("Hello {{{{{}!", "World");
		LIBV_LOG_SANDBOX_TRACE("Hello {:20}!", 2.543);
		LIBV_LOG_SANDBOX_TRACE("Hello {{{:.2}}}!", 2.543);
		LIBV_LOG_SANDBOX_TRACE("Hello {{{:^ 30.6}}}{{{:X}}}!", 12.5454323, 43);
		LIBV_LOG_SANDBOX_TRACE("Hello {{{:^ 30.6}}}{{{:X}}}!", -12.5454323, 43);

		const auto format = "Hello {{{:^ 30.6}}}{{{:X}}}!";
		LIBV_LOG_SANDBOX_TRACE(format, -12.5454323, 43);
		std::string format2 = "Hello {{{:^ 30.6}}}{{{:X}}}!";
		LIBV_LOG_SANDBOX_TRACE(format2, -12.5454323, 43);
		LIBV_LOG_SANDBOX_TRACE("Bad format {", -12.5454323, 43);
		LIBV_LOG_SANDBOX_TRACE("Bad format { 2 { text text", -12.5454323, 43);
		LIBV_LOG_SANDBOX_TRACE("Bad format { 2 { text } text", 43);
		LIBV_LOG_SANDBOX_TRACE("Bad format {}");
		LIBV_LOG_SANDBOX_TRACE("Bad format {:.3f}", "x");

		LIBV_LOG_SANDBOX_TRACE("Hello World!");
		LIBV_LOG_SANDBOX_DEBUG("Hello World!");
		LIBV_LOG_SANDBOX_INFO("Hello World!");
		LIBV_LOG_SANDBOX_WARN("Hello World!");
		LIBV_LOG_SANDBOX_ERROR("Hello World!");
		LIBV_LOG_SANDBOX_FATAL("Hello World!");
	}

	return 0;
}
