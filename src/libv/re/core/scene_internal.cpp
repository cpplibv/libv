// Project: libv.re, File: src/libv/re/core/scene_internal.cpp

#include <libv/re/core/scene_internal.hxx>

#include <libv/gl/gl.hpp>
#include <libv/re/core/canvas.hpp>
#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/engine.hpp>
#include <libv/re/core/pass/render_pass.hpp>
#include <libv/re/core/pass/render_pass_depth_prepass.hpp>
#include <libv/re/core/pass/render_pass_outline.hpp>
#include <libv/re/core/pass/render_pass_post_process.hpp>
#include <libv/re/core/pass/render_passes.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/material/material_fullscreen.hpp>
#include <libv/re/mesh/mesh_fullscreen.hpp>
#include <libv/re/node/camera.hpp>
#include <libv/re/node/light/light.hpp>
#include <libv/re/uniform/builtin.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

SceneInternal::SceneInternal(std::string&& name) :
	name(std::move(name)) {
}

SceneInternal::~SceneInternal() {
	// For the sake of forward declared types
}

void SceneInternal::create(libv::GL& gl) {
	passes.emplace_back(std::make_unique<RenderPassDepthPrime>());
	// passes.emplace_back(std::make_unique<RenderPassLightCull>());
	// passes.emplace_back(std::make_unique<RenderPassShadow>());
	passes.emplace_back(std::make_unique<RenderPassOpaque>());
	passes.emplace_back(std::make_unique<RenderPassBackground>());
	passes.emplace_back(std::make_unique<RenderPassTransparent>());
	passes.emplace_back(std::make_unique<RenderPassPostProcess>());
	passes.emplace_back(std::make_unique<RenderPassFlare>());
	passes.emplace_back(std::make_unique<RenderPassOutline>());
	passes.emplace_back(std::make_unique<RenderPassHUD>());

	for (const auto& pass : passes)
		if (pass->hasNodes()) // Find a better pattern?
			passesWithNodes.emplace_back(pass.get());

	pipeline.emplace(gl);
	pipeline->fullscreenMesh = MeshFullscreen::create();

	ContextRender ctx{gl, pipeline->contextFrame};
	for (auto& pass : passes)
		pass->create(*pipeline, ctx);
}

void SceneInternal::queue(const Light& light) {
	switch (light.type) {
	case LightType::disable:
		break;

	case LightType::directional: {
		assert(blockLights.numLightDirectionals < static_cast<uint32_t>(blockLights.lightDirectionals.size()) + 1);
		auto& uniform = blockLights.lightDirectionals[blockLights.numLightDirectionals];
		++blockLights.numLightDirectionals;

		uniform.directionW = light.transform.rotation.forward();
		uniform.radiance = light.color * light.intensity;
		break;
	}

	case LightType::point: {
		assert(blockLights.numLightPoints < static_cast<uint32_t>(blockLights.lightPoints.size()) + 1);
		auto& uniform = blockLights.lightPoints[blockLights.numLightPoints];
		++blockLights.numLightPoints;

		uniform.positionW = light.transform.position;
		uniform.range = light.range;
		uniform.radiance = light.color * light.intensity;
		uniform.falloff = light.falloff;
		uniform.attenuationFormula = light.attenuationFormula;
		break;
	}

	case LightType::spot: {
		assert(blockLights.numLightSpots < static_cast<uint32_t>(blockLights.lightSpots.size()) + 1);
		auto& uniform = blockLights.lightSpots[blockLights.numLightSpots];
		++blockLights.numLightSpots;

		uniform.positionW = light.transform.position;
		uniform.range = light.range;
		uniform.directionW = light.transform.rotation.forward();
		uniform.cosAngleInner = light.cosAngleInner;
		uniform.radiance = light.color * light.intensity;
		uniform.cosAngleOuter = light.cosAngleOuter;
		uniform.falloff = light.falloff;
		uniform.attenuationFormula = light.attenuationFormula;
		break;
	}
	} // switch end
}

