// Project: libv.net, File: src/libv/net/mtcp/socket.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <netts/internet.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct ResolveResults {
	netts::ip::tcp::resolver::results_type results;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
