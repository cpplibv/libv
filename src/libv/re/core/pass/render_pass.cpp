// Project: libv.re, File: src/libv/re/core/pass/render_pass.cpp


#include <libv/re/core/pass/render_pass.hpp>

#include <libv/algo/sort.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/query.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/node/node.hpp>
#include <libv/re/uniform/builtin.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void RenderPass::create(ContextPipeline& pipeline, ContextRender& ctx) {
	(void) pipeline;
	for (int i = 0; i < numQueries; ++i)
		ctx.gl(timeQueries[i]).dsa_create(libv::gl::QueryType::TimeElapsed);
}

struct SorterShaderDepthThenDistanceIncreasing {
	static inline bool operator()(const RenderNodeEntry& lhs, const RenderNodeEntry& rhs) {
		if (lhs.material->shaderDepth->id() < rhs.material->shaderDepth->id())
			return true;
		if (lhs.material->shaderDepth->id() > rhs.material->shaderDepth->id())
			return false;

		if (lhs.distanceSQ < rhs.distanceSQ)
			return true;
		if (lhs.distanceSQ > rhs.distanceSQ)
			return false;

		return false;
	}
};

struct SorterMaterial {
	static inline bool operator()(const RenderNodeEntry& lhs, const RenderNodeEntry& rhs) {
		// This sorting does not directly account for texture and uniform block bindings values,
		// but indirectly by having the same material instance does account for most use-case

		if (lhs.material->shader->id() < rhs.material->shader->id())
			return true;
		if (lhs.material->shader->id() > rhs.material->shader->id())
			return false;

		if (lhs.material->state.as_int() < rhs.material->state.as_int())
			return true;
		if (lhs.material->state.as_int() > rhs.material->state.as_int())
			return false;

		// TODO P2: If material matches last material, skip bind and uniform assignment that are inside the material
		if (lhs.material < rhs.material)
			return true;
		if (lhs.material > rhs.material)
			return false;

		if (lhs.mesh < rhs.mesh)
			return true;
		if (lhs.mesh > rhs.mesh)
			return false;

		return false;
	}
};

void RenderPass::sort() {
	switch (sortingOrder) {
	case RenderSortingOrder::none:
		break; // Noop
	case RenderSortingOrder::distanceIncreasing:
		libv::sort_unstable(renderNodeEntries, std::less<>{}, &RenderNodeEntry::distanceSQ);
		break;
	case RenderSortingOrder::distanceDecreasing:
		libv::sort_unstable(renderNodeEntries, std::less<>{}, &RenderNodeEntry::distanceSQ);
		break;
	case RenderSortingOrder::shaderDepthThenDistanceIncreasing:
		libv::sort_unstable(renderNodeEntries, SorterShaderDepthThenDistanceIncreasing{});
		break;
	case RenderSortingOrder::material:
		libv::sort_unstable(renderNodeEntries, SorterMaterial{});
		break;
	}
}

void RenderPass::update(ContextUpdate& ctx) {
	doUpdate(ctx);
}

void RenderPass::pass(ContextPipeline& pipeline, ContextRender& ctx) {
	auto debugGroup_guard = ctx.gl.pushDebugGroup_guard(name);

	// TODO P2: Wrapper object for rotating query objects
	if (++currentQueryIndex == numQueries)
		currentQueryIndex = 0;
	if (auto time = ctx.gl(timeQueries[currentQueryIndex]).result_no_wait_i64())
		elapsedTimeNS = time;

	// TODO P1: Collect performance metrics (elapsedTimeNS)
	// log_re.trace("Pass {:<15}: {:5.3f}ms", name, static_cast<double>(elapsedTimeNS) / 1000. / 1000.);
	ctx.gl(timeQueries[currentQueryIndex]).begin();

	// TODO P3: overrides are not generalized, currently only gl_renderDepth relies on them
	if (overrideState) {
		ctx.frame.currentState = *overrideState;
		// Apply override state manual
		ctx.frame.currentState.gl_apply(ctx.gl);
	}

	doPass(pipeline, ctx);

	if (overrideState) {
		// ReSync state as overridden states are not tracks
		// (Currently gl_applyDepth (called from gl_renderDepth) did not track every state)
		ctx.frame.currentState.gl_apply(ctx.gl);
	}

	ctx.gl(timeQueries[currentQueryIndex]).end();
}

void RenderPass::gl_renderNodes(ContextRender& ctx) {
	Material* lastMaterial = nullptr;
	for (const auto& entry : renderNodeEntries) {
		if (lastMaterial != entry.material) {
			entry.material->gl_bind(ctx);
			lastMaterial = entry.material;
		}
		ctx.frame.uniformBuffer.gl_bind(ctx.gl, BlockModelMatrices::binding, entry.builtinUniformBlockOffsetModel, sizeof(BlockModelMatrices));
		entry.node->gl_render(ctx, entry.subObjectIndex);
	}
}

void RenderPass::gl_renderDepthNodes(ContextRender& ctx) {
	Material* lastMaterial = nullptr;
	for (const auto& entry : renderNodeEntries) {
		if (lastMaterial != entry.material) {
			entry.material->gl_bindDepth(ctx);
			lastMaterial = entry.material;
		}
		ctx.frame.uniformBuffer.gl_bind(ctx.gl, BlockModelMatrices::binding, entry.builtinUniformBlockOffsetModel, sizeof(BlockModelMatrices));
		entry.node->gl_render(ctx, entry.subObjectIndex);
	}
}

// -------------------------------------------------------------------------------------------------

void RenderPass::doCreate(ContextPipeline& pipeline) {
	(void) pipeline; // Default is noop
}

void RenderPass::doUpdate(ContextUpdate& ctx) {
	(void) ctx; // Default is noop
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
