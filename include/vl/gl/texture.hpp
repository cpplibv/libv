// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// vl
#include <vl/resource_manager.hpp>
// std
#include <memory>
// pro
#include "vl/gl/detail/texture_impl.hpp"
#include "vl/gl/gl_contexts.hpp"
#include "shader.hpp"

namespace vl {
namespace gl {

class TextureManager : public vl::ResourceManager<detail::TextureImpl> {
public:
	TextureContext* context;
	TextureManager(TextureContext* context) : context(context) { }
};

class Texture : public vl::Resource<detail::TextureImpl> {
public:
	void bind(TextureType);
	void unbind(TextureType);
public:
	Texture(TextureManager& resourceManager, const std::string& filePath);
	virtual ~Texture();
};

} //namespace gl
} //namespace vl