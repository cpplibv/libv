// Project: libv.ui, File: src/libv/ui/style/style_fwd.hpp

#pragma once

#include <cstdint>
#include <libv/utility/memory/intrusive2_ptr.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Style;
void increase_ref_count(Style* ptr);
void decrease_ref_count(Style* ptr);
using Style_ptr = libv::intrusive2_ptr<Style>;
using Style_cptr = libv::intrusive2_ptr<const Style>;
using Style_ref = libv::intrusive2_ref<Style>;
using Style_cref = libv::intrusive2_ref<const Style>;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
