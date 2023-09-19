// Project: libv.re, File: src/libv/re/core/render_state.cpp

#include <libv/gl/gl.hpp>
#include <libv/re/core/render_state.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void RenderState::gl_apply(libv::GL& gl) const noexcept {
	gl.capability.blend.set(blend());
	gl.capability.cull.set(cull());
	gl.capability.depthTest.set(depthTest());
	// gl.capability.multisample.set(multisample());
	gl.capability.rasterizerDiscard.set(rasterizerDiscard());
	gl.capability.scissorTest.set(scissorTest());
	gl.capability.stencilTest.set(stencilTest());
	// gl.capability.textureCubeMapSeamless.set(textureCubeMapSeamless());

	// gl.mask.color.set(colorWrite());
	gl.mask.depth.set(depthWrite());

	if (stencilTest()) {
		// If stencil test is disabled no need to update any other stencil state
		gl.mask.stencil.set(stencilWrite());
		gl.stencil.function.set(stencilFunction(), stencilFunctionReference(), stencilFunctionMask());
		if (stencilWrite() != 0)
			gl.stencil.operation.set(stencilOpStencilFail(), stencilOpDepthFail(), stencilOpStencilAndDepthPass());
	}

	gl.frontFace.set(frontFace());
	gl.cullFace.set(cullFace());

	gl.polygonMode.set(polygonMode());

	gl.depthFunction.set(depthFunction());
	gl.blendFunction.set(blendSrc(), blendDst());

	gl.clipPlanes.set(clipPlanes());
}

void RenderState::gl_applyDepth(libv::GL& gl) const noexcept {
	// For depth states some state tracking is disabled:
	// - write masks and depth functions are the same for all and are set by the calling pass
	// - blend properties are indifferent

	// gl.capability.blend.set(blend());
	gl.capability.cull.set(cull());
	// gl.capability.depthTest.set(depthTest());
	// gl.capability.multisample.set(multisample());
	gl.capability.rasterizerDiscard.set(rasterizerDiscard());
	gl.capability.scissorTest.set(scissorTest());
	// gl.capability.stencilTest.set(stencilTest());
	// gl.capability.textureCubeMapSeamless.set(textureCubeMapSeamless());

	// gl.mask.color.set(colorWrite());
	// gl.mask.depth.set(depthWrite());

	// if (stencilTest()) {
	//	// If stencil test is disabled no need to update any other stencil state
	// 	gl.mask.stencil.set(stencilWrite());
	// 	gl.stencil.function.set(stencilFunction(), stencilFunctionReference(), stencilFunctionMask());
	// 	if (stencilWrite() != 0)
	// 		gl.stencil.operation.set(stencilOpStencilFail(), stencilOpDepthFail(), stencilOpStencilAndDepthPass());
	// }

	gl.frontFace.set(frontFace());
	gl.cullFace.set(cullFace());

	gl.polygonMode.set(polygonMode());

	// gl.depthFunction.set(depthFunction());
	// gl.blendFunction.set(blendSrc(), blendDst());

	gl.clipPlanes.set(clipPlanes());
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
