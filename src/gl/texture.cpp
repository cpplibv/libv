// File: Texture.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include <libv/gl/texture.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/filesystem/path.hpp>
#include <GL/glew.h>
#include <gli/gli.hpp>
// libv
#include <libv/read_file.hpp>
// pro
#include <libv/gl/log.hpp>

namespace libv {
namespace gl {

// Texture -----------------------------------------------------------------------------------------

Texture::Texture(const boost::asio::const_buffer& data, const std::string& name) :
	Texture(boost::asio::buffer_cast<const char*>(data), boost::asio::buffer_size(data), name) { }

Texture::Texture(const boost::filesystem::path& filePath) :
	Texture(filePath, filePath.string()) { }

Texture::Texture(const boost::filesystem::path& filePath, const std::string& name) :
	name(name) {
	const auto data = readFile(filePath, std::ios_base::binary);
	init(data.data(), data.size());
}

Texture::Texture(const char* data, const size_t size, const std::string& name) :
	name(name) {
	init(data, size);
}

Texture::~Texture() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void Texture::init(const char* data, const size_t size) {
	if (!data || size <= sizeof(gli::detail::ddsHeader)) {
		LIBV_GL_WARN("Invalid texture data: [%s]", name);
		texture.reset();
		return;
	}
	texture = std::make_unique<gli::texture>(gli::load(data, size));

	if (texture->empty()) {
		LIBV_GL_WARN("Failed to load texture [%s]", name);
		texture.reset();
		return;
	}

	loadGL();
	texture.reset();
}

// -------------------------------------------------------------------------------------------------

void Texture::bind(TextureType type) {
	if (!textureID)
		return;

	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t> (type));
	glBindTexture(target, textureID);

	checkGL();
}

void Texture::unbind(TextureType type) {
	if (!textureID)
		return;

	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t> (type));
	glBindTexture(target, 0);
	checkGL();
}

// -------------------------------------------------------------------------------------------------

void Texture::loadGL() {
	LIBV_GL_TRACE("GL Loading texture: [%s]", name);
	assert(!texture->empty());

	gli::gl GL;
	const gli::gl::format format = GL.translate(texture->format());
	target = GL.translate(texture->target());
	GLuint tmpTargetID = 0;
	glGenTextures(1, &tmpTargetID);
	glBindTexture(target, tmpTargetID);
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint> (texture->levels() - 1));
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);

	//TODO P5: Min and mag filter data will be separated and handled by the sampler.
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texture->levels() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::tvec3<GLsizei> const Dimensions(texture->dimensions());
	GLsizei const FaceTotal = static_cast<GLsizei> (texture->layers() * texture->faces());
	switch (texture->target()) {
	case gli::TARGET_1D:
		glTexStorage1D(target, static_cast<GLint> (texture->levels()), format.Internal, Dimensions.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
				target, static_cast<GLint> (texture->levels()), format.Internal,
				Dimensions.x, texture->target() == gli::TARGET_2D ? Dimensions.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
				target, static_cast<GLint> (texture->levels()), format.Internal,
				Dimensions.x, Dimensions.y, texture->target() == gli::TARGET_3D ? Dimensions.z : FaceTotal);
		break;
	default: assert(0);
		break;
	}

	for (std::size_t Layer = 0; Layer < texture->layers(); ++Layer)
		for (std::size_t Face = 0; Face < texture->faces(); ++Face)
			for (std::size_t Level = 0; Level < texture->levels(); ++Level) {
				GLsizei const LayerGL = static_cast<GLsizei> (Layer);
				glm::tvec3<GLsizei> Dimensions(texture->dimensions(Level));
				if (gli::is_target_cube(texture->target()))
					target = static_cast<GLenum> (GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face);
				switch (texture->target()) {
				case gli::TARGET_1D:
					if (gli::is_compressed(texture->format()))
						glCompressedTexSubImage1D(
							target, static_cast<GLint> (Level), 0, Dimensions.x,
							format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage1D(
							target, static_cast<GLint> (Level), 0, Dimensions.x, format.External, format.Type,
							texture->data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(texture->format()))
						glCompressedTexSubImage2D(
							target, static_cast<GLint> (Level), 0, 0,
							Dimensions.x, texture->target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
							format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage2D(
							target, static_cast<GLint> (Level), 0, 0,
							Dimensions.x, texture->target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
							format.External, format.Type, texture->data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(texture->format()))
						glCompressedTexSubImage3D(
							target, static_cast<GLint> (Level), 0, 0, 0,
							Dimensions.x, Dimensions.y, texture->target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
							format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage3D(
							target, static_cast<GLint> (Level), 0, 0, 0,
							Dimensions.x, Dimensions.y, texture->target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
							format.External, format.Type, texture->data(Layer, Face, Level));
					break;
				default: assert(0);
					break;
				}
			}

	textureID = tmpTargetID;
	//	changeResourceState(ResourceState::READY);

	texture.reset(); // Unloading IO

	checkGL();
}

void Texture::unloadGL() {
	LIBV_GL_TRACE("GL Unloading texture: [%s]", name);

	const auto temp = textureID;
	textureID = 0;
	//	changeResourceState(ResourceState::UNREADY);
	glDeleteTextures(1, &temp);
	checkGL();
}

} //namespace gl
} //namespace libv