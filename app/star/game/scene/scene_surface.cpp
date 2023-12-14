// Project: libv, File: app/star/game/scene/scene_surface.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component_system/bean.hpp>
#include <libv/ui/component_system/create_scene.hpp>
#include <libv/ui/component_system/switch_scene.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/utility/nexus_fwd.hpp>
// pro
//#include <star/game/scene/surface/camera.hpp>
//#include <star/game/config/client_config.hpp>



#include <libv/ctrl/controls.hpp>

#include <libv/sun/camera.hpp>

// #include <libv/gl/gl.hpp>
// #include <libv/glr/attribute.hpp>
// #include <libv/glr/layout_std140.hpp>
// #include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
// #include <libv/glr/uniform.hpp>
// #include <libv/glr/uniform_buffer.hpp>
// #include <libv/meta/reflection_access.hpp>
// #include <libv/rev/renderer/renderer_editor_grid.hpp>
// #include <libv/rev/resource_manager.hpp>
// #include <libv/rev/settings.hpp>
// #include <libv/rev/shader/block/matrices.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/ui.hpp>



#include <string>

#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <star/log.hpp>
#include <star/game/control/time_control.hpp>


// <<< Improve the following re includes:
// Remove or get a better wrapper / glue layer:
#include <libv/re/core/canvas.hpp>
#include <libv/re/node/camera.hpp>
#include <libv/re/r.hpp>
// Ok:
#include <libv/re/core/scene.hpp>
#include <libv/re/node/skybox.hpp>
#include <libv/re/resource/texture_loader.hpp>

#include <libv/re/node/editor_grid.hpp>
#include <libv/re/node/object.hpp>
#include <libv/re/mesh/mesh_sphere.hpp>
#include <libv/re/material/material_solid.hpp>

// Testing / debugging only:
#include <libv/ui/component/input_field.hpp>
#include <libv/ctrl/feature_register.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

// !!! generalized wrapper with a built in libv.re scene

// struct SurfaceScene {
// 	static constexpr std::string_view sceneName = "Surface Scene";
//
// 	libv::re::Scene_ptr scene = libv::re::Scene::create(sceneName);
//
// 	static void setupCanvasControls(libv::ctrl::Controls& controls);
// 	static void setupSceneControls(libv::ctrl::Controls& controls);
// 	void updateAsync();
// 	void update();
// 	void render();
//
// 	// void enableCanvasControls(libv::ctrl::Controls& controls);
// 	// void disableCanvasControls(libv::ctrl::Controls& controls);
// };

// -------------------------------------------------------------------------------------------------

struct Surface {
	libv::re::Scene_ptr scene;
	std::vector<libv::re::Node_ptr> balls;
	// Terrain, Buildings, Enviroment, etc

	explicit Surface(const libv::re::Scene_ptr& scene) :
		scene(scene) {
		const auto meshBall = libv::re::MeshSphere::create(12);
		const auto materialBall = libv::re::MaterialSolid::create(libv::vec4f{0.5f, 0.25f, 0.25f, 1.f});
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{libv::vec3f{+5, 0, 0}, libv::quatf::identity(), libv::vec3f::one(1.f)}, materialBall, meshBall));
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{libv::vec3f{ 0, 0, 0}, libv::quatf::identity(), libv::vec3f::one(3.f)}, materialBall, meshBall));
		balls.emplace_back(libv::re::Object::create(libv::re::Transform{libv::vec3f{-5, 0, 0}, libv::quatf::identity(), libv::vec3f::one(1.f)}, materialBall, meshBall));

		for (const auto& ball : balls)
			scene->add(ball);
	}
	~Surface() {
		for (const auto& ball : balls)
			scene->remove(ball);
	}
};

// -------------------------------------------------------------------------------------------------

