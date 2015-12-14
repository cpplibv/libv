// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// libv
#include <libv/resource.hpp>
// std
#include <memory>
// pro
#include <libv/gl/gl.hpp>
#include <libv/gl/types.hpp>

// -------------------------------------------------------------------------------------------------

namespace gli {
struct texture;
} //namespace gli

namespace boost {
namespace filesystem {
struct path;
} //namespace filesystem
namespace asio {
struct const_buffer;
} //namespace asio
} //namespace boost

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

constexpr const char DEFAULT_TEXTURE_NAME[] = "--UNNAMED--";

// -------------------------------------------------------------------------------------------------

// TODO P4: Switch to const boost::iostreams::array_source& from boost::asio::const_buffer

/**
 * @note Non-virtual class
 */
class Texture {
private:
	GLuint textureID = 0;
	GLenum target;
	std::unique_ptr<gli::texture> texture;
	std::string name;

private:
	void init(const char* data, const size_t size);
	void loadGL();
	void unloadGL();

public:
	Texture(const boost::asio::const_buffer& data, const std::string& name = DEFAULT_TEXTURE_NAME);
	Texture(const boost::filesystem::path& filePath);
	Texture(const boost::filesystem::path& filePath, const std::string& name);
	Texture(const char* data, const size_t size, const std::string& name = DEFAULT_TEXTURE_NAME);

	~Texture();
public:
	void bind(TextureType);
	void unbind(TextureType);
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv