// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Style;
void intrusive_ptr_add_ref(Style*);
void intrusive_ptr_release(Style*);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
