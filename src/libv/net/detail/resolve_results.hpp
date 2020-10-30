// Project: libv.net, File: src/libv/net/detail/resolve_results.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/detail/fwd_resolve_results.hpp>
// ext
#include <boost/asio/ip/tcp.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct ResolveResults {
	boost::asio::ip::tcp::resolver::results_type results;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