struct CanvasSurface : libv::ui::CanvasBase {
public:
	// Visuals
	libv::re::Scene_ptr scene = libv::re::Scene::create("Surface Scene");
	libv::re::Camera_ptr camera = libv::re::Camera::create();
	// CameraControl cameraControl;

	// ...
	Surface surface{scene};
	TimeControl timeControl;

	// Render
	libv::re::Skybox_ptr skybox;

	// UI
	libv::ui::Label parentLabel;

	// State
	bool rotate = false;

public:
	explicit CanvasSurface(libv::Nexus& nexus, libv::ctrl::Controls& controls, libv::ui::Label parentLabel) :
		parentLabel(std::move(parentLabel)) {
		// controls(controls) {
		(void) nexus;
		(void) controls;

		// TODO P2: Proper camera control management
		// TODO P2: Proper control management
		camera->tmpCameraPlayer.look_at({-3.f, -6.f, 4.0f}, {0, 0, 0});
		camera->tmpCameraPlayer.far(50.0f); // With inf projection far is only used for linearized visualization (and camera controls)
		camera->tmpCameraPlayer.near(0.5f);
		// CameraPerspective camera(pos2, fov, aspectRatio, near, far);
		// ControllerOrbit controller(camera, canvas);

		scene->add(libv::re::EditorGrid::create());
		skybox = libv::re::Skybox::create(
				// libv::r.texture.load_async("texture/sky/debug_x_front_uv_cube_srgb.dds")));
				// libv::r.texture.load_async("texture/sky/ambience_morning_green_eqrt_srgb.dds"), libv::re::SkyboxType::equirectangular));
				// libv::r.texture.load_async("texture/sky/ambience_morning_green_eqrt_hdr.dds"), libv::re::SkyboxType::equirectangular));
				// libv::r.texture.load_async("texture/sky/ambience_morning_green_cube_srgb.dds"), libv::re::SkyboxType::cubemapZXY));
				// libv::r.texture.load_async("texture/sky/ambience_morning_green_cube_hdr.dds"), libv::re::SkyboxType::cubemapZXY));
				// libv::r.texture.load_async("texture/sky/snowy_river_valley_eqrt_srgb.dds"), libv::re::SkyboxType::equirectangular));
				// libv::r.texture.load_async("texture/sky/snowy_river_valley_eqrt_hdr.dds"), libv::re::SkyboxType::equirectangular));
				// libv::r.texture.load_async("texture/sky/snowy_river_valley_cube_srgb.dds"), libv::re::SkyboxType::cubemapZXY));
				libv::r.texture.load_async("texture/sky/snowy_river_valley_cube_hdr.dds"), libv::re::SkyboxType::cubemapZXY);
		scene->add(skybox);

		//Debug gizmo: Coordinates text: renderer.text.add_debug_coordinates
		//Debug gizmo: Camera orbit point: renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);
		//Debug gizmo: Camera orientation gizmo in top right
		// 	const auto orientation_gizmo_size = 64.f; // The axes of the gizmo will be half of this size
		// 	const auto orientation_gizmo_margin = 4.f;
		//
		// 	glr.projection = libv::mat4f::ortho(
		// 			-canvas_size + orientation_gizmo_size * 0.5f + orientation_gizmo_margin,
		// 			canvas_size,
		// 			-orientation_gizmo_size,
		// 			+orientation_gizmo_size);
		// 	glr.view = camera.orientation_view().translate(-1, 0, 0);
		// 	glr.model.scale(orientation_gizmo_size * 0.5f);
		//
		// 	renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);

		// controls.context_enter<libv::sun::BaseCameraOrbit>(&camera);
	}
	~CanvasSurface() {
		// controls.context_leave_if_matches<libv::sun::BaseCameraOrbit>(&camera);
	}

public:
	void enableControls(libv::ctrl::Controls& controls) {
		// (void) controls;
		// TODO P1: app.space: Should not store it (?) only to bypass controls invalidation issue
		// libv::ctrl::Controls& controls;
		// std::optional<ControlVar> controlVar

	// 	if (!controlVar)
	// 		// TODO P1: app.space:
	// 		controlVar.emplace(
	// 				*this,
	// 				playout,
	// 				playout.simulation->universe->galaxy,
	// //				player,
	// 				game_session.player
	// 		);
	//
	// 	controls.context_enter<CanvasControl>(&*controlVar);
	// 	controls.context_enter<libv::sun::BaseCameraOrbit>(&camera);
		controls.context_enter<libv::sun::BaseCameraOrbit>(&camera->tmpCameraPlayer);
		controls.context_enter<TimeControl>(&timeControl);
	}

