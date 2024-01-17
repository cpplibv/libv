// Project: libv.re, File: src/libv/re/node/skybox.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/skybox_type.hpp>
#include <libv/re/node/object.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Skybox : public Object {
public:
	using ptr = Skybox_ptr;
	using cptr = Skybox_cptr;

public:
	explicit Skybox(Texture_ptr skyTexture, SkyboxType skyboxType = SkyboxType::cubemapXYZ);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Texture_ptr skyTexture, SkyboxType skyboxType = SkyboxType::cubemapXYZ) {
		return ptr::make(std::move(skyTexture), skyboxType);
	}

public:
	void textureSky(Texture_ptr value, SkyboxType type) noexcept;
	[[nodiscard]] Texture_ptr texture() const noexcept;
	[[nodiscard]] SkyboxType type() const noexcept;

	void intensity(float value) noexcept;
	[[nodiscard]] float intensity() const noexcept;

	void rotateZ(float value) noexcept;
	//[[nodiscard]] float rotateZ() const noexcept;

	void horizonOffset(float value) noexcept;
	[[nodiscard]] float horizonOffset() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
