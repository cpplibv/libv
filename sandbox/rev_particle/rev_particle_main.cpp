// Project: libv.rev, File: sandbox/libv_rev_main.cpp

// libv
// #include <libv/arg/arg.hpp>
// #include <libv/ctrl/controls.hpp>
#include <libv/frame/frame.hpp>
#include <libv/frame/icon_set.hpp>
#include <libv/log/log.hpp>
#include <libv/math/exp_moving_avg.hpp>
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
// std
#include <iostream>
// pro
// #include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle_system.hpp>
// #include <libv/rev/post_processing.hpp>
// #include <libv/rev/render_target.hpp>
// #include <libv/rev/resource/resource_manager.hpp>
// #include <libv/rev/resource/shader.hpp>
#include <libv/rev/effect/particle.hpp>
#include <libv/rev/engine/canvas.hpp>
#include <libv/rev/engine/engine.hpp>
#include <libv/rev/engine/render_pass.hpp>
#include <libv/rev/engine/scene.hpp>
#include <libv/rev/settings.hpp>

#include <libv/sun/ui/overlay_shader_error.hpp>
#include <libv/sun/camera.hpp>

#include <wish/resource_path.hpp>


// -------------------------------------------------------------------------------------------------

static constexpr libv::vec2i window_size = {1680, 1050};

bool enablePostProcessing = true;
bool enableSimulationUpdate = true;
// !!! enableCameraRotate should be different
bool enableCameraRotate = true;

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// -------------------------------------------------------------------------------------------------

struct SandboxState {
	int state = 0;

	libv::rev::ParticleEffect fountain;
	// std::vector<libv::rev::ParticleEffect> fountains;

	// ParticleEffect cameraDust;
	//
	// struct Ship {
	// 	ShipType type;
	// 	ParticleEffect* engineBloom = nullptr;
	// };

	// std::vector<Ship> ships;

	SandboxState() {
		// !!! effect definitions, effect library, load from lua
		// for (int i = 0; i < 10; ++i) {
		// 	auto& fountain = fountains.emplace_back();
		// 	fountain.addModule(std::make_unique<libv::rev::ParticleModuleEmitter>(true, libv::vec3f{0}, libv::vec3f{0, 0, 40.f}, 50.f));
		// 	fountain.addModule(std::make_unique<libv::rev::ParticleModuleGravity>());
		// 	fountain.addModule(std::make_unique<libv::rev::ParticleModuleTurbulence>(1.f));
		// }
		fountain.addModule(std::make_unique<libv::rev::ParticleModuleEmitter>(true, libv::vec3f{0}, libv::vec3f{0, 0, 40.f}, 50.f));
		fountain.addModule(std::make_unique<libv::rev::ParticleModuleGravity>());
		fountain.addModule(std::make_unique<libv::rev::ParticleModuleTurbulence>(1.f));
	}

	void async_update(libv::ui::time_point frameTime, libv::ui::time_duration deltaTime) {
		// engine.async_update(frameTime, deltaTime);
		// engine.async_update(deltaTime);

		// !!! do it auto by the engine
		if (enableSimulationUpdate)
			fountain.update(deltaTime);
			// fountain.update(deltaTime, eye);
		// else
		// 	fountain.sort(eye);
		//
		// if constexpr (!isServer)
		// 	ships[0].engineBloom.update();
	}

	void render(libv::rev::Engine& rev, libv::glr::Queue& glr) {
	// void render(libv::rev::Engine& rev) {
	// void render(libv::rev::RenderPass& rev) {
	// 	fountain.render(rev);
		// fountain.render(rev(layer::effect));
		// fountain.render(rev.layer(layer::effect));
		// rev.render(fountain);
		// rev.render(fountain, layer::effect);
		// !!! do it auto by the engine
		// fountain.sort(glr.eye());
		rev.rendererParticle.render(glr, rev.uniformStream, fountain);
	}
	// void render(libv::rev::RenderPass& rev) {
	// 	fountain.render(rev);
	// }
};

// -------------------------------------------------------------------------------------------------

class SandboxCanvas : public libv::ui::CanvasBase {
	libv::Nexus nexus;

	libv::rev::Canvas canvas;
	// libv::rev::Scene scene;

	libv::sun::CameraPlayer camera;

