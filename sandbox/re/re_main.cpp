// Project: libv.re, File: sandbox/re/re_main.cpp

#include <libv/ctrl/binding_register.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/frame/icon_set.hpp>
#include <libv/glr/queue.hpp>
#include <libv/log/log.hpp>
#include <libv/math/exp_moving_avg.hpp>
#include <libv/re/core/engine.hpp>
#include <libv/re/settings.hpp>
// #include <libv/re/uniform/fullscreen_debug_mode.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/sun/camera.hpp>
#include <libv/sun/camera_control.hpp>
#include <libv/sun/ui/overlay_resource_error.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/scene_container.hpp>
#include <libv/ui/component_system/canvas.hpp>
#include <libv/ui/component_system/create_scene.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/timer.hpp>
#include <wish/resource_mapping.hpp>

#include <iostream>

#include "log.hpp"
#include "scene_alpha.hpp"

#include "libv/re/core/render_library.hpp"
// #include <libv/arg/arg.hpp>
// #include <libv/ctrl/controls.hpp>
// // #include <libv/rev/effect/particle.hpp>
// // #include <libv/rev/effect/particle_system.hpp>
// // #include <libv/rev/post_processing.hpp>
// // #include <libv/rev/render_target.hpp>
// // #include <libv/rev/resource/resource_manager.hpp>
// // #include <libv/rev/resource/shader.hpp>
// #include <libv/rev/effect/particle.hpp>
// // #include <libv/rev/engine/canvas.hpp>
// #include <libv/rev/engine/render_pass.hpp>
// #include <libv/rev/newapi/scene.hpp>
// // #include <libv/rev/material/material_editor_grid.hpp>
// #include <libv/rev/material_adv/material_editor_grid.hpp>


// -------------------------------------------------------------------------------------------------

static constexpr libv::vec2i window_size = {1680, 1050};

bool enablePostProcessing = true;

// -------------------------------------------------------------------------------------------------

class SandboxCanvas : public libv::ui::CanvasBase {
	libv::Nexus nexus;
	std::optional<SandboxState>& state;

public:
	explicit SandboxCanvas(libv::Nexus& nexus, std::optional<SandboxState>& state) :
		nexus(nexus), state(state) { }

	virtual void update(libv::time_duration deltaTime) override {
		(void) deltaTime;
	}
	virtual void create(libv::glr::Queue& glr) override {
		glr.callback([&](libv::GL& gl) {
			state->create(gl);
		});
	}
	virtual void destroy(libv::glr::Queue& glr) override {
		glr.callback([&](libv::GL& gl) {
			state->destroy(gl);
		});
	}
	virtual void render(libv::glr::Queue& glr) override {
		(void) glr;
		// canvas.resize(canvas_size.cast<int32_t>());
		glr.callback([&](libv::GL& gl) {
			// calculate_local_mouse_coord();
			state->render(gl, canvas_position, canvas_size);
		});
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class ReAdaptorScene {
private:
	std::optional<libv::re::RenderLibrary> re;
	std::optional<T> state;

public:
	ReAdaptorScene() {}

public:
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) {
		auto canvas = libv::ui::createCanvas<SandboxCanvas>("canvas", nexus, state);

		canvas.event().global_before_update([this](const libv::ui::EventBeforeUpdate& event) mutable {
			if (state) {
				state->async_update(event.frame_time, event.delta_time);
			}
		});
		canvas.event().before_create.connect_system([this, nexus = nexus](const libv::ui::EventCanvasBeforeCreate& event) mutable {
			re.emplace(libv::re::Settings{}, nexus, event.gl);
			state.emplace(nexus);
		});
		// canvas.event().before_render.connect_system([this](const libv::ui::EventCanvasBeforeRender& event) mutable {
		// 	engine.prepare_render(event.gl);
		// });
		canvas.event().after_destroy.connect_system([this](const libv::ui::EventCanvasAfterDestroy& event) mutable {
			(void) event;
			state.reset();
			re.reset();
		});

		return canvas;
	}
};

// -------------------------------------------------------------------------------------------------

class Sandbox : public libv::Frame {
	libv::ctrl::Controls controls;

