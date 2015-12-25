// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <libv/log/log.hpp>
#include <iostream>

#define LIBV_INNER_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.sandbox.inner", __VA_ARGS__)

int main(int, char **) {
//	auto filtered_log = libv::log.filter();
//	filtered_log.allow().modul("libv.gl").above(libv::Info).format("[{time}][{modul}]: {message} {poc}");
//	filtered_log.deny().equal(libv::debug);
//	filtered_log.allow().format("[{modul}]: {message}");
//	std::cout << filtered_log;
//
//	libv::log(libv::debug, "libv.sandbox", "Hello World!", LIBV_POC);
//
//	filtered_log << libv::inner::log;
//	libv::inner::log(libv::Debug, "libv.sandbox.inner", "Hello World!", LIBV_POC);
//	LIBV_INNER_DEBUG("Hello World!");

	return 0;
}