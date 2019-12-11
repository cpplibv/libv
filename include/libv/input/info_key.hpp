// File: inputs.hpp Author: Vader Created on 2017. január 24., 3:11

#pragma once

// libv
#include <libv/utility/span.hpp>
// std
#include <optional>
#include <string_view>
// pro
#include <libv/input/input.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

// TODO P4: Implement key and other enum tables
struct InfoKey {
	Key key;
//	bool sided = false; // Control / Shift / Alt / Super (Left or Right)
	std::string_view id; // Parsable generic name
};

InfoKey table(Key key);
libv::span<InfoKey> table_key();

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
