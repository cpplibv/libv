// Project: libv, File: app/space/view/render/model.hpp

// hpp
#include <libv/rev/resource/texture_loader.hpp>
#include <libv/rev/resource/texture_loader_internal.hxx>
// pro
#include <libv/rev/resource/texture.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

TextureLoader::TextureLoader(const libv::intrusive2_ptr<InternalResourceManager>& irm) :
	self(libv::make_intrusive2_ptr<InternalTextureLoader>(irm)) {
}

TextureLoader::TextureLoader(const TextureLoader&) noexcept = default;
TextureLoader& TextureLoader::operator=(const TextureLoader&) & noexcept = default;
TextureLoader::TextureLoader(TextureLoader&&) noexcept = default;
TextureLoader& TextureLoader::operator=(TextureLoader&&) & noexcept = default;

TextureLoader::~TextureLoader() {
	// For the sake of forward declared ptr
}

Texture TextureLoader::load(std::string_view path) {
	return Texture{self->lookup(path)};
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
