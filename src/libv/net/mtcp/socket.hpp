// Project: libv.net, File: src/libv/net/mtcp/socket.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/asio/ip/tcp.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct Socket {
	boost::asio::ip::tcp::socket socket;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
