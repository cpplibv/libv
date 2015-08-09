// File: Texture.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include <vl/gl/texture.hpp>
// pro
#include <vl/gl/detail/texture_impl.hpp>
#include <vl/gl/shader.hpp>

namespace vl {
namespace gl {

Texture::Texture(TextureManager& resourceManager, const std::string& filePath) :
	vl::Resource<detail::TextureImpl>(resourceManager, filePath, resourceManager.context) { }

void Texture::bind(TextureType type) {
	if (data().loaded())
		data().bind(type);
}

void Texture::unbind(TextureType type) {
	if (data().loaded())
		data().unbind(type);
}

Texture::~Texture() { }

} //namespace gl
} //namespace vl
