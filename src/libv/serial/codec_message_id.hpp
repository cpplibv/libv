// Project: libv.serial, File: src/libv/serial/codec_message_id.hpp

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

enum class CodecMessageID : uint8_t {};

[[nodiscard]] constexpr inline auto operator+(CodecMessageID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
