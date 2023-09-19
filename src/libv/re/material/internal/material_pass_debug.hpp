// Project: libv.re, File: src/libv/re/material/internal/material_pass_debug.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/common/debug_capture_mode.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialDebugPassCapture : public Material {
public:
	using ptr = MaterialDebugPassCapture_ptr;
	using cptr = MaterialDebugPassCapture_cptr;

public:
	Uniform<DebugCaptureMode> captureMode = DebugCaptureMode::normal;
	Uniform<int32_t> levelIndex = -1;
	Uniform<int32_t> layerIndex = -1;
	// Uniform<Texture> textureDiffuse = unitDiffuse;
	// Uniform<Texture> textureSky = unitSky;
	// Uniform<Texture> textureDepth = unitDepth;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(captureMode, "captureMode");
		access(levelIndex, "levelIndex");
		access(layerIndex, "layerIndex");
		// access(textureDiffuse, "textureDiffuse");
		// access(textureSky, "textureSky");
		// access(textureDepth, "textureDepth");
	}

public:
	MaterialDebugPassCapture();
	inline MaterialDebugPassCapture(const MaterialDebugPassCapture&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialDebugPassCapture& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual void gl_update(ContextUpdate& ctx) override;
	virtual void gl_bind(ContextRender& ctx) override;
	virtual void gl_bindDepth(ContextRender& ctx) override;

	virtual bool equals(const Material& other) const noexcept override;
	[[nodiscard]] virtual Material_ptr doClone() const override;
	[[nodiscard]] inline ptr clone() const { return static_pointer_cast<ptr::value_type>(doClone()); };
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
