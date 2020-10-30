// Project: libv.net, File: src/libv/net/mtcp/message.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/storage_size.hpp>
// std
#include <string>
//#include <vector>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

//using Message = std::vector<std::byte>; // TODO P2: Switch to std::vector<std::byte>
using Message = std::string;
using PacketHeader = uint32_t;

constexpr auto MTCP_MESSAGE_MAX_RESERVE = libv::MB(1);
constexpr auto MTCP_MESSAGE_MAX_SIZE = std::numeric_limits<PacketHeader>::max();

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
