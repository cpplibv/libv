// Project: libv.noise, File: sandbox/noise/noise_view_main.cpp

// libv
// #include <libv/arg/arg.hpp>
// #include <libv/ctrl/controls.hpp>
#include <libv/container/vector_2d.hpp>
#include <libv/frame/frame.hpp>
#include <libv/frame/icon_set.hpp>
#include <libv/log/log.hpp>
#include <libv/math/exp_moving_avg.hpp>
#include <libv/noise/noise_2.hpp>
#include <libv/res/resource_path.hpp>
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
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/rev/renderer/renderer_editor_grid.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/rev/settings.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader/attribute.hpp>

#include <libv/sun/ui/overlay_shader_error.hpp>
#include <libv/sun/camera.hpp>

#include <wish/resource_mapping.hpp>



#include <libv/glr/procedural/plane.hpp>


// -------------------------------------------------------------------------------------------------

static constexpr libv::vec2i window_size = {1680, 1050};

bool enablePostProcessing = true;
bool enableSimulationUpdate = true;
bool enableCameraRotate = true;

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// -------------------------------------------------------------------------------------------------

struct UniformsNoiseGen {
	libv::glr::Uniform_float time;
	libv::glr::Uniform_texture texture;
	libv::glr::Uniform_texture texture_cpu;

	template <typename Access> void access_uniforms(Access& access) {
		access(time, "time", 0.f);
		access(texture, "texture", libv::rev::textureUnit_diffuse);
		access(texture_cpu, "texture_cpu", libv::rev::textureUnit_normal);
	}

	template <typename Access> void access_blocks(Access& access) {
		(void) access;
	}
};

using ShaderNoiseGen = libv::rev::Shader<UniformsNoiseGen>;

// -------------------------------------------------------------------------------------------------

struct UniformsPlane {
	libv::glr::Uniform_texture texture;
	libv::glr::Uniform_mat4f matMVP;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture, "texture0Sampler", libv::rev::textureUnit_diffuse);
		access(matMVP, "matMVP");
	}

	template <typename Access> void access_blocks(Access& access) {
		(void) access;
	}
};

using ShaderPlane = libv::rev::Shader<UniformsPlane>;

// -------------------------------------------------------------------------------------------------

class SandboxCanvas : public libv::ui::CanvasBase {
	libv::Nexus nexus;

	libv::sun::CameraPlayer camera;

	libv::rev::ResourceManager& rm;
	libv::rev::RendererEditorGrid& rendererEditorGrid;
	libv::glr::UniformBuffer& uniformStream;