	void disableControls(libv::ctrl::Controls& controls) {
		controls.context_leave_if_matches<libv::sun::BaseCameraOrbit>(&camera->tmpCameraPlayer);
		controls.context_leave_if_matches<TimeControl>(&timeControl);
		// controls.context_leave_if_matches<CanvasControl>(&*controlVar);

		// // TODO P2: Controls guard objects for safety
		// auto& controls = *nexus.object_view_get<libv::ctrl::Controls>();
		// controls.context_leave<libv::sun::BaseCameraOrbit>();
		// controls.context_leave<SandboxState>();
	}

private:
	virtual void attach() override {
		focus();
	}
	virtual void update(libv::ui::time_duration timeDelta) override {
		timeControl.update(timeDelta);

		// skybox->horizonOffset(0.1f * std::sin(timeControl.timeSim.count()));
		// skybox->intensity(2.0f + 2.0f * std::sin(timeControl.timeSim.count() * 0.5f));
		// skybox->rotateZ(std::sin(timeControl.timeSim.count() * 0.1f));

		parentLabel.text(fmt::format("Canvas: paused {}", timeControl.paused));
		// if (rotate)
		// 	camera->tmpCameraPlayer.orbit_yaw(static_cast<float>(timeDelta.count()) * libv::tau * 0.01f);
	}
	virtual void render(libv::glr::Queue& glr) override {
		// NOTE: screen_picker update has to be placed around render, as canvas_size is only set after layout
		// TODO P1: Move screen_picker update into a layout post hook so mouse event use the update value
		// screen_picker = camera.picker(canvas_size);

		glr.callback([&](libv::GL& gl) {
			scene->render(
				gl,
				*camera,
				(canvas_position + 0.5f).cast<int32_t>(),
				(canvas_size + 0.5f).cast<int32_t>(),
				calculate_local_mouse_coord(),
				timeControl.timeSim.count(),
				timeControl.timeReal().count());
		});
	}
};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::ui::Component createInfoBar(libv::Nexus& nexus) {
	(void) nexus;
	// auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");

	// auto layers = libv::ui::PanelAnchor::n("layers");
	// auto label1 = libv::ui::Label::s("surface.menu.label");

	auto panel = libv::ui::PanelLine::ns("info-bar", "game.info-bar.panel");
	{
		auto leftLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		{
			auto lbl0 = leftLine.add_sa<libv::ui::Label>("game.info-bar.label", "Left");
			// !!! Toggle buttons and button groups
			auto btnPause = leftLine.add_sa<libv::ui::Button>("game.info-bar.time.pause", "][");
			// btnPause.event().submit.connect([] {
			// 	nexus.broadcast_channel(gameChannel, RequestTimeControl{0});
			// });
			auto btnX1 = leftLine.add_sa<libv::ui::Button>("game.info-bar.time.speed", "x1");
			// btnX1.event().submit.connect([] {
			// 	nexus.broadcast_channel(gameChannel, RequestTimeControl{1});
			// });
			auto btnX2 = leftLine.add_sa<libv::ui::Button>("game.info-bar.time.speed", "x2");
			// btnX2.event().submit.connect([] {
			// 	nexus.broadcast_channel(gameChannel, RequestTimeControl{2});
			// });
			auto btnX4 = leftLine.add_sa<libv::ui::Button>("game.info-bar.time.speed", "x4");
			// btnX4.event().submit.connect([] {
			// 	nexus.broadcast_channel(gameChannel, RequestTimeControl{4});
			// });
		}
		auto gap = panel.add_s<libv::ui::Gap>("game.info-bar.gap");
		auto rightLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		{
			auto lbl0 = rightLine.add_sa<libv::ui::Label>("game.info-bar.label", "Right");

		}
	}
	// // auto label1 = line.add_s<libv::ui::Label>("surface.menu.label");
	// line.add(label1);
	// auto input = line.add_s<libv::ui::InputField>("surface.menu.label");
	// input.text("Test input field");
	// auto btn = line.add_sa<libv::ui::Button>("surface.menu.button", "Exit to Main Menu");
	// btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
	// 	libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
	// });

	return panel;
}

