// File: TextureImpl.cpp, Created on 2014. november 28. 17:45, Author: Vader

// hpp
#include <vl/gl/detail/texture_impl.hpp>
// ext
#include <GL/glew.h>
#include <gli/gli.hpp>
// pro
#include <vl/gl/gl.hpp>
#include <vl/gl/log.hpp>
#include <vl/gl/services.hpp>

namespace vl {
namespace gl {
namespace detail {

TextureImpl::TextureImpl(ServiceTexture * const service, const std::string& name) :
	filePath(name),
	service(service) { }

TextureImpl::~TextureImpl() { }

// -------------------------------------------------------------------------------------------------

void TextureImpl::bind(TextureType type) {
	if (!textureID)
		return;

	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t> (type));
	glBindTexture(target, textureID);

	checkGL();
}

void TextureImpl::unbind(TextureType type) {
	if (!textureID)
		return;

	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t> (type));
	glBindTexture(target, 0);
	checkGL();
}

// -------------------------------------------------------------------------------------------------

void TextureImpl::loadIO() {
	VLOG_TRACE(vl::gl::log(), "IO Loading texture: [%s]", filePath);
	texture = std::make_unique<gli::texture>(gli::load(filePath.c_str()));

	if (texture->empty()) {
		VLOG_WARN(vl::gl::log(), "Failed to load texture [%s]", filePath);
		texture.reset();
		changeResourceState(ResourceState::FAILED);
		return;
	}

	const auto self = shared_from_this();
	service->threadGL->executeAsync([self] {
		self->loadGL();
	}, priority);
}

void TextureImpl::unloadIO() {
	VLOG_TRACE(vl::gl::log(), "IO Unloading texture: [%s]", filePath);
	texture.reset();
}

void TextureImpl::loadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Loading texture: [%s]", filePath);
	assert(!texture->empty());

	gli::gl GL;
	gli::gl::format const Format = GL.translate(texture->format());
	target = GL.translate(texture->target());
	GLuint tmpTargetID = 0;
	glGenTextures(1, &tmpTargetID);
	glBindTexture(target, tmpTargetID);
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint> (texture->levels() - 1));
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, Format.Swizzle[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, Format.Swizzle[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, Format.Swizzle[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, Format.Swizzle[3]);

	// TODO P5: Min and mag filter data will be separated and handled by a sampler.
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texture->levels() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::tvec3<GLsizei> const Dimensions(texture->dimensions());
	GLsizei const FaceTotal = static_cast<GLsizei> (texture->layers() * texture->faces());
	switch (texture->target()) {
	case gli::TARGET_1D:
		glTexStorage1D(target, static_cast<GLint> (texture->levels()), Format.Internal, Dimensions.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
				target, static_cast<GLint> (texture->levels()), Format.Internal,
				Dimensions.x, texture->target() == gli::TARGET_2D ? Dimensions.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
				target, static_cast<GLint> (texture->levels()), Format.Internal,
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
							Format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage1D(
							target, static_cast<GLint> (Level), 0, Dimensions.x, Format.External, Format.Type,
							texture->data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(texture->format()))
						glCompressedTexSubImage2D(
							target, static_cast<GLint> (Level), 0, 0,
							Dimensions.x, texture->target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
							Format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage2D(
							target, static_cast<GLint> (Level), 0, 0,
							Dimensions.x, texture->target() == gli::TARGET_1D_ARRAY ? LayerGL : Dimensions.y,
							Format.External, Format.Type, texture->data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(texture->format()))
						glCompressedTexSubImage3D(
							target, static_cast<GLint> (Level), 0, 0, 0,
							Dimensions.x, Dimensions.y, texture->target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
							Format.Internal, static_cast<GLsizei> (texture->size(Level)),
							texture->data(Layer, Face, Level));
					else
						glTexSubImage3D(
							target, static_cast<GLint> (Level), 0, 0, 0,
							Dimensions.x, Dimensions.y, texture->target() == gli::TARGET_3D ? Dimensions.z : LayerGL,
							Format.External, Format.Type, texture->data(Layer, Face, Level));
					break;
				default: assert(0);
					break;
				}
			}

	textureID = tmpTargetID;
	changeResourceState(ResourceState::READY);

	texture.reset(); // Unloading IO

	checkGL();
}

void TextureImpl::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading texture: [%s]", filePath);

	const auto temp = textureID;
	textureID = 0;
	changeResourceState(ResourceState::UNREADY);
	glDeleteTextures(1, &temp);
	checkGL();
}

// -------------------------------------------------------------------------------------------------

void TextureImpl::load(const std::shared_ptr<TextureImpl>& self) {
	service->threadIO->executeAsync([self] {
		self->loadIO();
	}, priority);
}

void TextureImpl::unload(const std::shared_ptr<TextureImpl>& self) {
	service->threadGL->executeAsync([self] {
		self->unloadGL();
	}, priority);
}

bool TextureImpl::operator<(const TextureImpl& rhs) const {
	return filePath < rhs.filePath;
}

bool operator<(const std::string& arg, const TextureImpl& texture) {
	return arg < texture.filePath;
}

bool operator<(const TextureImpl& texture, const std::string& arg) {
	return texture.filePath < arg;
}

} //namespace detail
} //namespace gl
} //namespace vl