	libv::glr::Mesh plane_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Texture2D::R32F gen_texture;
	libv::glr::Texture2D::RGB32F cpu_texture;
	ShaderNoiseGen shader;
	ShaderPlane shaderPlane;

public:
	explicit SandboxCanvas(libv::Nexus& nexus, libv::rev::ResourceManager& rm, libv::rev::RendererEditorGrid& rendererEditorGrid, libv::glr::UniformBuffer& uniformStream) :
			nexus(nexus),
			rm(rm),
			rendererEditorGrid(rendererEditorGrid),
			uniformStream(uniformStream),
			shader(rm.shader, "noise_gen.cs"),
			shaderPlane(rm.shader, "simple_texture.vs", "simple_texture.fs") {
		camera.look_at({100, 100, 60}, {0, 0, 0});

		{
			plane_mesh.clear();

			constexpr auto attribute_position = libv::glr::Attribute<0, libv::vec3f>{};
			constexpr auto attribute_texture0 = libv::glr::Attribute<8, libv::vec2f>{};

			auto position = plane_mesh.attribute(attribute_position);
			auto texture0 = plane_mesh.attribute(attribute_texture0);
			auto index = plane_mesh.index();

			libv::glr::GeneratePlaneXY<float>(libv::vec2f{1024, 1024})(position, texture0, index);
		}

		gen_texture.storage(1, {1024, 1024});
		gen_texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
		gen_texture.set(libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::One);
		// gen_texture.set(libv::gl::MinFilter::Nearest);
		// gen_texture.set(libv::gl::MagFilter::Nearest);

		cpu_texture.storage(1, {1024, 1024});
		cpu_texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
		cpu_texture.set(libv::gl::Swizzle::Red, libv::gl::Swizzle::Green, libv::gl::Swizzle::Blue, libv::gl::Swizzle::One);
		// cpu_texture.set(libv::gl::MinFilter::Nearest);
		// cpu_texture.set(libv::gl::MagFilter::Nearest);
		std::vector<libv::vec3f> cpu_data;
		for (int y = 0; y < 1024; ++y) {
			float yf = static_cast<float>(y);
			for (int x = 0; x < 1024; ++x) {
				float xf = static_cast<float>(x);
				auto seed_offset = x / 64;
				const auto uv = libv::vec2f{xf / 1024.f, yf / 1024.f} * 10.f;
				const auto cpu = libv::noise::simplex(0x5EED + seed_offset, uv) * 0.5f + 0.5f;
				// const auto cpu = libv::noise::cellular(0x5EED, xf / 1024.f, yf / 1024.f, 3.1415f) * 0.5f + 0.5f;
				// const auto cpu = yf / 1024.f;
				// const auto cpu = libv::noise::simplex(0x5EED, xf / 1024.f, yf / 1024.f, 3.1415f) * 0.5f + 0.5f;
				// const auto cpu = libv::noise::simplex(0x5EED, xf / 1024.f, yf / 1024.f) * 0.5f + 0.5f;
				// const auto cpu = libv::noise::cellular(0x511D, uv * 20.f,
				// 		libv::noise::CellularDistance::euclideanSq,
				// 		libv::noise::CellularReturn::cellValue,
				// 		1.f) * 0.5f + 0.5f;

				// const auto warp = libv::noise::fractal(0x511D, uv.x * 5.f, uv.y * 5.f, libv::noise::warp, 5) * 20.f;
				// const auto warp = libv::noise::fractal_progressive(0x511D, uv.x * 5.f, uv.y * 5.f, libv::noise::simplex_grad, 5, 50.f);
				// const auto warp = libv::noise::perlin(0x5EED, uv.x * 5.f, uv.y * 5.f, libv::noise::simplex_grad, 5, 50.f);
				// const auto warp = libv::noise::fractal(0x511D, uv.x * 5.f, uv.y * 5.f, libv::noise::warp, 5, 200.f);
				// const auto cpu = static_cast<int>(libv::round(uv * 20.f + warp).x + libv::round(uv * 20.f + warp).y) % 2 == 0;
				// const auto gpu = output(x, y);
				// const auto diff = gpu - cpu;
				// fmt::print("CPU: {:7.5f} GPU: {:7.5f} Diff: {: 10.8f}\n", cpu, gpu, diff);

				cpu_data.push_back({cpu, 0, 0});
			}
		}
		cpu_texture.image(0, {0, 0}, {1024, 1024}, cpu_data.data());
	}

	virtual void update(libv::ui::time_duration deltaTime) override {
		if (enableCameraRotate)
			camera.orbit_yaw(static_cast<float>(deltaTime.count()) * libv::deg_to_rad(22.5f));
	}

