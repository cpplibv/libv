// File: Texture.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include <vl/gl/texture.hpp>
// pro
#include <vl/gl/detail/texture_impl.hpp>
#include <vl/gl/services.hpp>

namespace vl {
namespace gl {

Texture::Texture(ServiceTexture * const service, const std::string& filePath) :
	impl(service->cacheTextureImpl.get<vl::use < 1 >> (service, filePath)) {
//	addDependency(impl);
//	setDependencyCallback([this] {
//		textureID = impl->getTextureID();
//	});
}

void Texture::bind(TextureType type) {
	impl->bind(type);
}

void Texture::unbind(TextureType type) {
	impl->unbind(type);
}

Texture::~Texture() { }

} //namespace gl
} //namespace vl