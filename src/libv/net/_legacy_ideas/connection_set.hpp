// Project: libv.net, File: src/libv/net/_legacy_ideas/connection_set.hpp

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