	virtual void render(libv::glr::Queue& glr) override {
		rm.update(glr.out_of_order_gl());

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

		{
			glr.program(shader.program());
			glr.bindImageTexture(0, gen_texture, 0, libv::gl::BufferAccessFull::Write);
			glr.uniform(shader.uniform().time, static_cast<float>(this->ui().state.time().count()));
			glr.texture(cpu_texture, libv::rev::textureUnit_normal);
			glr.dispatchCompute(1024 / 8, 1024 / 8);
			glr.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
		}


		rendererEditorGrid.render(glr, uniformStream);


		glr.projection = libv::mat4f::ortho({0, 0}, canvas_size);
		glr.view = libv::mat4f::identity();
		glr.model = libv::mat4f::identity();

		{
			const auto guard_m = glr.model.push_guard();
			glr.model.translate(canvas_size.x * 0.5f, canvas_size.y * 0.5f, 0);

			glr.program(shaderPlane.program());
			glr.uniform(shaderPlane.uniform().matMVP, glr.mvp());
			glr.texture(gen_texture, libv::rev::textureUnit_diffuse);
			glr.render(plane_mesh);
		}

		// glr.callback([this](libv::GL& gl) {
		// 	libv::vector_2D<float> output{1024, 1024};
		// 	gl.readTextureImage(gen_texture.out_of_order_gl(), 0, libv::gl::ReadFormat::R, libv::gl::DataType::F32, output.data());
		//
		// 	for (int y = 0; y < 1024; ++y) {
		// 		float yf = static_cast<float>(y);
		// 		for (int x = 0; x < 1024; ++x) {
		// 			float xf = static_cast<float>(x);
		//
		// 			// const auto cpu = libv::noise::simplex(0x5EED, xf / 1024.f, yf / 1024.f, 3.1415f) * 0.5f + 0.5f;
		// 			// const auto cpu = libv::noise::cellular(0x5EED, xf / 1024.f, yf / 1024.f, 3.1415f) * 0.5f + 0.5f;
		// 			// const auto cpu = yf / 1024.f;
		// 			// const auto cpu = libv::noise::simplex(0x5EED, xf / 1024.f, yf / 1024.f, 3.1415f) * 0.5f + 0.5f;
		// 			// const auto cpu = libv::noise::simplex(0x5EED, xf / 1024.f, yf / 1024.f) * 0.5f + 0.5f;
		// 			// const auto cpu = libv::noise::cellular(0x511D, xf / 1024.f, yf / 1024.f,
		// 			// 		libv::noise::CellularDistance::euclidean,
		// 			// 		libv::noise::CellularReturn::cellValue, 1.f) * 0.5f + 0.5f;
		// 			// const auto gpu = output(x, y);
		// 			// const auto diff = gpu - cpu;
		// 			// fmt::print("CPU: {:7.5f} GPU: {:7.5f} Diff: {: 10.8f}\n", cpu, gpu, diff);
		// 		}
		// 	}
		// });
	}
};

// -------------------------------------------------------------------------------------------------

class SandboxScene {
private:
	libv::rev::ResourceManager rm;
	libv::rev::RendererEditorGrid rendererEditorGrid{rm};
	libv::glr::UniformBuffer uniformStream{libv::gl::BufferUsage::StreamDraw};

public:
	explicit SandboxScene(libv::Nexus& nexus) :
		rm([] {
				libv::rev::Settings settings;
				settings.model.base_path = "../../res/model/";
				settings.shader.base_path = "../../res/shader/";
				settings.texture.base_path = "../../res/texture/";
				return settings;
			}(), nexus) {
		rm.shader.add_include_directory("", "res/shader/");
	}

public:
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus) {
		return libv::ui::createCanvas<SandboxCanvas>("canvas", nexus, rm, rendererEditorGrid, uniformStream);
	}
};

// -------------------------------------------------------------------------------------------------

class Sandbox : public libv::Frame {
	// libv::ctrl::Controls controls;

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
	Sandbox() : Frame("LIBV - Noise View", window_size) {
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

			if (event.keycode == libv::input::Keycode::M)
				setOpenGLSamples(static_cast<OpenGLSamples>(0));
			if (event.keycode == libv::input::Keycode::N)
				setOpenGLSamples(static_cast<OpenGLSamples>(4));

		});

		// --- UI Initialization ---
		ui.attach(*this);
		ui.load_style_script_file(std::string(libv::res::resource_path("res/style.lua")));
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
		// static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{250};
		static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{2500};

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
	libv::res::init_resource_mapping(wish::resource_mapping());

	Sandbox sandbox;
	sandbox.show();
	sandbox.join();

	return EXIT_SUCCESS;
}
