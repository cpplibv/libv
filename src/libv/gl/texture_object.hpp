// Project: libv.gl, File: src/libv/gl/texture_object.hpp

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/assert.hpp>


namespace libv {
namespace gl {

// Texture -----------------------------------------------------------------------------------------

template <TextureTarget Target>
struct Texture_t {
	uint32_t id = 0;
	static constexpr TextureTarget target = Target;

	template <TextureTarget... Targets>
	constexpr inline void assert_target() noexcept {
		LIBV_GL_DEBUG_ASSERT_STATIC(((Targets == Target) || ...), "Operation is not supported on Texture Target");
	}
	template <TextureTarget... Targets>
	constexpr inline void assert_not_target() noexcept {
		LIBV_GL_DEBUG_ASSERT_STATIC(((Targets != Target) && ...), "Operation is not supported on Texture Target");
	}
};

struct Texture {
	uint32_t id = 0;
	TextureTarget target = TextureTarget::_2D;

	inline Texture() noexcept = default;

	inline Texture(uint32_t id, TextureTarget target) noexcept :
		id(id),
		target(target) { }

	template <TextureTarget Target>
	inline Texture(const Texture_t<Target>& texture) noexcept :
		id(texture.id),
		target(Target) { }

	template <TextureTarget Target>
	inline explicit operator Texture_t<Target>() noexcept {
		LIBV_GL_DEBUG_ASSERT(target == Target);
		return {id};
	}

	template <TextureTarget... Targets>
	constexpr inline void assert_target() noexcept {
		LIBV_GL_DEBUG_ASSERT((Targets == target) || ...);
	}
	template <TextureTarget... Targets>
	constexpr inline void assert_not_target() noexcept {
		LIBV_GL_DEBUG_ASSERT((Targets != target) && ...);
	}
};

using Texture1D = Texture_t<TextureTarget::_1D>;
using Texture1DArray = Texture_t<TextureTarget::_1DArray>;
using Texture2D = Texture_t<TextureTarget::_2D>;
using Texture2DArray = Texture_t<TextureTarget::_2DArray>;
using Texture2DMultisample = Texture_t<TextureTarget::_2DMultisample>;
using Texture2DMultisampleArray = Texture_t<TextureTarget::_2DMultisampleArray>;
using Texture3D = Texture_t<TextureTarget::_3D>;
using TextureCube = Texture_t<TextureTarget::CubeMap>;
using TextureCubeArray = Texture_t<TextureTarget::CubeMapArray>;
using TextureRectangle = Texture_t<TextureTarget::Rectangle>;

//using TextureBuffer = Texture_t<TextureTarget::Buffer>;

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