	libv::ui::UI ui{[] {
		libv::ui::Settings settings;
		settings.use_libv_res_resource_path = true;
		settings.res_font.base_path = "res/font/";
		settings.res_font.restrict_under_base = true;
		settings.res_shader.base_path = "res/shader/";
		settings.res_shader.restrict_under_base = true;
		settings.res_texture.base_path = "res/texture/";
		settings.res_texture.restrict_under_base = true;
		settings.track_style_scripts = true;
		// settings.style_scripts = { "style.lua" };
		return settings;
	}()};

public:
	Sandbox() : Frame("RE Sandbox", window_size) {
		register_nexus(ui.nexus());

		// --- Frame Initialization ---
		setOpenGLVersion(4, 5);
		setOpenGLProfile(OpenGLProfile::core);
		setPosition(FramePosition::os_default);
		setIcon(libv::frame::icon_set_iris_cyan());
		setOpenGLSamples(OpenGLSamples{0});
		onKey.output([this](const libv::input::EventKey& event) {
			if (event.action != libv::input::Action::press)
				return;

			if (event.keycode == libv::input::Keycode::Escape)
				closeDefault();

			if (event.keycode == libv::input::Keycode::P)
				enablePostProcessing = !enablePostProcessing;

			if (event.keycode == libv::input::Keycode::O)
				setSwapInterval(getSwapInterval() ? 0 : 1);

			// if (event.keycode == libv::input::Keycode::Backtick)
			// 	fullscreenDebugMode = libv::re::FullscreenDebugMode::normal;
			// if (event.keycode == libv::input::Keycode::Num1)
			// 	fullscreenDebugMode = libv::re::FullscreenDebugMode::depth;
			// if (event.keycode == libv::input::Keycode::Num2)
			// 	fullscreenDebugMode = libv::re::FullscreenDebugMode::depthLinear;
			// if (event.keycode == libv::input::Keycode::Num3)
			// 	fullscreenDebugMode = libv::re::FullscreenDebugMode::depthLinearFract;
		});

		// --- UI Initialization ---
		ui.attach(*this);
		ui.load_style_script_file(std::string(libv::res::resource_path("res/style.lua")));
		ui.add(init_ui());
		ui.add(overlay_version());
		ui.add(overlay_fps());
		ui.add(libv::sun::overlay_resource_error(true));

		// --- Controls Initialization ---
		libv::sun::CameraControl::register_controls(controls);
		libv::sun::CameraControl::bind_default_controls(controls, 1);
		// CanvasControl::register_controls(controls);
		// CanvasControl::bind_default_controls(controls);
		// SceneMainControl::register_controls(controls);
		// SceneMainControl::bind_default_controls(controls);
		controls.attach(*this);
	}

	~Sandbox() {
		unregister_nexus(ui.nexus());
	}

private:
	void register_nexus(libv::Nexus& nexus) {
		nexus.object_view_set(&controls);
		// ...
	}

	void unregister_nexus(libv::Nexus& nexus) {
		// TODO P2: Nexus guard objects for safety
		nexus.object_view_remove<libv::ctrl::Controls>();
		// ...
	}

	[[nodiscard]] libv::ui::Component init_ui() {
		return libv::ui::SceneContainer::na(
				"main-sc", "main", libv::ui::createScene<ReAdaptorScene<SandboxState>>()(ui.nexus()));
	}

	[[nodiscard]] libv::ui::Component overlay_version() {
		return libv::ui::Label::nsa("version", "overlay.version", fmt::format("v0"));
	}

