// Project: libv.re, File: src/libv/re/node/editorGrid.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/node/object.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class EditorGrid : public Object {
public:
	using ptr = EditorGrid_ptr;
	using cptr = EditorGrid_cptr;

public:
	explicit EditorGrid();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
