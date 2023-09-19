// Project: libv.re, File: src/libv/re/material/internal/material_outline_mask.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialOutlineMask : public Material {
public:
	using ptr = MaterialOutlineMask_ptr;
	using cptr = MaterialOutlineMask_cptr;

public:
	libv::vec4f color; // This is not a uniform as mask will work with color ID
	Uniform<float> colorID = 0.f; // colorID is updated by the Outline RenderPass from the color

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(colorID, "colorID");
	}

public:
	MaterialOutlineMask(NodeType nodeType, libv::vec4f color);
	inline MaterialOutlineMask(const MaterialOutlineMask&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialOutlineMask& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(NodeType nodeType, libv::vec4f color) {
		return ptr::make(nodeType, color);
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
