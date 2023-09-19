// Project: libv.re, File: src/libv/re/material/internal/material_jfa_outline.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/material/material_fullscreen.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct UniformBlockJFAOutline {
	static constexpr std::string_view name = "JFAOutline";
	static constexpr BlockBinding binding = BlockBinding::passRelated;

	static constexpr uint32_t maxNumColors = 8; // Must match glsl

	int32_t sourceLayer = 0;
	float outlineRadius = 5.f;
	float outlineColorIDDecoder = 0.f;
	Padding<4> _p0;

	std::array<libv::vec4f, maxNumColors> outlineColors;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(sourceLayer, "sourceLayer");
		access(outlineRadius, "outlineRadius");
		access(outlineColorIDDecoder, "outlineColorIDDecoder");
		access(_p0, "_p0");
		access(outlineColors, "outlineColors");
	}

	[[nodiscard]] bool operator==(const UniformBlockJFAOutline& other) const noexcept = default;
};

class MaterialJFAOutline : public MaterialFullscreen {
public:
	using ptr = MaterialJFAOutline_ptr;
	using cptr = MaterialJFAOutline_cptr;

public:
	Uniform<UniformBlockJFAOutline> block = UniformBlock_ptr<UniformBlockJFAOutline>::create();

	template <typename Access>
	void access_uniforms(Access& access) {
		MaterialFullscreen::access_uniforms(access);
		access(block);
	}

public:
	explicit MaterialJFAOutline(float radius);
	inline MaterialJFAOutline(const MaterialJFAOutline&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialJFAOutline& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(float radius = 5.f) {
		return ptr::make(radius);
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
