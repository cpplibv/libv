// Project: libv.net, File: src/libv/net/mtcp/connection_set.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <unordered_set>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct ConnectionSet {
	struct Entry {
		T object;
		libv::net::mtcp::ConnectionCB connection;
	};

	std::unordered_set<Entry> storage;


};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