void SceneInternal::queue(Node* node, const libv::mat4f& matM, Mesh* mesh, Material* material, SubObjectIndex subObjectIndex) {
	const auto position = xyz(matM.column(3));
	const auto matMV = pipeline->contextFrame.matV * matM;
	BlockModelMatrices blockModelMatrices;
	blockModelMatrices.matM = matM;
	blockModelMatrices.matMV = matMV;
	blockModelMatrices.matMInverse = matM.inverse_copy();
	const auto modelBlockOffset = pipeline->uniformBuffer.write(blockModelMatrices);

	if (!node->updatePending) {
		node->updatePending = true;
		pendingNodes.emplace_back(node);
	}
	if (!mesh->updatePending) {
		mesh->updatePending = true;
		pendingMeshes.emplace_back(mesh);
	}
	if (!material->updatePending) {
		material->updatePending = true;
		pendingMaterials.emplace_back(material);
	}

	const RenderNodeEntry entry{
			material,
			mesh,
			(pipeline->contextFrame.eyeW - position).length_sq(),
			node,
			subObjectIndex,
			modelBlockOffset,
	};

	for (const auto& pass : passesWithNodes)
		if (material->layers & pass->layerMask)
			pass->renderNodeEntries.push_back(entry);
}

std::span<libv::vec2f> SceneInternal::queueDepthQuery(ReadbackQueryIndex& queryIndex, uint32_t count) {
	return pipeline->depthReadbackBuffer.request(queryIndex, count);
}