// For more information about GLI: http://gli.g-truc.net/0.7.0/code.html
//
// Old GLI example load, i will keep this until  a successful test with the new system.
// So next time 1 read this, delete this section...
//
//	GLenum target = storage->layers() > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
//
//	glGenTextures(1, &id);
//	glBindTexture(target, id);
//	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint> (storage->levels() - 1));
//	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, storage->levels() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
//	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	if (storage->layers() > 1) {
//		gli::texture2DArray texture(*storage);
//		glTexStorage3D(target, static_cast<GLint> (texture.levels()),
//				static_cast<GLenum> (gli::internal_format(texture.format())),
//				static_cast<GLsizei> (texture.dimensions().x),
//				static_cast<GLsizei> (texture.dimensions().y),
//				static_cast<GLsizei> (texture.layers()));
//		if (gli::is_compressed(texture.format())) {
//			for (gli::texture2DArray::size_type layer = 0; layer < texture.layers(); ++layer)
//				for (gli::texture2DArray::size_type level = 0; level < texture.levels(); ++level) {
//					glCompressedTexSubImage3D(target,
//							static_cast<GLint> (level),
//							0, 0, static_cast<GLint> (layer),
//							static_cast<GLsizei> (texture[layer][level].dimensions().x),
//							static_cast<GLsizei> (texture[layer][level].dimensions().y),
//							static_cast<GLsizei> (1),
//							static_cast<GLenum> (gli::external_format(texture.format())),
//							static_cast<GLsizei> (texture[layer][level].size()),
//							texture[layer][level].data());
//				}
//		} else {
//			for (gli::texture2DArray::size_type layer = 0; layer < texture.layers(); ++layer)
//				for (gli::texture2DArray::size_type level = 0; level < texture.levels(); ++level) {
//					glTexSubImage3D(target,
//							static_cast<GLint> (level),
//							0, 0, static_cast<GLint> (layer),
//							static_cast<GLsizei> (texture[layer][level].dimensions().x),
//							static_cast<GLsizei> (texture[layer][level].dimensions().y),
//							static_cast<GLsizei> (1),
//							static_cast<GLenum> (gli::external_format(texture.format())),
//							static_cast<GLenum> (gli::type_format(texture.format())),
//							texture[layer][level].data());
//				}
//		}
//	} else {
//		gli::texture2D texture(*storage);
//		glTexStorage2D(target, static_cast<GLint> (texture.levels()),
//				static_cast<GLenum> (gli::internal_format(texture.format())),
//				static_cast<GLsizei> (texture.dimensions().x), static_cast<GLsizei> (texture.dimensions().y));
//		if (gli::is_compressed(texture.format())) {
//			for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
//				glCompressedTexSubImage2D(target,
//						static_cast<GLint> (level), 0, 0,
//						static_cast<GLsizei> (texture[level].dimensions().x),
//						static_cast<GLsizei> (texture[level].dimensions().y),
//						static_cast<GLenum> (gli::external_format(texture.format())),
//						static_cast<GLsizei> (texture[level].size()),
//						texture[level].data());
//			}
//		} else {
//			for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
//				glTexSubImage2D(target,
//						static_cast<GLint> (level), 0, 0,
//						static_cast<GLsizei> (texture[level].dimensions().x),
//						static_cast<GLsizei> (texture[level].dimensions().y),
//						static_cast<GLenum> (gli::external_format(texture.format())),
//						static_cast<GLenum> (gli::type_format(texture.format())),
//						texture[level].data());
//			}
//		}
//	}