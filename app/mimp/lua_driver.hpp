// Project: libv, File: app/mimp/lua_driver.hpp

#pragma once

// std
#include <string>
// pro
#include <libv/vm4/fwd.hpp>


namespace mimp {

// -------------------------------------------------------------------------------------------------

void execute_script(libv::vm4::Model& model, const std::string& script);

// -------------------------------------------------------------------------------------------------

} // namespace mimp
