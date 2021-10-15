// Project: libv.net, File: src/libv/net/detail/socket.hpp

#pragma once

// ext
#include <boost/asio/ip/tcp.hpp>


namespace libv {
namespace net {
namespace mtcp {
namespace detail {

// -------------------------------------------------------------------------------------------------

struct Socket {
	boost::asio::ip::tcp::socket socket;
};

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace mtcp
} // namespace net
} // namespace libv
