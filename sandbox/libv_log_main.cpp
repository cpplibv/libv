// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <libv/log/log.hpp>
#include <iostream>


#define LIBV_LOG_LOGSANDBOX_TRACE(...) LIBV_LOG_BASE_TRACE("libv.logsandbox", __VA_ARGS__)
#define LIBV_LOG_LOGSANDBOX_DEBUG(...) LIBV_LOG_BASE_DEBUG("libv.logsandbox", __VA_ARGS__)
#define LIBV_LOG_LOGSANDBOX_INFO( ...) LIBV_LOG_BASE_INFO( "libv.logsandbox", __VA_ARGS__)
#define LIBV_LOG_LOGSANDBOX_WARN( ...) LIBV_LOG_BASE_WARN( "libv.logsandbox", __VA_ARGS__)
#define LIBV_LOG_LOGSANDBOX_ERROR(...) LIBV_LOG_BASE_ERROR("libv.logsandbox", __VA_ARGS__)
#define LIBV_LOG_LOGSANDBOX_FATAL(...) LIBV_LOG_BASE_FATAL("libv.logsandbox", __VA_ARGS__)

int main(int, char **) {
	std::cout << libv::log;

	LIBV_LOG_LOGSANDBOX_TRACE("Hello World!");
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {}!", "World");
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{}}!", "World");
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{{}}}!", "World");
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{{{{}!", "World");
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {:20}!", 2.543);
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{{:.2}}}!", 2.543);
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{{:^ 30.6}}}{{{:X}}}!", 12.5454323, 43);
	LIBV_LOG_LOGSANDBOX_TRACE("Hello {{{:^ 30.6}}}{{{:X}}}!", -12.5454323, 43);

	const auto format = "Hello {{{:^ 30.6}}}{{{:X}}}!";
	LIBV_LOG_LOGSANDBOX_TRACE(format, -12.5454323, 43);
	std::string format2 = "Hello {{{:^ 30.6}}}{{{:X}}}!";
	LIBV_LOG_LOGSANDBOX_TRACE(format2, -12.5454323, 43);
	LIBV_LOG_LOGSANDBOX_TRACE("Bad format {", -12.5454323, 43);

	LIBV_LOG_LOGSANDBOX_TRACE("Hello World!");
	LIBV_LOG_LOGSANDBOX_DEBUG("Hello World!");
	LIBV_LOG_LOGSANDBOX_INFO("Hello World!");
	LIBV_LOG_LOGSANDBOX_WARN("Hello World!");
	LIBV_LOG_LOGSANDBOX_ERROR("Hello World!");
	LIBV_LOG_LOGSANDBOX_FATAL("Hello World!");

	return 0;
}