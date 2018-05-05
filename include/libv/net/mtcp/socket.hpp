// File: socket.hpp Author: Vader Created on 2018.11.01. 23:31

#pragma once

// ext
#include <netts/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct Socket {
	netts::ip::tcp::socket socket;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
