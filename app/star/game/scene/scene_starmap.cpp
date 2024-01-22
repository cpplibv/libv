// Project: libv, File: app/star/game/scene/scene_starmap.cpp

#include <star/game/scene/scene_starmap.hpp>

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

CanvasStarmap::CanvasStarmap(libv::Nexus& nexus, Starmap_sp starmap_, TimeController_sp timeController_) :
	scene(libv::re::Scene::create("Starmap Scene")),
	camera(libv::re::Camera::create()),
	starmap(std::move(starmap_)),
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

	skybox = libv::re::Skybox::create(libv::r.texture.load_async(starmap->sky.texture), starmap->sky.type);
	skybox->intensity(starmap->sky.intensity);
	skybox->rotateZ(starmap->sky.rotateZ);
	skybox->horizonOffset(starmap->sky.horizonOffset);
	for (const auto& ball : starmap->balls)
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{ball.position, libv::quatf::identity(), libv::vec3f::one(ball.size)}, ballMaterial, ballMesh));
	scene->add(movingBall = libv::re::Object::create(libv::re::Transform{starmap->movingBall.position, libv::quatf::identity(), libv::vec3f::one(starmap->movingBall.size)}, ballMaterial, ballMesh));
	scene->add(pickingBall = libv::re::Object::create(libv::re::Transform{starmap->pickingBall.position, libv::quatf::identity(), libv::vec3f::one(starmap->pickingBall.size)}, ballMaterial, ballMesh));

	scene->add(skybox);
	for (const auto& ball : balls)
		scene->add(ball);
}

CanvasStarmap::~CanvasStarmap() {
// 	scene->remove(skybox);
// 	for (const auto& ball : balls)
// 		scene->remove(ball);
}

void CanvasStarmap::render(libv::GL& gl) {
	// Picking code
	screenPicker = camera->tmpCameraPlayer.picker(canvas_size); // Picker update, and passive picking is best placed before render

	const auto mouseCoord = calculate_local_mouse_coord();
	mouseRay = libv::ray3f{camera->tmpCameraPlayer.eye(), screenPicker.to_world(mouseCoord)};
	const auto plane = libv::planef::from_normal_point({0, 0, 1}, {0, 0, 0});

	const auto intersection = libv::intersect_ray_plane(mouseRay, plane);
	pickingBall->visible(intersection.has_value());
	starmap->pickingBall.position = intersection ? *intersection : libv::vec3f{0};

	// Update render state
	// TODO P3: Model-View state sync utilities update(scene, balls, starmap->balls, createFn, updateFn);
	const auto sizeOld = balls.size();
	const auto sizeNew = starmap->balls.size();
	for (std::size_t i = sizeNew; i < sizeOld; ++i)
		scene->remove(balls[i]);
	balls.resize(sizeNew);
	for (std::size_t i = sizeOld; i < sizeNew; ++i) {
		balls[i] = libv::re::Object::create(libv::re::Transform{starmap->balls[i].position, libv::quatf::identity(), libv::vec3f::one(starmap->balls[i].size)}, ballMaterial, ballMesh);
		scene->add(balls[i]);
	}
	for (std::size_t i = 0; i < sizeOld; ++i) {
		balls[i]->transform.position = starmap->balls[i].position;
		balls[i]->transform.scale = libv::vec3f{starmap->balls[i].size};
	}

	pickingBall->transform.position = starmap->pickingBall.position;
	movingBall->transform.position = starmap->movingBall.position;

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

SceneStarmap::SceneStarmap(libv::Nexus& nexus, Starmap_sp starmap, TimeController_sp timeController) :
	starmap(std::move(starmap)),
	timeController(std::move(timeController)),
	starmapController(nexus) {
}

void SceneStarmap::controls(ControllerContext& ctx) {
	ctx.attached<StarmapController>(&starmapController);
	ctx.attached<libv::sun::BaseCameraOrbit>(&canvas->camera->tmpCameraPlayer);
}

[[nodiscard]] libv::ui::Component SceneStarmap::createScene(libv::Nexus& nexus) {
	auto layers = libv::ui::PanelAnchor::n("layers");

	canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasStarmap>>("canvas", nexus, starmap, timeController);
	canvas.z_index_offset(-100);
	starmapController.init(starmap.get(), &canvas.object()); // Sadness (2 step init)

	return layers;
}

// =================================================================================================

libv::ui::Component createSceneStarmap(libv::Nexus& nexus, Starmap_sp starmap, TimeController_sp timeController) {
	auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Starmap", "Controls");
	return star::createControlledScene<SceneStarmap>(controls, nexus, std::move(starmap), std::move(timeController))(nexus);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
