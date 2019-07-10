// File: size.hpp Author: Vader Created on 2017. augusztus 17., 0:04

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using Anchor = libv::vec3f;

// TODO P4: Anchor enum, at least for anchor content (for grid layout), flow layout should remain float anchor
static constexpr auto ANCHOR_TOP_LEFT      = libv::ui::Anchor{0.0f, 1.0f, 0};
static constexpr auto ANCHOR_TOP_CENTER    = libv::ui::Anchor{0.5f, 1.0f, 0};
static constexpr auto ANCHOR_TOP_RIGHT     = libv::ui::Anchor{1.0f, 1.0f, 0};
static constexpr auto ANCHOR_CENTER_LEFT   = libv::ui::Anchor{0.0f, 0.5f, 0};
static constexpr auto ANCHOR_CENTER_CENTER = libv::ui::Anchor{0.5f, 0.5f, 0};
static constexpr auto ANCHOR_CENTER_RIGHT  = libv::ui::Anchor{1.0f, 0.5f, 0};
static constexpr auto ANCHOR_BOTTOM_LEFT   = libv::ui::Anchor{0.0f, 0.0f, 0};
static constexpr auto ANCHOR_BOTTOM_CENTER = libv::ui::Anchor{0.5f, 0.0f, 0};
static constexpr auto ANCHOR_BOTTOM_RIGHT  = libv::ui::Anchor{1.0f, 0.0f, 0};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
