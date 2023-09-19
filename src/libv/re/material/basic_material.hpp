// Project: libv.re, File: src/libv/re/material/basic_material.hpp

#pragma once

#include <libv/gl/gl.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/uniform.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/uniform_buffer_mapped.hpp>
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/re/uniform/concept.hpp>
#include <libv/re/uniform/uniform.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct MaterialAccessor_assignShader {
	Shader_ptr& shader;
	Shader_ptr& shaderDepth;

	LIBV_FORCE_INLINE void operator()(is_uniform_wrapper auto& member, std::string_view name) {
		member.location.nonBlock.indirectNormal = shader->registerUniform(name);
		if (shaderDepth)
			member.location.nonBlock.indirectDepth = shaderDepth->registerUniform(name);
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_block_wrapper auto& member) {
		using block_type = typename std::remove_cvref_t<decltype(member)>::block_type;
		shader->registerBlockBinding(block_type::name, block_type::binding);
		if (shaderDepth)
			shaderDepth->registerBlockBinding(block_type::name, block_type::binding);
	}

	// LIBV_FORCE_INLINE void operator()(is_uniform_struct auto& member) {
	// LIBV_FORCE_INLINE void operator()(is_uniform_array auto& member) {
};

struct MaterialAccessor_gl_update {
	ContextUpdate& ctx;

	LIBV_FORCE_INLINE void operator()(is_uniform_wrapper auto& member, std::string_view name) {
		(void) member;
		(void) name;
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_block_wrapper auto& member) {
		using block_type = typename std::remove_cvref_t<decltype(member)>::block_type;
		ctx.frame.uniformBuffer.write(
				member.block.placement(),
				std::span{reinterpret_cast<const std::byte*>(&*member.block), sizeof(block_type)});
	}

	// LIBV_FORCE_INLINE void operator()(is_uniform_struct auto& member) {
	// LIBV_FORCE_INLINE void operator()(is_uniform_array auto& member) {
};

struct MaterialAccessor_gl_bind {
	ContextRender& ctx;
	Shader_ptr& shader;

	LIBV_FORCE_INLINE MaterialAccessor_gl_bind(ContextRender& ctx, Shader_ptr& shader, RenderState state) :
		ctx(ctx),
		shader(shader) {
		assert(shader);
		if (ctx.frame.currentState != state) {
			ctx.frame.currentState = state;
			state.gl_apply(ctx.gl);
		}
		shader->gl_bind(ctx.gl);
	}

	LIBV_FORCE_INLINE void operator()(Uniform<Texture>& member, std::string_view name) {
		(void) name;
		member.texture->gl_bind(ctx.gl, member.value);
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_wrapper auto& member, std::string_view name) {
		(void) name;
		const auto loc = shader->gl_uniformLocation(member.location.nonBlock.indirectNormal);
		if (loc != -1) // Call with -1 is not an error, but slower
			libv::gl::uniform(loc, member.value);
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_block_wrapper auto& member) {
		ctx.frame.uniformBuffer.gl_bind(ctx.gl, member.bufferBinding(), member.bufferOffset(), member.bufferSize());
	}

	// LIBV_FORCE_INLINE void operator()(is_uniform_struct auto& member) {
	// LIBV_FORCE_INLINE void operator()(is_uniform_array auto& member) {
};

struct MaterialAccessor_gl_bindDepth {
	ContextRender& ctx;
	Shader_ptr& shaderDepth;

	LIBV_FORCE_INLINE MaterialAccessor_gl_bindDepth(ContextRender& ctx, Shader_ptr& shaderDepth, RenderState state) :
		ctx(ctx),
		shaderDepth(shaderDepth) {
		assert(shaderDepth);
		if (ctx.frame.currentState != state) {
			ctx.frame.currentState = state;
			state.gl_applyDepth(ctx.gl);
		}
		shaderDepth->gl_bind(ctx.gl);
	}

	LIBV_FORCE_INLINE void operator()(Uniform<Texture>& member, std::string_view name) {
		(void) name;
		// TODO P1: neededForDepth could be assessed by the interface query api during assign
		// if (member.neededForDepth)
		member.texture->gl_bind(ctx.gl, member.value);
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_wrapper auto& member, std::string_view name) {
		(void) name;
		// TODO P1: neededForDepth could be assessed by the interface query api during assign
		// if (member.neededForDepth)
		const auto loc = shaderDepth->gl_uniformLocation(member.location.nonBlock.indirectDepth);
		if (loc != -1) // Call with -1 is not an error, but slower
			libv::gl::uniform(loc, member.value);
	}

	LIBV_FORCE_INLINE void operator()(is_uniform_block_wrapper auto& member) {
		// TODO P1: neededForDepth could be assessed by the interface query api during assign
		// if (member.neededForDepth)
		ctx.frame.uniformBuffer.gl_bind(ctx.gl, member.bufferBinding(), member.bufferOffset(), member.bufferSize());
	}

	// LIBV_FORCE_INLINE void operator()(is_uniform_struct auto& member) {
	// LIBV_FORCE_INLINE void operator()(is_uniform_array auto& member) {
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
