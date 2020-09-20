// Project: libv.ui, File: src/libv/ui/fragment/background.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Background {
public:
	virtual void render(class Renderer& r, libv::vec2f pos, libv::vec2f size) = 0;
	virtual libv::vec2f size() = 0;
	virtual ~Background() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