struct SceneSurface {
	std::string some_data;

	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) const {
		auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");

		auto layers = libv::ui::PanelAnchor::n("layers");
		auto label1 = libv::ui::Label::s("surface.menu.label");

		auto canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas", nexus, controls, label1);
		canvas.z_index_offset(-100);
		canvas.event().focus.connect([&controls](libv::ui::CanvasAdaptorT<CanvasSurface>& self, const libv::ui::EventFocus& event) {
			log_star.trace("Surface Canvas Focus: {}", event.gain());

			if (event.gain())
				self.object().enableControls(controls);
			else
				self.object().disableControls(controls);
		});

		layers.add(createInfoBar(nexus));
		auto line = layers.add_ns<libv::ui::PanelLine>("menu-line", "surface.menu.line");
		auto noop = line.add_sa<libv::ui::Button>("surface.menu.button", "Does nothing");
		auto label0 = line.add_sa<libv::ui::Label>("surface.menu.label", "Surface: " + some_data);
		// auto label1 = line.add_s<libv::ui::Label>("surface.menu.label");
		line.add(label1);
		auto input = line.add_s<libv::ui::InputField>("surface.menu.label");
		input.text("Test input field");
		auto btn = line.add_sa<libv::ui::Button>("surface.menu.button", "Exit to Main Menu");
		btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
			libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
		});

		return layers;
	}
};

libv::ui::Component createSceneSurface(libv::Nexus& nexus) {
	return libv::ui::createScene<SceneSurface>("Some data passed to Surface scene")(nexus);
}

// struct SceneInGame {
// 	std::string some_data;
//	TimeControl timeControl;
//
// 	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) const {
// 		auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");
//
// 		auto layers = libv::ui::PanelAnchor::n("layers");
//
// 		auto canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas", nexus, controls);
// 		canvas.z_index_offset(-100);
// 		canvas.event().focus.connect([&controls](libv::ui::CanvasAdaptorT<CanvasSurface>& self, const libv::ui::EventFocus& event) {
// 			log_star.trace("Surface Canvas Focus: {}", event.gain());
//
// 			if (event.gain())
// 				self.object().enableControls(controls);
// 			else
// 				self.object().disableControls(controls);
// 		});
//
// 		auto line = layers.add_ns<libv::ui::PanelLine>("menu-line", "surface.menu.line");
// 		auto label = line.add_sa<libv::ui::Label>("surface.menu.label", "Surface: " + some_data);
// 		auto input = line.add_s<libv::ui::InputField>("surface.menu.label");
// 		input.text("Test input field");
// 		auto btn = line.add_sa<libv::ui::Button>("surface.menu.button", "Exit to Main Menu");
// 		btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
// 			libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
// 		});
//
// 		return layers;
// 	}
// };
//
// libv::ui::Component createSceneInGame(libv::Nexus& nexus) {
// 	return libv::ui::createScene<SceneInGame>("Some data passed to InGame scene")(nexus);
// }

// -------------------------------------------------------------------------------------------------

} // namespace star
