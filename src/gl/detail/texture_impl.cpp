// File: TextureImpl.cpp, Created on 2014. november 28. 17:45, Author: Vader

// hpp
#include <vl/gl/detail/texture_impl.hpp>
// ext
#include <gli/gli.hpp>
// pro
#include <vl/gl/log.hpp>
#include <vl/gl/vgl.hpp>

//http://gli.g-truc.net/0.5.1/code.html

namespace vl {
namespace gl {
namespace detail {

//TextureImpl::TextureImpl(const std::string& name, TextureContext* context) :
//	name(name),
//	context_(context),
//	loaded_(false) {
//
//	context_->io.executeAsync([this] {
//		if (loadIO())
//			context_->gl.executeAsync([this] {
//				if (loadGL())
//					loaded_.exchange(true);
//			});
//	});
//}
//
//TextureImpl::~TextureImpl() { }
//
//bool TextureImpl::loadIO() {
//	VLOG_TRACE(vl::gl::log(), "IO Loading texture: [%s]", name);
//	storage = std::make_unique<gli::storage>(gli::load_dds(name.c_str()));
//
//	if (storage->empty()) {
//		VLOG_WARN(vl::gl::log(), "Failed to load texture {%s}", name);
//		return false;
//	}
//
//	return true;
//}
//
//bool TextureImpl::unloadIO() {
//	VLOG_TRACE(vl::gl::log(), "IO Unloading texture: [%s]", name);
//	storage.reset(nullptr);
//	return true;
//}
//
//bool TextureImpl::loadGL() {
//	VLOG_TRACE(vl::gl::log(), "GL Loading texture: [%s]", name);
//	assert(!storage->empty());
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
//	checkGL();
//	return true;
//}
//
//bool TextureImpl::unloadGL() {
//	VLOG_TRACE(vl::gl::log(), "GL Unloading texture: [%s]", name);
//	checkGL();
//	return true;
//}
//
//void TextureImpl::bind(TextureType type) {
//	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t>(type));
//	GLenum target = storage->layers() > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
//	glBindTexture(target, id);
//	checkGL();
//}
//
//void TextureImpl::unbind(TextureType type) {
//	glActiveTexture(GL_TEXTURE0 + static_cast<uint32_t>(type));
//	GLenum target = storage->layers() > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
//	glBindTexture(target, 0);
//	checkGL();
//}
//
//bool operator<(const std::tuple<std::string, TextureContext*>& args, const TextureImpl& texture) {
//	if (texture.getName() != std::get<0>(args))
//		return std::get<0>(args) < texture.getName();
//	else
//		return std::get<1>(args) < texture.getContext();
//}
//
//bool operator<(const TextureImpl& texture, const std::tuple<std::string, TextureContext*>& args) {
//	if (texture.getName() != std::get<0>(args))
//		return texture.getName() < std::get<0>(args);
//	else
//		return texture.getContext() < std::get<1>(args);
//}
//
//bool operator<(const TextureImpl& lhs, const TextureImpl& rhs) {
//	if (lhs.getName() != rhs.getName())
//		return lhs.getName() < rhs.getName();
//	else
//		return lhs.getContext() < rhs.getContext();
//}

} //namespace detail
} //namespace gl
} //namespace vl