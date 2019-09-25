// File:   flag.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/utility/flag_enum.hpp>
// std
#include <cstdint>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using Flag_t = libv::flag_enum<uint32_t, class tag_Flag>;
namespace Flag {

static constexpr Flag_t none = Flag_t{};

//static constexpr Flag_t enable              = libv::bit(0);
static constexpr Flag_t render                = libv::bit(1);
static constexpr Flag_t layout                = libv::bit(2);
//static constexpr Flag_t ______              = libv::bit(3);
static constexpr Flag_t focused               = libv::bit(4);
//static constexpr Flag_t focusLinked         = libv::bit(5);
static constexpr Flag_t focusableChild        = libv::bit(6);
static constexpr Flag_t focusableSelf         = libv::bit(7);

static constexpr Flag_t pendingAttachChild    = libv::bit(8); /// Component has a child that's pending attachment
static constexpr Flag_t pendingAttachSelf     = libv::bit(9); /// Component is pending attachment. Requests \c attach call before any other ui operation
static constexpr Flag_t pendingStyleChild     = libv::bit(10);
static constexpr Flag_t pendingStyleSelf      = libv::bit(11);
static constexpr Flag_t pendingLayoutChild    = libv::bit(12);
static constexpr Flag_t pendingLayoutSelf     = libv::bit(13);
static constexpr Flag_t pendingRenderChild    = libv::bit(14);
static constexpr Flag_t pendingRenderSelf     = libv::bit(15);

static constexpr Flag_t pendingDetachChild    = libv::bit(16);
static constexpr Flag_t pendingDetachSelf     = libv::bit(17);
static constexpr Flag_t pendingCreate         = libv::bit(18);
//static constexpr Flag_t awakeSelf             = libv::bit(19);
//static constexpr Flag_t awakeChild            = libv::bit(20);
//static constexpr Flag_t parentsDependOnLayout = libv::bit(21); /// Component dynamic content size change will affect parent's layout (if not set, parent layout does not have to be invalidated on dynamic content size change)
static constexpr Flag_t updatedSize           = libv::bit(22); /// Component's size was changed since it last render
static constexpr Flag_t updatedPosition       = libv::bit(23); /// Component's position was changed since it last render

static constexpr Flag_t watchChar             = libv::bit(24);
static constexpr Flag_t watchKey              = libv::bit(25);
static constexpr Flag_t watchFocus            = libv::bit(26); // Not really a self contained flag, mostly present for consistency
static constexpr Flag_t watchMouseButton      = libv::bit(27);
static constexpr Flag_t watchMouseEnter       = libv::bit(28);
static constexpr Flag_t watchMousePosition    = libv::bit(29);
static constexpr Flag_t watchMouseScroll      = libv::bit(30);
//static constexpr Flag_t ______              = libv::bit(31);

// -------------------------------------------------------------------------------------------------

static constexpr Flag_t focusable        = focusableChild     | focusableSelf;
static constexpr Flag_t pendingAttach    = pendingAttachChild | pendingAttachSelf;
static constexpr Flag_t pendingStyle     = pendingStyleChild  | pendingStyleSelf;
static constexpr Flag_t pendingLayout    = pendingLayoutChild | pendingLayoutSelf;
static constexpr Flag_t pendingRender    = pendingRenderChild | pendingRenderSelf;
static constexpr Flag_t pendingDetach    = pendingDetachChild | pendingDetachSelf;

static constexpr Flag_t mask_propagate =
		focusableChild |
		pendingAttachChild |
		pendingStyleChild |
		pendingLayoutChild |
		pendingRenderChild |
		pendingDetachChild;

static constexpr Flag_t mask_self =
//		enable |
		render |
		layout |

		focused |
		focusableSelf |

		pendingAttachSelf |
		pendingStyleSelf |
		pendingLayoutSelf |
		pendingRenderSelf |
		pendingDetachSelf |

		pendingCreate |

//		parentsDependOnLayout |

		updatedSize |
		updatedPosition |

		watchChar |
		watchKey |
		watchFocus |
		watchMouseButton |
		watchMouseEnter |
		watchMousePosition |
		watchMouseScroll
;

static constexpr Flag_t mask_init =
//		enable |
		render |
		layout |

		pendingAttachSelf |
		pendingStyleSelf |
		pendingLayoutSelf |
		pendingRenderSelf |

		pendingCreate |

		updatedSize |
		updatedPosition;

static constexpr Flag_t mask_watchMouse =
		watchMouseButton |
		watchMouseEnter |
		watchMousePosition |
		watchMouseScroll;

} // namespace flag

constexpr inline Flag_t calculatePropagateFlags(Flag_t flags) noexcept {
	const auto mask_propagatableSelf = Flag_t{Flag::mask_propagate.value() << 1};
	const auto propagatableSelf = flags & mask_propagatableSelf;
	const auto propagatableChild = flags & Flag::mask_propagate;

	return Flag_t{propagatableSelf.value() >> 1} | propagatableChild;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
