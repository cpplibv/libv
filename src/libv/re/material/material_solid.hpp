// Project: libv.re, File: src/libv/re/material/material_solid.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialSolid : public Material {
public:
	using ptr = MaterialSolid_ptr;
	using cptr = MaterialSolid_cptr;

public:
	Uniform<libv::vec4f> uniform_color;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(uniform_color, "uniform_color");
	}

public:
	explicit MaterialSolid(libv::vec4f color = libv::vec4f(1, 0, 0, 1));
	explicit MaterialSolid(const MaterialScanner& scan);
	inline MaterialSolid(const MaterialSolid&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialSolid& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(libv::vec4f color = libv::vec4f(1, 0, 0, 1)) {
		return ptr::make(color);
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(const MaterialScanner& scan) {
		return ptr::make(scan);
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