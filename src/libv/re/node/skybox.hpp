// Project: libv.re, File: src/libv/re/node/skybox.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/node/object.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Skybox : public Object {
public:
	using ptr = Skybox_ptr;
	using cptr = Skybox_cptr;

public:
	explicit Skybox(Texture_ptr skyTexture);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Texture_ptr skyTexture) {
		return ptr::make(std::move(skyTexture));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
