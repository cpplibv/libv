// File: TextureImpl.hpp, Created on 2014. november 28. 17:45, Author: Vader

#pragma once

// vl
#include <vl/resource.hpp>
// std
#include <atomic>
#include <memory>
#include <string>
// pro
#include <vl/gl/shader.hpp> // only for TextureType
#include <vl/gl/types.hpp>

namespace gli {
struct texture;
} //namespace gli

namespace vl {
namespace gl {

class ServiceTexture;

// -------------------------------------------------------------------------------------------------

namespace detail {

class TextureImpl : public vl::Resource, public std::enable_shared_from_this<TextureImpl> {
private:
	std::string filePath;
	ServiceTexture* service;
	std::unique_ptr<gli::texture> texture;
	GLuint textureID = 0;
	GLenum target;
	const size_t priority = 0; //<<< Default priority
private:
	void loadIO();
	void loadGL();
	void unloadIO();
	void unloadGL();

public:
	TextureImpl(ServiceTexture* const service, const std::string& filePath);
	virtual ~TextureImpl();
public:
	void bind(TextureType);
	void unbind(TextureType);
//	inline GLuint getTextureID() const {
//		return textureID;
//	}

protected:
	void load(const std::shared_ptr<TextureImpl>& self);
	void unload(const std::shared_ptr<TextureImpl>& self);
public:
	bool operator<(const TextureImpl&) const;
	friend bool operator<(const std::string&, const TextureImpl&);
	friend bool operator<(const TextureImpl&, const std::string&);
};


} //namespace detail

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl