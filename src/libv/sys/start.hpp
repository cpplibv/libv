// Project: libv.sys, File: src/libv/sys/start.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdlib>
#include <string>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

/// Open links/files with the OS defaults
/// Source: https://stackoverflow.com/a/60225910
///		WIN: start https://corruptedai.com
///		UNIX: xdg-open https://corruptedai.com
///	Note: For links it is important to include either www. or http:// as prefix
/// Security: UNSAFE - Calls system with the passed argument as a non escaped part of it
void start(std::string link) {
#ifdef _WIN32
	std::system(("start " + std::move(link)).c_str());
#else
	std::system(("xdg-open " + std::move(link)).c_str());
#endif
}

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
