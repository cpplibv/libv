// Project: libv.re, File: src/libv/re/material/internal/material_jfa_step.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialJFAStep : public Material {
public:
	using ptr = MaterialJFAStep_ptr;
	using cptr = MaterialJFAStep_cptr;

public:
	Uniform<libv::vec3i> packStepSourceTarget;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(packStepSourceTarget, "packStepSourceTarget");
	}

public:
	MaterialJFAStep();
	inline MaterialJFAStep(const MaterialJFAStep&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialJFAStep& other) const noexcept = default;

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
