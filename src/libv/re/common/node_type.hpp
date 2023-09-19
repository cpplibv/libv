// Project: libv.re, File: src/libv/re/common/node_type.hpp

#pragma once

#include <cstdint>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class NodeType : uint8_t {
	light,
	object,
	model,
	proxy,
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
