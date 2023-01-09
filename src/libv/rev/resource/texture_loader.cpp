// Project: libv.rev, File: src/libv/rev/resource/texture_loader.cpp

// hpp
#include <libv/rev/resource/texture_loader.hpp>
// pro
#include <libv/rev/resource/texture.hpp>
#include <libv/rev/resource/texture_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader(const std::shared_ptr<InternalResourceManager>& irm) :
	self(std::make_shared<InternalTextureLoader>(irm)) {
}

Texture TextureLoader::load(std::string_view name, Swizzle swizzle) {
	return Texture{self->load(name, swizzle)};
}

Texture TextureLoader::fallback() const {
	return Texture{self->fallback()};
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
