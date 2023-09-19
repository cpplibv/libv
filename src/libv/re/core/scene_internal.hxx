// Project: libv.re, File: src/libv/re/scene.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/frustum.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/uniform/builtin.hpp>
#include <libv/utility/memory/observer_ref.hpp>
// #include <libv/utility/chrono.hpp>

#include <string>
#include <vector>
#include <memory>
#include <optional>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class BlockLights;

struct SceneInternal {
	std::string name;
	std::vector<Node_ptr> nodes;

	bool created = false;
	std::vector<std::unique_ptr<RenderPass>> passes;
	std::vector<libv::observer_ref<RenderPass>> passesWithNodes;

	std::optional<ContextPipeline> pipeline;

	// Active state (Frame)
	std::vector<libv::observer_ref<Node>> pendingNodes;
	std::vector<libv::observer_ref<Mesh>> pendingMeshes;
	std::vector<libv::observer_ref<Material>> pendingMaterials;

	// Active state (Persistent)
	// FrameViewContext frameViewContext;
	// FrameBufferContext frameViewContext;
	BlockLights blockLights;

	double lastTimeReal;
	double lastTimeSim;
	// libv::time_point lastTimeReal;
	// libv::time_point lastTimeSim;

	// Control variables
	bool freezeViewFrustum = false;

	// TODO P5: Temporal variables, find a better pattern for them:
	// BlockLights* blockLights = nullptr;
	// libv::vec3f eye;
	// libv::mat4f matV;

public:
	explicit SceneInternal(std::string&& name);
	~SceneInternal();

	void create(libv::GL& gl);
	void queue(const Light& light);
	void queue(Node* node, const libv::mat4f& matM, Mesh* mesh, Material* material, SubObjectIndex subObjectIndex);
	std::span<libv::vec2f> queueDepthQuery(ReadbackQueryIndex& queryIndex, uint32_t count);
	void render(Engine& re, libv::GL& gl, const Camera& camera, const Canvas_ptr& canvas, double timeSimulation, double timeReal);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
