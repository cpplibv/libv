// Project: libv, File: app/surface/view/icon_set.hpp

#pragma once

// libv
#include <libv/frame/icon.hpp> // But would be optional, but it is much more reasonable to provide this include
// std
#include <vector>


namespace surface {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::vector<libv::frame::Icon> icon_set_iris_cyan();

// -------------------------------------------------------------------------------------------------

} // namespace surface
