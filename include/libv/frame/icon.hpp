// File: Events.hpp, Created on 2013. december 31. 21:52, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <vector>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

struct Icon {
	libv::vec2i size;

	/// The pixel data of this image, arranged left-to-right, top-to-bottom.
	std::vector<libv::vec4uc> pixels;
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
