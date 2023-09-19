// Project: libv.re, File: src/libv/re/material/material_fullscreen.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>

#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialFullscreen : public Material {
public:
	using ptr = MaterialFullscreen_ptr;
	using cptr = MaterialFullscreen_cptr;

public:
	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
	}

public:
	explicit MaterialFullscreen(std::string_view fragmentShaderPath);
	inline MaterialFullscreen(const MaterialFullscreen&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialFullscreen& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(std::string_view fragmentShaderPath) {
		return ptr::make(fragmentShaderPath);
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
