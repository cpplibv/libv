// Project: libv, File: app/star/game/scene/scene_surface.cpp

#include <star/game/scene/scene_surface.hpp>

#include <libv/ctrl/controls.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/math/plane.hpp>
#include <libv/math/ray.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/core/scene.hpp>
#include <libv/re/material/material_solid.hpp>
#include <libv/re/mesh/mesh_sphere.hpp>
#include <libv/re/node/camera.hpp>
#include <libv/re/node/editor_grid.hpp>
#include <libv/re/node/object.hpp>
#include <libv/re/node/skybox.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/sun/camera.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component_system/bean.hpp>
#include <libv/utility/screen_picker.hpp>

#include <utility>

#include <star/game/control/surface_controller.hpp>
#include <star/game/control/time_controller.hpp>
#include <star/game/scene/scenes.hpp>
#include <star/game/sim/universe.hpp>
#include <star/game/ui/controlled_scene.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

CanvasSurface::CanvasSurface(libv::Nexus& nexus, Surface_sp surface_, TimeController_sp timeController_) :
	scene(libv::re::Scene::create("Surface Scene")),
	camera(libv::re::Camera::create()),
	surface(std::move(surface_)),
	timeController(std::move(timeController_)) {
	(void) nexus;

	// TODO P2: Proper camera
	camera->tmpCameraPlayer.look_at({-3.f, -6.f, 4.0f}, {0, 0, 0});
	camera->tmpCameraPlayer.fov_y(libv::deg_to_rad(75.0f));
	camera->tmpCameraPlayer.far(50.0f); // With inf projection far is only used for linearized visualization (and camera controls)
	camera->tmpCameraPlayer.near(0.5f);

	scene->add(editorGrid = libv::re::EditorGrid::create());

	ballMesh = libv::re::MeshSphere::create(12);
	ballMaterial = libv::re::MaterialSolid::create(libv::vec4f{0.5f, 0.25f, 0.25f, 1.f});

	skybox = libv::re::Skybox::create(libv::r.texture.load_async(surface->sky.texture), surface->sky.type);
	skybox->intensity(surface->sky.intensity);
	skybox->rotateZ(surface->sky.rotateZ);
	skybox->horizonOffset(surface->sky.horizonOffset);
	for (const auto& ball : surface->balls)
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{ball.position, libv::quatf::identity(), libv::vec3f::one(ball.size)}, ballMaterial, ballMesh));
	scene->add(movingBall = libv::re::Object::create(libv::re::Transform{surface->movingBall.position, libv::quatf::identity(), libv::vec3f::one(surface->movingBall.size)}, ballMaterial, ballMesh));
	scene->add(pickingBall = libv::re::Object::create(libv::re::Transform{surface->pickingBall.position, libv::quatf::identity(), libv::vec3f::one(surface->pickingBall.size)}, ballMaterial, ballMesh));

	scene->add(skybox);
	for (const auto& ball : balls)
		scene->add(ball);
}

CanvasSurface::~CanvasSurface() {
// 	scene->remove(skybox);
// 	for (const auto& ball : balls)
// 		scene->remove(ball);
}

void CanvasSurface::render(libv::GL& gl) {
	// Picking code
	screenPicker = camera->tmpCameraPlayer.picker(canvas_size); // Picker update, and passive picking is best placed before render

	const auto mouseCoord = calculate_local_mouse_coord();
	mouseRay = libv::ray3f{camera->tmpCameraPlayer.eye(), screenPicker.to_world(mouseCoord)};
	const auto plane = libv::planef::from_normal_point({0, 0, 1}, {0, 0, 0});

	const auto intersection = libv::intersect_ray_plane(mouseRay, plane);
	pickingBall->visible(intersection.has_value());
	surface->pickingBall.position = intersection ? *intersection : libv::vec3f{0};

	// Update render state
	// TODO P3: Model-View state sync utilities update(scene, balls, surface->balls, createFn, updateFn);
	const auto sizeOld = balls.size();
	const auto sizeNew = surface->balls.size();
	for (std::size_t i = sizeNew; i < sizeOld; ++i)
		scene->remove(balls[i]);
	balls.resize(sizeNew);
	for (std::size_t i = sizeOld; i < sizeNew; ++i) {
		balls[i] = libv::re::Object::create(libv::re::Transform{surface->balls[i].position, libv::quatf::identity(), libv::vec3f::one(surface->balls[i].size)}, ballMaterial, ballMesh);
		scene->add(balls[i]);
	}
	for (std::size_t i = 0; i < sizeOld; ++i) {
		balls[i]->transform.position = surface->balls[i].position;
		balls[i]->transform.scale = libv::vec3f{surface->balls[i].size};
	}

	pickingBall->transform.position = surface->pickingBall.position;
	movingBall->transform.position = surface->movingBall.position;

	// Execute render
	scene->render(
			gl,
			*camera,
			(canvas_position + 0.5f).cast<int32_t>(),
			(canvas_size + 0.5f).cast<int32_t>(),
			mouseCoord,
			timeController->timeSim.count(),
			timeController->timeReal().count());
}

// =================================================================================================

SceneSurface::SceneSurface(libv::Nexus& nexus, Surface_sp surface_, TimeController_sp timeController_) :
	surface(std::move(surface_)),
	timeController(std::move(timeController_)),
	surfaceController(nexus) {
}

void SceneSurface::controls(ControllerContext& ctx) {
	ctx.attached<SurfaceController>(&surfaceController);
	ctx.attached<libv::sun::BaseCameraOrbit>(&canvas->camera->tmpCameraPlayer);
}

[[nodiscard]] libv::ui::Component SceneSurface::createScene(libv::Nexus& nexus) {
	auto layers = libv::ui::PanelAnchor::n("layers");

	canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas", nexus, surface, timeController);
	canvas.z_index_offset(-100);
	surfaceController.init(surface.get(), &canvas.object()); // Sadness (2 step init)

	return layers;
}

// =================================================================================================

libv::ui::Component createSceneSurface(libv::Nexus& nexus, Surface_sp surface, TimeController_sp timeController) {
	auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");
	return star::createControlledScene<SceneSurface>(controls, nexus, std::move(surface), std::move(timeController))(nexus);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
