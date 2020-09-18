// Project: libv.ecs, File: src/libv/ecs/component.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ecs/common.hpp>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

template <typename CRTP, ComponentSystemID TID, bool Optional = false>
struct Component {
	using optional = Component<CRTP, TID, true>;
	using type = CRTP;

	static constexpr ComponentSystemID ID = TID;
	static constexpr bool is_optional_v = Optional;
};

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv
