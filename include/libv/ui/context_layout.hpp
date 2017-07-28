// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ContextLayoutPass1 {
};

struct ContextLayoutPass2 {
	libv::vec3f position;
	libv::vec3f size;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
