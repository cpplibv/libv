// Project: libv.rev, File: src/libv/rev/resource/texture_internal.cpp

// hpp
#include <libv/rev/resource/texture_internal.hxx>
// pro
#include <libv/rev/resource/texture_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

void InternalTexture::ref_count_one() noexcept {
	if (auto sp = loader_.lock())
		sp->unload(this);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
