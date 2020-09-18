// Project: libv.net, File: src/libv/net/mtcp/socket.hpp, Author: Császár Mátyás [Vader]

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
