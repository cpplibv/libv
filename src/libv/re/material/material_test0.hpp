// Project: libv.re, File: src/libv/re/material/material_test0.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/material/material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MaterialTest0 : public Material {
public:
	using ptr = MaterialTest0_ptr;
	using cptr = MaterialTest0_cptr;

public:
	struct UniformSharedBlock {
		static constexpr std::string_view name = "Shared";
		static constexpr BlockBinding binding = BlockBinding::custom0;

		libv::vec4f shared_block_color;

		template <typename Access>
		constexpr void access_uniforms(Access& access) {
			access(shared_block_color, "shared_block_color");
		}

		[[nodiscard]] bool operator==(const UniformSharedBlock& other) const noexcept = default;
	};

	struct UniformCustomBlock {
		static constexpr std::string_view name = "Custom";
		static constexpr BlockBinding binding = BlockBinding::custom1;

		libv::vec4f custom_block_color;

		template <typename Access>
		constexpr void access_uniforms(Access& access) {
			access(custom_block_color, "custom_block_color");
		}

		[[nodiscard]] bool operator==(const UniformCustomBlock& other) const noexcept = default;
	};

	struct UniformCustomStruct {
		Uniform<libv::vec4f> color0;
		Uniform<libv::vec4f> color1;

		template <typename Access>
		constexpr void access_uniforms(Access& access) {
			access(color0, "color0");
			access(color1, "color1");
		}

		[[nodiscard]] bool operator==(const UniformCustomStruct& other) const noexcept = default;
	};

	Uniform<Texture> texture0 = unitDiffuse;
	Uniform<Texture> texture1 = TextureUnit{1};
	Uniform<Texture> texture2 = TextureUnit{2};
	Uniform<Texture> texture3 = TextureUnit{3};
	Uniform<Texture> texture4 = TextureUnit{4};
	Uniform<Texture> texture5 = TextureUnit{5};
	Uniform<Texture> textureSky;
	Uniform<int32_t> mode = 0;
	Uniform<libv::vec4f> uniform_color = libv::vec4f(0, 1, 1, 1);
	Uniform<float> uniform_dynamic = 0.f;
	// Uniform<UniformCustomStruct> uniform_struct;
	Uniform<UniformCustomBlock> block_custom = UniformBlock_ptr<UniformCustomBlock>::create();
	Uniform<UniformSharedBlock> block_shared = UniformBlock_ptr<UniformSharedBlock>::create();

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(texture0, "texture0");
		access(texture1, "texture1");
		access(texture2, "texture2");
		access(texture3, "texture3");
		access(texture4, "texture4");
		access(texture5, "texture5");
		access(textureSky, "textureSky");
		access(mode, "mode");
		access(uniform_color, "uniform_color");
		access(uniform_dynamic, "uniform_dynamic");
		// TODO P4: Non-Block uniform struct support
		// access(uniform_struct, "uniform_struct");
		access(block_custom); // Goes for T::binding / T::name
		access(block_shared); // Goes for T::binding / T::name
	}

public:
	explicit MaterialTest0();
	explicit MaterialTest0(const MaterialScanner& scan);
	inline MaterialTest0(const MaterialTest0&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialTest0& other) const noexcept = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
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
