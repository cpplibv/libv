// File: Texture.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include <libv/gl/texture.hpp>
// ext
#include <GL/glew.h>
#include <gli/gli.hpp>

// -------------------------------------------------------------------------------------------------

namespace {

/// File can be KTX, KMG or DDS files
bool gliCreateTexture(libv::gl::BaseTexture& vglTexture, char const* data, std::size_t size) {
	gli::texture Texture = gli::load(data, size);
	if (Texture.empty())
		return false;

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum target = GL.translate(Texture.target());

	vglTexture.bind(target);
	vglTexture.setBaseLevel(0);
	vglTexture.setMaxLevel(Texture.levels() - 1);
	vglTexture.setSwizzleR(Format.Swizzles[0]);
	vglTexture.setSwizzleG(Format.Swizzles[1]);
	vglTexture.setSwizzleB(Format.Swizzles[2]);
	vglTexture.setSwizzleA(Format.Swizzles[3]);

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceNum = static_cast<GLsizei> (Texture.layers() * Texture.faces());

	switch (Texture.target()) {
	case gli::TARGET_1D:
		glTexStorage1D(target, Texture.levels(), Format.Internal, Extent.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
				target, static_cast<GLint> (Texture.levels()), Format.Internal,
				Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceNum);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
				target, static_cast<GLint> (Texture.levels()), Format.Internal,
				Extent.x, Extent.y,
				Texture.target() == gli::TARGET_3D ? Extent.z : FaceNum);
		break;
	default:
		assert(0);
		break;
	}

	for (std::size_t LayerIndex = 0; LayerIndex < Texture.layers(); ++LayerIndex)
		for (std::size_t FaceIndex = 0; FaceIndex < Texture.faces(); ++FaceIndex)
			for (std::size_t Level = 0; Level < Texture.levels(); ++Level) {
				GLsizei const LayerGL = static_cast<GLsizei> (LayerIndex);
				glm::tvec3<GLsizei> Extent(Texture.extent(Level));
				target = gli::is_target_cube(Texture.target())
						? static_cast<GLenum> (GL_TEXTURE_CUBE_MAP_POSITIVE_X + FaceIndex) : target;

				switch (Texture.target()) {
				case gli::TARGET_1D:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage1D(
							target, static_cast<GLint> (Level), 0, Extent.x,
							Format.Internal, static_cast<GLsizei> (Texture.size(Level)),
							Texture.data(LayerIndex, FaceIndex, Level));
					else
						glTexSubImage1D(
							target, static_cast<GLint> (Level), 0, Extent.x,
							Format.External, Format.Type,
							Texture.data(LayerIndex, FaceIndex, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage2D(
							target, static_cast<GLint> (Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.Internal, static_cast<GLsizei> (Texture.size(Level)),
							Texture.data(LayerIndex, FaceIndex, Level));
					else
						glTexSubImage2D(
							target, static_cast<GLint> (Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.External, Format.Type,
							Texture.data(LayerIndex, FaceIndex, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage3D(
							target, static_cast<GLint> (Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.Internal, static_cast<GLsizei> (Texture.size(Level)),
							Texture.data(LayerIndex, FaceIndex, Level));
					else
						glTexSubImage3D(
							target, static_cast<GLint> (Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.External, Format.Type,
							Texture.data(LayerIndex, FaceIndex, Level));
					break;
				default: assert(0);
					break;
				}
			}
	return true;
}
} //namespace

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

TextureAC TextureAC::createFromDDS(const char* data, const size_t size) {
	libv::gl::TextureAC vglTexture;
	gliCreateTexture(vglTexture, data, size);
	return vglTexture;
}
TextureNC TextureNC::createFromDDS(const char* data, const size_t size) {
	libv::gl::TextureNC vglTexture;
	gliCreateTexture(vglTexture, data, size);
	return vglTexture;
}

} //namespace gl
} //namespace libv