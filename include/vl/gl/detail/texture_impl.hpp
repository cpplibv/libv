// File: TextureImpl.hpp, Created on 2014. november 28. 17:45, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// std
#include <atomic>
#include <memory>
#include <string>
// pro
#include <vl/gl/shader.hpp>


namespace gli {
struct storage;
} //namespace gli

namespace vl {
namespace gl {
struct TextureContext;

namespace detail {

class TextureImpl {
private:
	std::string name;
	TextureContext* context_;
	std::atomic<bool> loaded_;
private:
	std::unique_ptr<gli::storage> storage;
	GLuint id;
public:
	std::string getName() const {
		return name;
	}
	TextureContext* getContext() const {
		return context_;
	}
	bool loaded() const {
		return loaded_.load();
	}
	void bind(TextureType);
	void unbind(TextureType);
private:
	bool loadIO();
	bool loadGL();
	bool unloadIO();
	bool unloadGL();

public:
	TextureImpl(const std::string& name, TextureContext* context);
	virtual ~TextureImpl();
};

bool operator<(const std::tuple<std::string, TextureContext*>&, const TextureImpl&);
bool operator<(const TextureImpl&, const std::tuple<std::string, TextureContext*>&);
bool operator<(const TextureImpl&, const TextureImpl&);

} //namespace detail
} //namespace gl
} //namespace vl