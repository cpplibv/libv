// Project: libv.sys, File: src/libv/sys/start.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
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
void start_unsafe(std::string link);

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
