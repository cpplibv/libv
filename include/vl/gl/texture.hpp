// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// vl
#include <vl/resource.hpp>
// std
#include <memory>
// pro
#include <vl/gl/shader.hpp> // only for TextureType
#include <vl/gl/types.hpp>

namespace vl {
namespace gl {

// -------------------------------------------------------------------------------------------------

class ServiceTexture;
namespace detail {
class TextureImpl;
} //namespace detail

class Texture : public vl::Resource {
private:
	//TODO P2:	GLuint textureID;
	std::shared_ptr<detail::TextureImpl> impl;
public:
	void bind(TextureType);
	void unbind(TextureType);
public:
	Texture(ServiceTexture * const service, const std::string& filePath);
	virtual ~Texture();
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl