// Project: libv.rev, File: src/libv/rev/resource/texture.cpp

// hpp
#include <libv/rev/resource/texture.hpp>
// pro
#include <libv/rev/resource/texture_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Texture::Texture(libv::intrusive2_ptr<InternalTexture> self) :
	self(std::move(self)) {
}

Texture::Texture(const Texture&) noexcept = default;
Texture& Texture::operator=(const Texture&) & noexcept = default;
Texture::Texture(Texture&&) noexcept = default;
Texture& Texture::operator=(Texture&&) & noexcept = default;

Texture::~Texture() {
	// For the sake of forward declared ptr
}

libv::glr::Texture& Texture::texture() noexcept {
	return self->texture;
}

const libv::glr::Texture& Texture::texture() const noexcept {
	return self->texture;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
