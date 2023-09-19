// Project: libv.re, File: src/libv/re/core/pass/render_pass.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/common/async_ring_size.hpp>
#include <libv/re/common/layer_mask.hpp>
#include <libv/re/common/sub_object_index.hpp>
#include <libv/re/core/render_state.hpp>
#include <libv/re/core/safe_gl.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class RenderSortingOrder {
	none,
	distanceIncreasing,
	distanceDecreasing,
	shaderDepthThenDistanceIncreasing,
	material,
};

// enum class RenderPassOrder {
// 	beforeEverything,
//
// 	beforePrePass,
// 	afterPrePass,
// 	beforeOpaque,
// 	afterOpaque,
// 	beforeSkybox,
// 	afterSkybox,
// 	beforeTransparent,
// 	afterTransparent,
// 	beforePostProcessing,
// 	afterPostProcessing,
//
// 	afterEverything,
// };

// -------------------------------------------------------------------------------------------------

struct RenderNodeEntry {
	Material* material = nullptr; // Non-owning, scene keeps it alive
	Mesh* mesh = nullptr; // Non-owning, scene keeps it alive
	float distanceSQ = 0;

	Node* node = nullptr; // Non-owning, scene keeps it alive
	SubObjectIndex subObjectIndex{0};
	uint32_t builtinUniformBlockOffsetModel = 0;
};

// -------------------------------------------------------------------------------------------------

class RenderPass {
private:
	std::string name;

	// TODO P1: Rotating query class wrapper
	// TODO P1: Create a way to have nested stackable libv.gl time query objects (it will most likely be a central object controlling it (it might end up inside libv.gl GL)
	static constexpr int numQueries = maxAsyncRingSize;
	int currentQueryIndex = 0;
	GLSafeQuery timeQueries[numQueries];
	int64_t elapsedTimeNS = -1;

public:
	friend Scene;

	LayerMask layerMask;
	std::vector<RenderNodeEntry> renderNodeEntries; /// Temporary container used during render
	RenderSortingOrder sortingOrder = RenderSortingOrder::none;

	bool hasNodes_ = true;

public:
	[[nodiscard]] constexpr inline bool hasNodes() const noexcept {
		return hasNodes_;
	}

protected:
	// Material_ptr overrideMaterial;
	std::optional<RenderState> overrideState;

public:
	explicit RenderPass(std::string name) :
		name(std::move(name)) {}

	virtual ~RenderPass() = default;

public:
	void create(ContextPipeline& pipeline, ContextRender& ctx);
	void sort();
	void update(ContextUpdate& ctx);
	void pass(ContextPipeline& pipeline, ContextRender& ctx);

protected:
	void gl_renderNodes(ContextRender& ctx);
	void gl_renderDepthNodes(ContextRender& ctx);

private:
	virtual void doCreate(ContextPipeline& pipeline);
	virtual void doUpdate(ContextUpdate& ctx);
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) = 0;
};

// -------------------------------------------------------------------------------------------------

// class RenderPassWithNodes : RenderPass {
// class RenderPassOverlay : RenderPass {

class RenderPassNodeless : public RenderPass {
public:
	RenderPassNodeless(std::string name) : RenderPass(std::move(name)) {
		hasNodes_ = false;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
