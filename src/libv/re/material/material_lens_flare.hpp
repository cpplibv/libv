// Project: libv.re, File: src/libv/re/material/material_solid.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct UniformBlockLensFlare {
	static constexpr std::string_view name = "LensFlare";
	static constexpr BlockBinding binding = BlockBinding::custom0;

	libv::vec4f targetPositionW;
	libv::vec3f lightColor = libv::vec3f{1, 1, 1};
	Padding<4> _p0;

	template <typename Access>
	void access_uniforms(Access& access) {
		access(targetPositionW, "targetPositionW");
		access(lightColor, "lightColor");
		access(_p0, "_p0");
	}
};

class MaterialLensFlare : public Material {
public:
	using ptr = MaterialLensFlare_ptr;
	using cptr = MaterialLensFlare_cptr;

public:
	Uniform<Texture> texture;
	Uniform<float> visibility = 1.f;
	Uniform<UniformBlockLensFlare> lensFlareInfo;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(texture, "texture");
		access(visibility, "visibility");
		access(lensFlareInfo);
	}

public:
	explicit MaterialLensFlare(Texture_ptr&& texture, UniformBlock_ptr<UniformBlockLensFlare>&& lensFlareInfo);
	inline MaterialLensFlare(const MaterialLensFlare&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialLensFlare& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Texture_ptr&& texture, UniformBlock_ptr<UniformBlockLensFlare> lensFlareInfo) {
		return ptr::make(std::move(texture), std::move(lensFlareInfo));
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
