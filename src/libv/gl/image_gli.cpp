// File: Texture.cpp, Created on 2014. december 6. 22:44, Author: Vader

// hpp
#include <libv/gl/image.hpp>
// ext
#include <GL/glew.h>
#include <gli/gli.hpp>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class ImageGLI : public detail::ImageImplementation {
	gli::texture texture;

public:
	ImageGLI(gli::texture&& texture) noexcept : texture(std::move(texture)) { }
	[[nodiscard]] virtual libv::vec2i size() const noexcept override;
	[[nodiscard]] virtual uint32_t createTexture() const noexcept override;
	virtual ~ImageGLI() noexcept override = default;
};

std::optional<Image> load_image_GLI(const std::string_view data) {
	auto texture = gli::load(data.data(), data.size());

	if (texture.empty())
		return std::nullopt;
	else
		return std::optional<Image>{std::make_shared<ImageGLI>(std::move(texture))};
}

// -------------------------------------------------------------------------------------------------

libv::vec2i ImageGLI::size() const noexcept {
	return {texture.extent().x, texture.extent().y};
}

uint32_t ImageGLI::createTexture() const noexcept {
	gli::gl gli_gl(gli::gl::PROFILE_GL33);
	const gli::gl::format Format = gli_gl.translate(texture.format(), texture.swizzles());
	GLenum Target = gli_gl.translate(texture.target());

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	glBindTexture(Target, TextureName);
	glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint> (texture.levels() - 1));
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);
	LIBV_GL_DEBUG_CHECK();

	const glm::tvec3<GLsizei> baseSize(texture.extent());
	const GLsizei FaceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());

	switch (texture.target()) {
	case gli::TARGET_1D:
		glTexStorage1D(
				Target, static_cast<GLint> (texture.levels()), Format.Internal, baseSize.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
				Target, static_cast<GLint> (texture.levels()), Format.Internal,
				baseSize.x, texture.target() != gli::TARGET_1D_ARRAY ? baseSize.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
				Target, static_cast<GLint> (texture.levels()), Format.Internal,
				baseSize.x, baseSize.y,
				texture.target() == gli::TARGET_3D ? baseSize.z : FaceTotal);
		break;
	default:
		assert(0);
		break;
	}
	LIBV_GL_DEBUG_CHECK();

	for (std::size_t Layer = 0; Layer < texture.layers(); ++Layer) {
		for (std::size_t Face = 0; Face < texture.faces(); ++Face) {
			for (std::size_t Level = 0; Level < texture.levels(); ++Level) {
				GLsizei const LayerGL = static_cast<GLsizei> (Layer);
				glm::tvec3<GLsizei> levelSize(texture.extent(Level));
				Target = gli::is_target_cube(texture.target())
						? static_cast<GLenum> (GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
						: Target;

				switch (texture.target()) {
				case gli::TARGET_1D:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage1D(
							Target, static_cast<GLint> (Level), 0, levelSize.x,
							Format.Internal, static_cast<GLsizei> (texture.size(Level)),
							texture.data(Layer, Face, Level));
					else
						glTexSubImage1D(
							Target, static_cast<GLint> (Level), 0, levelSize.x,
							Format.External, Format.Type,
							texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage2D(
							Target, static_cast<GLint> (Level),
							0, 0,
							levelSize.x,
							texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : levelSize.y,
							Format.Internal, static_cast<GLsizei> (texture.size(Level)),
							texture.data(Layer, Face, Level));
					else
						glTexSubImage2D(
							Target, static_cast<GLint> (Level),
							0, 0,
							levelSize.x,
							texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : levelSize.y,
							Format.External, Format.Type,
							texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage3D(
							Target, static_cast<GLint> (Level),
							0, 0, 0,
							levelSize.x, levelSize.y,
							texture.target() == gli::TARGET_3D ? levelSize.z : LayerGL,
							Format.Internal, static_cast<GLsizei> (texture.size(Level)),
							texture.data(Layer, Face, Level));
					else
						glTexSubImage3D(
							Target, static_cast<GLint> (Level),
							0, 0, 0,
							levelSize.x, levelSize.y,
							texture.target() == gli::TARGET_3D ? levelSize.z : LayerGL,
							Format.External, Format.Type,
							texture.data(Layer, Face, Level));
					break;
				default: assert(0);
					break;
				}
			}
		}
	}
	LIBV_GL_DEBUG_CHECK();
	return TextureName;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
