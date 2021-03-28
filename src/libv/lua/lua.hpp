// Project: libv.lua, File: src/libv/lua/lua.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/lua/fwd.hpp>
// ext
#include <sol/state.hpp>
#include <sol/types.hpp>
// libv
#include <libv/utility/to_underlying.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

enum class lualib : uint64_t {
	base = 1,
	vec = 2,
//	vm4 = 4,
	ui = 8 | vec,
};

inline lualib operator|(const lualib& lhs, const lualib& rhs) {
	return lualib(libv::to_value(lhs) | libv::to_value(rhs));
}

struct State : sol::state {
	using sol::state::state;
};

State create_state(lualib libmask = lualib::base);

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
