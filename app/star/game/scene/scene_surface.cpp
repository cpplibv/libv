// Project: libv, File: app/star/game/scene/scene_surface.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component_system/create_scene.hpp>
#include <libv/ui/component_system/switch_scene.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/utility/nexus_fwd.hpp>
// pro
//#include <star/game/scene/surface/camera.hpp>
#include <star/game/scene/bean.hpp>
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

#include "libv/ui/parse/parse_size.hpp"
//


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

struct CanvasSurface : libv::ui::CanvasBase {
public:
//	GameSession& game_session;

	// Visuals
	libv::re::Scene_ptr scene = libv::re::Scene::create("Surface Scene");
	libv::re::Camera_ptr camera = libv::re::Camera::create();
	libv::re::Canvas_ptr canvas = libv::re::Canvas::create();

	// Simulation
	bool paused = false;
	bool unpauseNextFrame = false;
	bool slowMode = false;
	int slowModeCounter = 0;
	libv::time_point timeRealStart = libv::clock::now();
	libv::time_point timeRealFrame = timeRealStart;
	libv::time_duration_d timeSim{0};

	// Render
	libv::re::Skybox_ptr skybox;

	// State
	bool rotate = false;

public:
	explicit CanvasSurface(libv::Nexus& nexus, libv::ctrl::Controls& controls) {
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

		// Simulation controls:
		// controls.feature_action<SandboxState>("sim.pause", [](libv::ctrl::arg_action, SandboxState& state) {
		// 	state.paused = !state.paused;
		// });
		// controls.bind("sim.pause", "space");
		// controls.feature_binary<SandboxState>("sim.debug.slow-mode", [](libv::ctrl::arg_binary event, SandboxState& state) {
		// 	state.slowMode = event.value;
		// 	state.slowModeCounter = 0;
		// });

		//Debug gizmo: coordinates text: renderer.text.add_debug_coordinates
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
		(void) controls;
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

	}

	void disableControls(libv::ctrl::Controls& controls) {
		(void) controls;
		// controls.context_leave_if_matches<libv::sun::BaseCameraOrbit>(&camera);
		// controls.context_leave_if_matches<CanvasControl>(&*controlVar);

		// // TODO P2: Controls guard objects for safety
		// auto& controls = *nexus.object_view_get<libv::ctrl::Controls>();
		// controls.context_leave<libv::sun::BaseCameraOrbit>();
		// controls.context_leave<SandboxState>();
	}

private:
	virtual void attach() override {
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
		scene->add(libv::re::Object::create(libv::re::Transform{libv::vec3f{}, libv::quatf::identity(), libv::vec3f::one(3.f)},
				libv::re::MaterialSolid::create(libv::vec4f{0.f, 0.25, 0.25f, 1.f}),
				libv::re::MeshSphere::create(12)));

		//Debug gizmo: coordinates text
		//	renderer.text.add_debug_coordinates_if_nothing_else();

		//Debug gizmo: Camera orbit point
		//	renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);

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

		focus();
	}
	virtual void update(libv::ui::time_duration timeDelta) override {
		this->timeRealFrame = ui().state.time_frame();

		if ((!paused && !slowMode) || unpauseNextFrame || (slowMode && (++slowModeCounter % 10) == 0)) {
			unpauseNextFrame = false;
			timeSim += timeDelta;
		}

		// skybox->horizonOffset(0.1f * std::sin(timeControl.timeSim.count()));
		// skybox->intensity(2.0f + 2.0f * std::sin(timeControl.timeSim.count() * 0.5f));
		// skybox->rotateZ(std::sin(timeControl.timeSim.count() * 0.1f));

		// if (rotate)
		// 	camera->tmpCameraPlayer.orbit_yaw(static_cast<float>(timeDelta.count()) * libv::tau * 0.01f);
	}
	virtual void render(libv::glr::Queue& glr) override {
		// NOTE: screen_picker update has to be placed around render, as canvas_size is only set after layout
		// TODO P1: Move screen_picker update into a layout post hook so mouse event use the update value
		// screen_picker = camera.picker(canvas_size);

		glr.callback([&](libv::GL& gl) {
			canvas->position(canvas_position.cast<int32_t>());
			canvas->size(canvas_size.cast<int32_t>());
			scene->render(libv::r, gl, *camera, canvas, timeSim.count(), (timeRealFrame - timeRealStart).count());
			// calculate_local_mouse_coord();
			// scene.render(gl, canvas_position, canvas_size);
		});
	}
};

// -------------------------------------------------------------------------------------------------

struct SceneSurface {
	std::string some_data;

	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) const {
		auto& controls = requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls");

		auto layers = libv::ui::PanelAnchor::n("layers");

		auto canvas = layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas", nexus, controls);
		canvas.z_index_offset(-100);
		canvas.event().focus.connect([](libv::ui::CanvasAdaptorT<CanvasSurface>& self, const libv::ui::EventFocus& event) {
			log_star.trace("Surface Canvas Focus: {}", event.gain());

			(void) self;
			// if (event.gain())
			// 	self.object().enableControls(controls);
			// else
			// 	self.object().disableControls(controls);
		});

		auto line = layers.add_ns<libv::ui::PanelLine>("menu-line", "surface.menu.line");
		auto label = line.add_sa<libv::ui::Label>("surface.menu.label", "Surface: " + some_data);
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

// -------------------------------------------------------------------------------------------------

} // namespace star
