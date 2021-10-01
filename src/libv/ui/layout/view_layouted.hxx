// Project: libv.ui, File: src/libv/ui/layout/view_layouted.lpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <range/v3/view/filter.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

inline auto view_layouted() {
	return ranges::view::filter([](const auto& child) {
		return child.core().isLayouted();
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
