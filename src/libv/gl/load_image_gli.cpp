// Project: libv.gl, File: src/libv/gl/load_image_gli.cpp

// hpp
#include <libv/gl/image.hpp>
// ext
#include <GL/glew.h>
#include <gli/gli.hpp>
// std
#include <optional>
// pro
#include <libv/gl/check.hpp>
//#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class ImageGLI : public detail::ImageImplementation {
	gli::texture texture;

public:
	ImageGLI(gli::texture&& texture) noexcept : texture(std::move(texture)) { }
	[[nodiscard]] virtual libv::vec2i size() const noexcept override;
	[[nodiscard]] virtual libv::vec4f pixel(int32_t level, int32_t x, int32_t y) const noexcept override;
	[[nodiscard]] virtual Texture createTexture() const noexcept override;
	virtual ~ImageGLI() noexcept override = default;
};

std::optional<Image> load_image_GLI(const std::string_view data) noexcept {
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

libv::vec4f ImageGLI::pixel(int32_t level, int32_t x, int32_t y) const noexcept {
//	assert(!gli::is_compressed(texture.format()) && "Not yet implemented");
//	assert(texture.target() == gli::TARGET_2D && "Not yet implemented");
//
//	gli::gl gli_gl(gli::gl::PROFILE_GL33);
//	const gli::gl::format format = gli_gl.translate(texture.format(), texture.swizzles());
//
//	assert(format.Type == gli::gl::type_format::TYPE_U8 && "Not yet implemented");
//	assert(format.External == gli::gl::external_format::EXTERNAL_RGBA && "Not yet implemented");
//
//	auto raw = reinterpret_cast<const uint8_t*>(texture.data(0, 0, level));
//
//	glm::tvec3<GLsizei> levelSize(texture.extent(level));
//
//	const auto index = levelSize.x * y * 4 + x * 4;
//
//	return {raw[index + 0], raw[index + 1], raw[index + 2], raw[index + 3]};

	assert("Not yet implemented");
	(void) level;
	(void) x;
	(void) y;
	return {0, 0, 0, 0};
}

Texture ImageGLI::createTexture() const noexcept {
	gli::gl gli_gl(gli::gl::PROFILE_GL33);
	const gli::gl::format format = gli_gl.translate(texture.format(), texture.swizzles());
	GLenum target = gli_gl.translate(texture.target());
	GLenum original_target = target;

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);
	libv::gl::checkGL();

	const glm::tvec3<GLsizei> baseSize = texture.extent();
	const auto faceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());

	switch (texture.target()) {
	case gli::TARGET_1D:
		glTexStorage1D(
				target, static_cast<GLint>(texture.levels()), format.Internal, baseSize.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
				target, static_cast<GLint>(texture.levels()), format.Internal,
				baseSize.x, texture.target() != gli::TARGET_1D_ARRAY ? baseSize.y : faceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
				target, static_cast<GLint>(texture.levels()), format.Internal,
				baseSize.x, baseSize.y,
				texture.target() == gli::TARGET_3D ? baseSize.z : faceTotal);
		break;
	default:
		return {textureID, TextureTarget{original_target}};
	}
	libv::gl::checkGL();

	for (size_t layer = 0; layer < texture.layers(); ++layer) {
		for (size_t face = 0; face < texture.faces(); ++face) {
			for (size_t level = 0; level < texture.levels(); ++level) {
				const auto layerGL = static_cast<GLsizei>(layer);
				glm::tvec3<GLsizei> levelSize(texture.extent(level));
				target = gli::is_target_cube(texture.target()) ?
						static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face) : target;

				switch (texture.target()) {
				case gli::TARGET_1D:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage1D(
								target, static_cast<GLint>(level), 0, levelSize.x,
								format.Internal, static_cast<GLsizei>(texture.size(level)),
								texture.data(layer, face, level));
					else
						glTexSubImage1D(
								target, static_cast<GLint>(level), 0, levelSize.x,
								format.External, format.Type,
								texture.data(layer, face, level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage2D(
								target, static_cast<GLint>(level),
								0, 0,
								levelSize.x,
								texture.target() == gli::TARGET_1D_ARRAY ? layerGL : levelSize.y,
								format.Internal, static_cast<GLsizei>(texture.size(level)),
								texture.data(layer, face, level));
					else
						glTexSubImage2D(
								target, static_cast<GLint>(level),
								0, 0,
								levelSize.x,
								texture.target() == gli::TARGET_1D_ARRAY ? layerGL : levelSize.y,
								format.External, format.Type,
								texture.data(layer, face, level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(texture.format()))
						glCompressedTexSubImage3D(
								target, static_cast<GLint>(level),
								0, 0, 0,
								levelSize.x, levelSize.y,
								texture.target() == gli::TARGET_3D ? levelSize.z : layerGL,
								format.Internal, static_cast<GLsizei>(texture.size(level)),
								texture.data(layer, face, level));
					else
						glTexSubImage3D(
								target, static_cast<GLint>(level),
								0, 0, 0,
								levelSize.x, levelSize.y,
								texture.target() == gli::TARGET_3D ? levelSize.z : layerGL,
								format.External, format.Type,
								texture.data(layer, face, level));
					break;
//				case gli::TARGET_RECT:
//					break;
//				case gli::TARGET_RECT_ARRAY:
//					break;
				default: break;
				}
			}
		}
	}

	libv::gl::checkGL();
	return {textureID, TextureTarget{original_target}};
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