	SandboxState& state;
	libv::rev::Engine& rev;

	// libv::rev::ParticleEffect particleFountain{scene, particleLibrary.fountain};

public:
	explicit SandboxCanvas(libv::Nexus& nexus, libv::rev::Engine& rev, SandboxState& state) :
		nexus(nexus),
		rev(rev),
		state(state) {
		camera.look_at({100, 100, 60}, {0, 0, 0});
	}

	virtual void update(libv::ui::time_duration deltaTime) override {
		if (enableCameraRotate)
			camera.orbit_yaw(static_cast<float>(deltaTime.count()) * libv::deg_to_rad(22.5f));

		// !!! Should be set differently, also eye must be mutex guarded, 1 frame diff is fine, its only used for sort
		//		but best would be a delayed evaluation, or rather the camera update be part of the async update process?
		// state.eye = camera.eye();
	}

	virtual void render(libv::glr::Queue& glr) override {
		canvas.resize(canvas_size.cast<int32_t>());

		const auto& guard_p = glr.projection.push_guard();
		const auto& guard_v = glr.view.push_guard();
		const auto& guard_s = glr.state.push_guard();

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		glr.state.enableCullFace();
		glr.state.frontFaceCCW();
		glr.state.cullBackFace();

		glr.state.polygonModeFill();

		glr.state.enableDepthTest();
		glr.state.depthFunctionLess();
		glr.clearColorDepth();

		glr.projection = camera.projection(canvas_size);
		glr.view = camera.view();

		// auto blockPass = rev.uniformStream.block_shared(libv::rev::layout_Pass200);

		// auto blockCamera = rev.uniformStream.block_shared(libv::rev::layout_Camera200);
		// blockCamera[libv::rev::layout_Camera200.matP] = glr.projection;
		// blockCamera[libv::rev::layout_Camera200.matV] = glr.view;
		// blockCamera[libv::rev::layout_Camera200.eye] = glr.eye();
		// blockCamera[libv::rev::layout_Camera200.cameraForwardW] = glr.view.forward();
		// blockCamera[libv::rev::layout_Camera200.cameraRightW] = glr.view.right();
		// blockCamera[libv::rev::layout_Camera200.cameraUpW] = glr.view.up();

		// !!! Render layers, passes, ordering
		state.render(rev, glr);
		// state.render(rev, glr, cameraBlock);
		// state.render(rev);
		// state.render(RenderPass);
		rev.rendererEditorGrid.render(glr, rev.uniformStream);


		// window_size.x = width;
		// window_size.y = height;
		//
		// render_target.size(window_size);
		// post_processing.size(window_size);

		// scene.update(deltaTime, camera);
		// camera ???

		// glr.callback([&](libv::gl::GL& gl) {
		// 	scene.render(canvas, gl);
		// });
		// canvas.post_processing();
	}
};

// -------------------------------------------------------------------------------------------------

class SandboxScene {
private:
	libv::rev::Engine engine;

	SandboxState state;

public:
	explicit SandboxScene(libv::Nexus& nexus) :
			engine([] {
				libv::rev::Settings settings;
				settings.model.base_path = "../../res/model/";
				settings.shader.base_path = "../../res/shader/";
				settings.texture.base_path = "../../res/texture/";
				return settings;
			}(), nexus) {}

public:
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) {
		auto canvas = libv::ui::createCanvas<SandboxCanvas>("canvas", nexus, engine, state);

		canvas.event().global_before_update([this](const libv::ui::EventBeforeUpdate& event) mutable {
			state.async_update(event.frame_time, event.delta_time);
			engine.async_update(event.frame_time, event.delta_time);
		});
		canvas.event().before_create.connect_system([this](const auto& event) mutable {
			engine.create(event.gl);
		});
		canvas.event().before_render.connect_system([this](const auto& event) mutable {
			engine.prepare_render(event.gl);
		});
		canvas.event().after_destroy.connect_system([this](const auto& event) mutable {
			engine.destroy(event.gl);
		});

		return canvas;
	}
};

// -------------------------------------------------------------------------------------------------

class Sandbox : public libv::Frame {
	// libv::ctrl::Controls controls;

