// Project: libv.security, File: sandbox/libv_security_main.cpp

// std
#include <iostream>
// libv
#include <libv/sys/os_version.hpp>


// -------------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::sys::os_version() << std::endl;

	return EXIT_SUCCESS;
}
