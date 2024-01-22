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
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/toggle_button.hpp>
#include <libv/ui/component_system/bean.hpp>
#include <libv/ui/component_system/switch_scene.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/screen_picker.hpp>

#include <cmath>
#include <star/log.hpp>

#include <star/game/control/requests.hpp>
#include <star/game/control/surface_controller.hpp>
#include <star/game/control/time_controller.hpp>
#include <star/game/scene/controlled_scene.hpp>
#include <star/game/scene/scenes.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

Surface::Surface() {
	sky = Sky("texture/sky/ambience_morning_green_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY, 1.f, libv::tau_f * 0.5f);
	movingBall = Ball(libv::vec3f{15, 0, 0}, 1.f);
	pickingBall = Ball(libv::vec3f{0, 0, 0}, 0.1f);
	balls.emplace_back(libv::vec3f{+10, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{+5, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{ 0, 0, 0}, 3.f);
	balls.emplace_back(libv::vec3f{-5, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{-10, 0, 0}, 1.f);
}

void Surface::update(double timeSim) {
	// TODO P1: Correct time type, Use delta time, But have access to the abs sim/frame/real time
	movingBall.position.x = std::cos(static_cast<float>(timeSim)) * 15.f;
	movingBall.position.y = std::sin(static_cast<float>(timeSim)) * 15.f;
}

// =================================================================================================

CanvasSurface::CanvasSurface(libv::Nexus& nexus, Surface& surface, TimeController& timeController) :
	scene(libv::re::Scene::create("Surface Scene")),
	camera(libv::re::Camera::create()),
	surface(surface),
	timeController(timeController) {
	(void) nexus;

	// TODO P2: Proper camera
	camera->tmpCameraPlayer.look_at({-3.f, -6.f, 4.0f}, {0, 0, 0});
	camera->tmpCameraPlayer.fov_y(libv::deg_to_rad(75.0f));
	camera->tmpCameraPlayer.far(50.0f); // With inf projection far is only used for linearized visualization (and camera controls)
	camera->tmpCameraPlayer.near(0.5f);

	scene->add(editorGrid = libv::re::EditorGrid::create());

	ballMesh = libv::re::MeshSphere::create(12);
	ballMaterial = libv::re::MaterialSolid::create(libv::vec4f{0.5f, 0.25f, 0.25f, 1.f});

	skybox = libv::re::Skybox::create(libv::r.texture.load_async(surface.sky.texture), surface.sky.type);
	skybox->intensity(surface.sky.intensity);
	skybox->rotateZ(surface.sky.rotateZ);
	skybox->horizonOffset(surface.sky.horizonOffset);
	for (const auto& ball : surface.balls)
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{ball.position, libv::quatf::identity(), libv::vec3f::one(ball.size)}, ballMaterial, ballMesh));
	scene->add(movingBall = libv::re::Object::create(libv::re::Transform{surface.movingBall.position, libv::quatf::identity(), libv::vec3f::one(surface.movingBall.size)}, ballMaterial, ballMesh));
	scene->add(pickingBall = libv::re::Object::create(libv::re::Transform{surface.pickingBall.position, libv::quatf::identity(), libv::vec3f::one(surface.pickingBall.size)}, ballMaterial, ballMesh));

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
	surface.pickingBall.position = intersection ? *intersection : libv::vec3f{0};

	// Update render state
	// TODO P3: Model-View state sync utilities update(scene, balls, surface.balls, createFn, updateFn);
	const auto sizeOld = balls.size();
	const auto sizeNew = surface.balls.size();
	for (std::size_t i = sizeNew; i < sizeOld; ++i)
		scene->remove(balls[i]);
	balls.resize(sizeNew);
	for (std::size_t i = sizeOld; i < sizeNew; ++i) {
		balls[i] = libv::re::Object::create(libv::re::Transform{surface.balls[i].position, libv::quatf::identity(), libv::vec3f::one(surface.balls[i].size)}, ballMaterial, ballMesh);
		scene->add(balls[i]);
	}
	for (std::size_t i = 0; i < sizeOld; ++i) {
		balls[i]->transform.position = surface.balls[i].position;
		balls[i]->transform.scale = libv::vec3f{surface.balls[i].size};
	}

	pickingBall->transform.position = surface.pickingBall.position;
	movingBall->transform.position = surface.movingBall.position;

	// Execute render
	scene->render(
			gl,
			*camera,
			(canvas_position + 0.5f).cast<int32_t>(),
			(canvas_size + 0.5f).cast<int32_t>(),
			mouseCoord,
			timeController.timeSim.count(),
			timeController.timeReal().count());
}

// =================================================================================================

SceneSurface::SceneSurface(libv::Nexus& nexus) :
	timeController(nexus),
	surfaceController(nexus) {
}

void SceneSurface::controls(ControllerContext& ctx) {
	ctx.attached<TimeController>(&timeController);
	ctx.attached<SurfaceController>(&surfaceController);
	ctx.attached<libv::sun::BaseCameraOrbit>(&canvas->camera->tmpCameraPlayer);
}

void SceneSurface::update(libv::time_point time, libv::time_duration_d delta) {
	(void) time;
	timeController.update(delta);
	surface.update(timeController.timeSim.count());
}

[[nodiscard]] libv::ui::Component SceneSurface::createScene(libv::Nexus& nexus) {
	auto layers = libv::ui::PanelAnchor::n("layers");
	const void* slot = layers.ptr();
	layers.event().detach.connect_system([nexus, slot] mutable {
		nexus.disconnect_slot_all(slot);
	});

	canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas", nexus, surface, timeController);
	canvas.z_index_offset(-100);
	surfaceController.init(&surface, &canvas.object()); // Sadness

	layers.add(createInfoBar(nexus));
	layers.add(createMenu(nexus));

	return layers;
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::ui::Component SceneSurface::createInfoBar(libv::Nexus& nexus) {
	auto panel = libv::ui::PanelLine::ns("info-bar", "game.info-bar.panel");
	const void* slot = panel.ptr();
	panel.event().detach.connect_system([nexus, slot] mutable {
		nexus.disconnect_slot_all(slot);
	});

	const auto timeEvent = [&]<typename Req, typename Cmd>(libv::ui::ToggleButton& btn, Req req, Cmd cmd) {
		btn.event().submit.connect([nexus, this, req](const libv::ui::EventSubmit& event) {
			timeController.request(req);
			event.reject_submit();
		});
		nexus.connect_channel_and_call<Cmd>(&timeController, slot, [btn, cmd](const Cmd& event) mutable {
			btn.select_silent(event == cmd);
		}, timeController.currentCommandTimeSpeed());
	};

	{
		auto leftLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		{
			// auto lbl0 = leftLine.add_sa<libv::ui::Label>("game.info-bar.label", "Left");

			auto btnPause = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.pause", "[II]", "II");
			auto btnSpeed1 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[1]", "1");
			auto btnSpeed2 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[2]", "2");
			auto btnSpeed3 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[4]", "4");
			timeEvent(btnPause, RequestTimeTogglePause{}, CommandTimeSpeed{0});
			timeEvent(btnSpeed1, RequestTimeSpeed{1}, CommandTimeSpeed{1});
			timeEvent(btnSpeed2, RequestTimeSpeed{2}, CommandTimeSpeed{2});
			timeEvent(btnSpeed3, RequestTimeSpeed{3}, CommandTimeSpeed{3});
		}
		auto gap = panel.add_s<libv::ui::Gap>("game.info-bar.gap");
		// auto rightLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		// {
		//	auto lbl0 = rightLine.add_sa<libv::ui::Label>("game.info-bar.label", "Right");
		// }
	}

	return panel;
}

[[nodiscard]] libv::ui::PanelLine SceneSurface::createMenu(libv::Nexus& nexus) {
	auto line = libv::ui::PanelLine::ns("menu-line", "surface.menu.line");
	auto input = line.add_s<libv::ui::InputField>("surface.menu.label");
	input.text("Test input field");
	auto btnExit = line.add_sa<libv::ui::Button>("surface.menu.button", "Exit to Main Menu");
	btnExit.event().submit.connect([nexus](libv::ui::Button& source) mutable {
		libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
	});
	return line;
}

// =================================================================================================

libv::ui::Component createSceneSurface(libv::Nexus& nexus) {
	auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");
	return star::createControlledScene<SceneSurface>(controls, nexus)(nexus);
}

// -------------------------------------------------------------------------------------------------

void registerSurfaceControls(libv::ctrl::Controls& controls) {
	TimeController::register_controls(controls);
	TimeController::bind_default_controls(controls);
	SurfaceController::register_controls(controls);
	SurfaceController::bind_default_controls(controls);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