	[[nodiscard]] libv::ui::Component overlay_fps() {
		static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{200};

		// TODO P4: Implement proper frame statistics
		//			min, max, avg, window min, window max, window avg, 1% low
		//  		(including integration with UI and Game state statistics)
		auto fps = libv::ui::Label::ns("fps", "overlay.fps");
		ui.event().global_before_update([fps, frameTimer = libv::Timer{}, updateTimer = libv::Timer{},
												avg = libv::exp_moving_avgf(60, 1, 0.8f)] mutable {
			const auto dt = frameTimer.timef_s().count();
			avg.add(1.0f / dt, dt);
			if (updateTimer.elapsed() < fpsUpdatePeriod)
				return;
			updateTimer.reset();
			fps.text(fmt::format("{:6.1f} FPS", avg.value()));
		});
		return fps;
	}
};

// =================================================================================================

int main(int argc, const char* const* argv) {
	std::cout << libv::logger_stream;
	libv::logger_stream.setFormat("{severity} {thread_id:2} {module}: {message}, {file}:{line}\n");

	wish::change_current_path(argc, argv);
#if not WISH_BUILD_PACKAGE
	libv::res::init_resource_mapping(wish::resource_mapping());
#endif

	Sandbox sandbox;
	sandbox.show();
	sandbox.join();

	return EXIT_SUCCESS;
}

// =================================================================================================
//
// inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};
// struct UserParticleLibrary : libv::re::ParticleLibrary {
// 	// using libv::re::ParticleLibrary::ParticleLibrary;
// 	libv::re::ParticleEffectDefinition smallExplosion = load("smallExplosion");
// 	libv::re::ParticleEffectDefinition rain = load("rain");
// 	libv::re::ParticleEffectDefinition rainHeavy = load("rainHeavy");
// 	libv::re::ParticleEffectDefinition smokeStack = load("smokeStack");
// 	libv::re::ParticleEffectDefinition fountain = load("fountain");
//
// 	UserParticleLibrary() {
// 		fountain.addModule<libv::re::ModuleEmitter>(true, libv::vec3f{0, 0, 0}, libv::vec3f{5.f, 5.f, 40.f}, 5.f);
// 		fountain.addModule<libv::re::ModuleGravity>();
// 	}
// };
// UserParticleLibrary particleLibrary{engine};
// 	// libv::re::ParticleSystem particle_rain;
// 	// libv::re::ParticleSystem particle_stars;
// 	// libv::re::ParticleSystem particle_stars;
//
// =================================================================================================
//
// // resource_manager.update(glr.out_of_order_gl());
// // engine.update(glr.out_of_order_gl());
// //engine.render(glr.out_of_order_gl());
//
// // if (enablePostProcessing) {
// // 	render_target.size(canvas_size.cast<int32_t>());
// // 	post_processing.size(canvas_size.cast<int32_t>());
// // }
//
// const auto guard_s = glr.state.push_guard();
// const auto guard_m = glr.model.push_guard();
// const auto guard_v = glr.view.push_guard();
// const auto guard_p = glr.projection.push_guard();
//
// glr.state.enableBlend();
// glr.state.blendSrc_SourceAlpha();
// glr.state.blendDst_One_Minus_SourceAlpha();
//
// glr.state.enableCullFace();
// glr.state.frontFaceCCW();
// glr.state.cullBackFace();
//
// glr.state.polygonModeFill();
//
// glr.state.enableDepthTest();
// glr.state.depthFunctionLess();
//
// // glr.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
// // glr.setClearColor(0.f, 0.f, 0.f, 1.0f);
// // glr.setClearColor(1.f, 1.f, 1.f, 1.0f);
// glr.setClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//
// glr.projection = camera.projection(window_size.cast<float>());
// glr.view = camera.view();
// glr.model = libv::mat4f::identity();
//
// // --- Render the Scene ---
//
// // if (enablePostProcessing)
// // 	glr.framebuffer_draw(render_target.framebuffer());
// // else
// // 	glr.framebuffer_draw_default();
//
// // glr.viewport({0, 0}, render_target.size());
// glr.viewport({0, 0}, window_size);
// glr.clearColor();
// glr.clearDepth();
//
// {
// 	// particleSet.render(glr, camera.frustum(canvas_size));
// 	// renderer_particle.render(glr, uniform_stream, particleFountain);
// 	// scene.render(glr, camera);
// 	// scene.render(glr);
// 	glr.callback([this](libv::GL& gl) {
// 		// engine.render(gl);
// 		scene.render(gl);
// 	});
// }
//
// // --- Post Processing ---
//
// // if (enablePostProcessing) {
// // 	const auto& main_texture = render_target.resolve(glr);
// // 	post_processing.pass(glr, main_texture);
// // }
