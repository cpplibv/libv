// Project: libv.re, File: src/libv/re/material/material_skybox.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/material/skybox_type.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialSkybox : public Material {
public:
	using ptr = MaterialSkybox_ptr;
	using cptr = MaterialSkybox_cptr;

public:
	Uniform<Texture> textureSky = unitSky;
	Uniform<SkyboxType> skyboxType = SkyboxType::cubemapXYZ;
	Uniform<float> intensity = 1.f;
	Uniform<float> horizonOffset = 0.f;
	Uniform<libv::mat2f> matRotateZ = libv::mat2f::identity();

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(textureSky, "textureSky");
		access(skyboxType, "skyboxType");
		access(intensity, "intensity");
		access(horizonOffset, "horizonOffset");
		access(matRotateZ, "matRotateZ");
	}

public:
	explicit MaterialSkybox(Texture_ptr sky, SkyboxType skyboxType = SkyboxType::cubemapXYZ);
	inline MaterialSkybox(const MaterialSkybox&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialSkybox& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Texture_ptr sky, SkyboxType skyboxType = SkyboxType::cubemapXYZ) {
		return ptr::make(std::move(sky), skyboxType);
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
