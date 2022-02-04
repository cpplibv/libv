// Project: libv.net, File: src/libv/net/detail/resolve_results.hpp

#pragma once

// fwd
#include <libv/net/detail/resolve_results_fwd.hpp>
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
