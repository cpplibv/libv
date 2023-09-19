// Project: libv.re, File: src/libv/re/core/scene.cpp

#include <libv/re/core/scene.hpp>
#include <libv/re/core/scene_internal.hxx>

#include <libv/algo/erase_unstable.hpp>
#include <libv/re/log.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Scene* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Scene* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Scene::Scene(std::string name) :
	self(std::make_unique<SceneInternal>(std::move(name))) {
}

Scene::~Scene() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void Scene::add(const libv::re::Node_ptr& node) {
	self->nodes.emplace_back(node);
}

void Scene::add(libv::re::Node_ptr&& node) {
	self->nodes.emplace_back(std::move(node));
}

void Scene::remove(const Node_ptr& node) {
	// As during rendering, sorting queueing the nodes are referenced by raw pointers:
	// assert(current_scene_state != rendering);
	// or if current_scene_state == rendering, keep_alive_queue.emplace_back(node)
	// TODO P4: Use better container for self->nodes: O(n^2) remove
	libv::erase_unstable(self->nodes, node);
}

void Scene::render(Engine& re, libv::GL& gl, const Camera& camera, const Canvas_ptr& canvas, double timeSimulation, double timeReal) {
	self->render(re, gl, camera, canvas, timeSimulation, timeReal);
}

// -------------------------------------------------------------------------------------------------

void Scene::freezeViewFrustum(bool value) noexcept {
	self->freezeViewFrustum = value;
}

[[nodiscard]] bool Scene::freezeViewFrustum() const noexcept {
	return self->freezeViewFrustum;
}

void Scene::debugPass(std::string_view name, int index, DebugCaptureMode captureMode) {
	if (!name.empty()) {
		log_re.debug("Scene {} debug: {} {} {}", self->name, name, index, +captureMode);
		self->pipeline->debug.show(name, index, captureMode);
	} else {
		log_re.debug("Scene {} debug disabled", self->name);
		self->pipeline->debug.hide();
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
