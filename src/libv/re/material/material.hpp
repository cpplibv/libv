// Project: libv.re, File: src/libv/re/material/material.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/common/block_binding.hpp> // For convenience
#include <libv/re/common/layer_mask.hpp>
#include <libv/re/core/render_state.hpp>
#include <libv/re/resource/shader.hpp>
#include <libv/re/uniform/padding.hpp> // For convenience
#include <libv/re/uniform/uniform.hpp> // For convenience


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Material : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	using ptr = Material_ptr;
	using cptr = Material_cptr;

public:
	LayerMask layers = layerDepthCaster | layerOpaque | layerShadowCaster;
private:
	// bool dirty = true; // We will see if custom blocks will require dirty flag or not
	friend SceneInternal;
	bool updatePending = false;
public:
	RenderState state;
	Shader_ptr shader;
	Shader_ptr shaderDepth = nullptr; /// Can be null if material is never used in depth-only passes

public:
	template <typename Access>
	void access_uniforms(Access&) {
		// Empty base case without any uniforms
		// Built-in uniform blocks are managed internally as they are not per material, but per object/pass
	}

public:
	Material() = default;
	Material(Shader_ptr shader, Shader_ptr shaderDepth);
	inline Material(const Material& other) noexcept :
		layers(other.layers),
		updatePending(false),
		state(other.state),
		shader(other.shader),
		shaderDepth(other.shaderDepth) {
	}
	[[nodiscard]] bool operator==(const Material& other) const noexcept = default;
	virtual ~Material();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Shader_ptr shader, Shader_ptr shaderDepth = nullptr) {
		return ptr::make(std::move(shader), std::move(shaderDepth));
	}

public:
	[[nodiscard]] static inline bool equals(const Material& lhs, const Material& rhs) noexcept {
		return typeid(lhs) == typeid(rhs) && lhs.equals(rhs);
	}

public:
	virtual void gl_update(ContextUpdate& ctx);
	virtual void gl_bind(ContextRender& ctx);
	virtual void gl_bindDepth(ContextRender& ctx);

	virtual bool equals(const Material& other) const noexcept;
	[[nodiscard]] virtual Material_ptr doClone() const;
	[[nodiscard]] inline ptr clone() const { return static_pointer_cast<ptr::value_type>(doClone()); };
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