	libv::ui::UI ui{[] {
		libv::ui::Settings settings;
		settings.res_font.base_path = "res/font/";
		settings.res_font.restrict_under_base = true;
		settings.res_shader.base_path = "res/shader/";
		settings.res_shader.restrict_under_base = true;
		settings.res_texture.base_path = "res/texture/";
		settings.res_texture.restrict_under_base = true;
		settings.track_style_scripts = true;

		settings.res_resolve = [](const std::string& path) {
			return wish::resource_path(path);
		};
		// settings.style_scripts = { "style.lua" };
		return settings;
	}()};

public:
	Sandbox() : Frame("REV - Particle System Demo", window_size) {
		register_nexus(ui.nexus());

		// --- Frame Initialization ---
		setOpenGLVersion(4, 5);
		setPosition(libv::Frame::FramePosition::os_default);
		setIcon(libv::frame::icon_set_iris_cyan());
		onKey.output([this](const libv::input::EventKey& event) {
			if (event.action != libv::input::Action::press)
				return;

			if (event.keycode == libv::input::Keycode::Escape)
				closeDefault();

			if (event.keycode == libv::input::Keycode::P)
				enablePostProcessing = !enablePostProcessing;

			if (event.keycode == libv::input::Keycode::Space)
				enableSimulationUpdate = !enableSimulationUpdate;

			if (event.keycode == libv::input::Keycode::C)
				enableCameraRotate = !enableCameraRotate;

			if (event.keycode == libv::input::Keycode::O)
				setSwapInterval(getSwapInterval() ? 0 : 1);
		});

		// --- UI Initialization ---
		ui.attach(*this);
		ui.load_style_script_file(wish::resource_path("res/style.lua"));
		// ui.load_style_script_file("style.lua");
		ui.add(init_ui());
		ui.add(overlay_version());
		ui.add(overlay_fps());
		ui.add(libv::sun::overlay_shader_error(true));
	}

	~Sandbox() {
		unregister_nexus(ui.nexus());
	}

private:
	void register_nexus(libv::Nexus& nexus) {
		(void) nexus;
		// ...
	}

	void unregister_nexus(libv::Nexus& nexus) {
		(void) nexus;
		// ...
	}

	[[nodiscard]] libv::ui::Component init_ui() {
		return libv::ui::SceneContainer::na("main-sc", "main",
				libv::ui::createScene<SandboxScene>(ui.nexus())(ui.nexus()));
	}

	[[nodiscard]] libv::ui::Component overlay_version() {
		return libv::ui::Label::nsa("version", "overlay.version", fmt::format("v0"));
	}

	[[nodiscard]] libv::ui::Component overlay_fps() {
		static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{250};

		// TODO P4: Implement proper frame statistics
		//			min, max, avg, window min, window max, window avg, 1% low
		//  		(including integration with UI and Game state statistics)
		auto fps = libv::ui::Label::ns("fps", "overlay.fps");
		ui.event().global_before_update([
				fps,
				frameTimer = libv::Timer{},
				updateTimer = libv::Timer{},
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

	wish::change_current_path(argc, argv);

	Sandbox sandbox;
	sandbox.show();
	sandbox.join();

	return EXIT_SUCCESS;
}

// =================================================================================================
//
// inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};
// struct UserParticleLibrary : libv::rev::ParticleLibrary {
// 	// using libv::rev::ParticleLibrary::ParticleLibrary;
// 	libv::rev::ParticleEffectDefinition smallExplosion = load("smallExplosion");
// 	libv::rev::ParticleEffectDefinition rain = load("rain");
// 	libv::rev::ParticleEffectDefinition rainHeavy = load("rainHeavy");
// 	libv::rev::ParticleEffectDefinition smokeStack = load("smokeStack");
// 	libv::rev::ParticleEffectDefinition fountain = load("fountain");
//
// 	UserParticleLibrary() {
// 		fountain.addModule<libv::rev::ModuleEmitter>(true, libv::vec3f{0, 0, 0}, libv::vec3f{5.f, 5.f, 40.f}, 5.f);
// 		fountain.addModule<libv::rev::ModuleGravity>();
// 	}
// };
// UserParticleLibrary particleLibrary{engine};
// 	// libv::rev::ParticleSystem particle_rain;
// 	// libv::rev::ParticleSystem particle_stars;
// 	// libv::rev::ParticleSystem particle_stars;
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
// 	glr.callback([this](libv::gl::GL& gl) {
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
