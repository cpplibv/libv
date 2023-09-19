// Project: libv.re, File: src/libv/re/core/canvas.cpp

#include <libv/re/core/canvas.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Canvas* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Canvas* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