void SceneInternal::render(Engine& re, libv::GL& gl, const Camera& camera, const Canvas_ptr& canvas, double timeSimulation, double timeReal) {
	if (!created) {
		created = true;
		create(gl);
	}

	// --- Camera / Canvas variables ---

	const auto canvasSizef = canvas->sizef();
	const auto canvasSizei = canvas->sizei();
	const auto matP = camera.projectionRevereZInf(canvasSizef);
	const auto matV = camera.view();
	const auto matVP = matP * matV;
	const auto matVPinv = matVP.inverse_copy();
	const auto matVinv = matV.inverse_copy();
	const auto eyeW = xyz(matVinv.column(3));
	const auto screenPicker = libv::screen_picker<float>{matVP, matVPinv, canvasSizef};

	// --- Begin Scene ---

	auto debugGroup_guard = gl.pushDebugGroup_guard("libv.re - Scene: " + name);

	// !!!

	pipeline->contextFrame.currentState = RenderState{};
	pipeline->contextFrame.deltaTimeRealf = libv::time_duration_f{timeReal - lastTimeReal};
	pipeline->contextFrame.deltaTimeSimf = libv::time_duration_f{timeSimulation - lastTimeSim};
	pipeline->contextFrame.screenPicker = screenPicker;
	if (!freezeViewFrustum)
		pipeline->contextFrame.viewFrustum = camera.tmpCameraPlayer.frustum_culler(canvasSizef);
	pipeline->contextFrame.eyeW = eyeW;
	pipeline->contextFrame.near = camera.near();
	pipeline->contextFrame.matV = matV;
	pipeline->contextFrame.canvasSize = canvasSizei;

	pipeline->uniformBuffer.gl_swap(gl);
	pipeline->indirectCommandBuffer.gl_swap(gl);
	pipeline->depthReadbackBuffer.gl_swap(gl);

	pipeline->rtStack.top()->position(canvas->positioni());
	pipeline->rtStack.top()->sampleCount(4); // <<< Settings.msaa
	pipeline->rtStack.top()->sampleFixed(false);
	pipeline->rtStack.top()->gl_bindDraw(gl, canvas->sizei(), pipeline->rtStack.top()->sampleCount(), pipeline->rtStack.top()->sampleFixed());

	ContextRender renderContext{gl, pipeline->contextFrame};
	pipeline->debug.gl_frameStart(*pipeline, renderContext);
	re.preRenderUpdate(gl);

	BlockRenderPass blockRenderPass;
	// const auto matPInverse = matP.inverse_copy();
	blockRenderPass.matP = matP;
	// blockRenderPass.matPInverse = matPInverse;
	blockRenderPass.matV = matV;
	// blockRenderPass.matVinv = matVinv;
	blockRenderPass.matVP = matVP;
	blockRenderPass.matVPinv = matVPinv;
	blockRenderPass.eyeW = eyeW;
	blockRenderPass.cameraForwardW = xyz(-matVinv.column(2));
	blockRenderPass.cameraRightW = xyz(matVinv.column(0));
	blockRenderPass.cameraUpW = xyz(matVinv.column(1));
	blockRenderPass.timeSimLooping = static_cast<float>(std::fmod(timeSimulation + 1800.0, 3600.0) - 1800.0);
	blockRenderPass.timeSimUncapped = static_cast<float>(timeSimulation);
	blockRenderPass.timeRealLooping = static_cast<float>(std::fmod(timeReal + 1800.0, 3600.0) - 1800.0);
	blockRenderPass.timeRealUncapped = static_cast<float>(timeReal);
	blockRenderPass.canvasSize = canvasSizef;
	blockRenderPass.pixelSize = 1.f / canvasSizef;
	// blockRenderPass.mousePosition = ...; // TODO P5: Canvas mousePosition for render pass block
	blockRenderPass.cameraNear = camera.near();
	blockRenderPass.cameraFar = camera.far();
	const auto builtinUniformBlockOffsetPass = pipeline->uniformBuffer.write(blockRenderPass);

	blockLights.numLightDirectionals = 0;
	blockLights.numLightPoints = 0;
	blockLights.numLightSpots = 0;

	// --- Queue / Gather ---

	{
		for (const auto& pass : passesWithNodes)
			pass->renderNodeEntries.clear();

		// const auto frustum = camera.frustum();
		// const auto shadowFrustums[] = shaderCasterLights[].frustum();

		ContextQueue queueContext{*this, pipeline->contextFrame};

		// TODO P1: Node hierarchy and recursion
		for (const auto& node : nodes) {
			// TODO P1: Shadow frustum culling
			// 		(object->layers & (passMaskBackground | passMaskFlare | passMaskHUD)).passed() ||
			// 		(object->layers & (passMaskOpaque | passMaskTransparent)).passed() &&
			// 				frustum.test(object.boundingSphere) ||
			// 		(object->layers & passMaskShadowCaster).passed()
			// 				&& shadowFrustums[].test(object.boundingSphere);

			node->queue(queueContext);
		}
	}

	// --- Update ---

	{
		auto debugGroup_guard = gl.pushDebugGroup_guard("Update");
		ContextUpdate updateContext{gl, pipeline->contextFrame};

		for (const auto& pass : passes)
			pass->update(updateContext);
		pipeline->fullscreenMesh->gl_update(updateContext);

		for (const auto& node : pendingNodes) {
			node->gl_update(updateContext);
			node->updatePending = false;
		}
		pendingNodes.clear();
		for (const auto& mesh : pendingMeshes) {
			mesh->gl_update(updateContext);
			mesh->updatePending = false;
		}
		pendingMeshes.clear();
		for (const auto& material : pendingMaterials) {
			material->gl_update(updateContext);
			material->updatePending = false;
		}
		pendingMaterials.clear();
	}

	const auto builtinUniformBlockOffsetLights = pipeline->uniformBuffer.write(blockLights);

	// --- Upload ---

	//> fork build material uniform blocks stream for dirty materials
	//> TODO P5: Fork sorts

	for (const auto& pass : passesWithNodes)
		pass->sort();

	//< From this point GL Thread is required

	{
		auto debugGroup_guard = gl.pushDebugGroup_guard("Upload");

		pipeline->uniformBuffer.gl_upload(gl);
		pipeline->indirectCommandBuffer.gl_upload(gl);
	}

	// --- Render ---

	// ClipControl for ReverseZ
	gl.clipControl(libv::gl::ClipOrigin::LowerLeft, libv::gl::ClipDepth::ZeroToOne);
	gl.clearColor(0.f, 0.f, 0.f, 0.f);
	gl.clearDepth(0.f);
	gl.clearStencil(0x00);
	gl.mask.color.set(true, true, true, true);
	gl.mask.depth.set(true);
	gl.mask.stencil.set(0xFF);

	pipeline->contextFrame.currentState.gl_apply(gl); // Apply default state to set up the current GL state

	// bind camera and render pass uniform blocks
	pipeline->uniformBuffer.gl_bind(gl, BlockRenderPass::binding, builtinUniformBlockOffsetPass, sizeof(BlockRenderPass));
	pipeline->uniformBuffer.gl_bind(gl, BlockLights::binding, builtinUniformBlockOffsetLights, sizeof(BlockLights));
	pipeline->indirectCommandBuffer.gl_bind(gl);

	//< join sort(renderQueue)

	for (const auto& pass : passes)
		pass->pass(*pipeline, renderContext);

	// Cleanup, Frame end maintenance
	pipeline->debug.gl_frameEnd(*pipeline, renderContext);
	assert(pipeline->rtStack.stackSize() == 1);
	re.postFrameUpdate(gl);

	lastTimeReal = timeReal;
	lastTimeSim = timeSimulation;

	// Restore GL states
	// TODO P1: Restore the previously used framebuffer / viewport / clip control, dont assume anything
	gl.clipControl(libv::gl::ClipOrigin::LowerLeft, libv::gl::ClipDepth::NegativeOneToOne);
	gl.clearDepth(1.f);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv