// Project: libv, File: app/space/space_main.cpp

// std
#include <filesystem>
#include <iostream>
// pro
//#include <space/game/game_instance.hpp>
#include <space/log.hpp>


#include <iostream>
#include <space/sim/gen/surface.hpp>
#include <space/view/camera.hpp>

#include <libv/color/space.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/math/noise/perlin.hpp>
#include <libv/noise/noise.hpp>
#include <libv/rev/post_processing.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/timer.hpp>
#include <space/sim/gen/surface.hpp>
#include <space/view/camera.hpp>
#include <space/view/camera_control.hpp>
#include <space/view/frame.hpp>
#include <space/view/render/renderer.hpp>
#include <space/view/render/renderer.hpp>
#include <libv/meta/resolve.hpp>
//#include <libv/ui/component/canvas.hpp>


namespace space {


class SurfaceCanvas : public libv::ui::CanvasBase {
public:
	CameraPlayer camera;
//	CameraPlayer::screen_picker screen_picker;

private:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
	Renderer renderer;
	libv::glr::Texture2D::RGBA32F heightMap;
	int count;

public:
	explicit SurfaceCanvas(libv::ui::UI& ui) :
			renderer(ui)
	//screen_picker(camera.picker({100, 100})),
	//renderTarget({100, 100}, 4),
	//postProcessing(renderer.resource_context.shader_loader, {100, 100})
	{
		count = 1024;
		camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

		heightMap.storage(1, libv::vec2i{count, count});
		//postProcessing.vignetteIntensity(0.15f);
	}

private:
	virtual void attach() override {
		focus();
	}

//	virtual void update(libv::ui::time_duration delta_time) override {}
	virtual void render(libv::glr::Queue& glr) override {
		renderer.prepare_for_render(glr);

//		screen_picker = camera.picker(canvas_size);
//		renderTarget.size(canvas_size.cast<int32_t>());
//		postProcessing.size(canvas_size.cast<int32_t>());

		const auto s_guard = glr.state.push_guard();

		glr.state.enableDepthTest();
		glr.state.depthFunctionLess();
		glr.state.enableDepthMask();

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		//glr.state.enableCullFace();
		glr.state.disableCullFace();
		glr.state.cullBackFace();
		glr.state.frontFaceCCW();

		glr.state.clipPlanes(0);
		glr.state.polygonModeFill();
		//glr.state.polygonModeLine();

		glr.projection = camera.projection(canvas_size);
		glr.view = camera.view();
		glr.model = libv::mat4f::identity();

		//const auto eye = glr.eye();

		// Set framebuffer to the post-processing target
		//glr.framebuffer_draw(renderTarget.framebuffer());

		glr.setClearColor(0, 0, 0, 1);
		glr.clearColor();
		glr.clearDepth();

		// =================================================================================================

		libv::Timer timerNoiseGen;

		Surface surface;
		std::vector<libv::vec4f> colors;
		colors.reserve(count * count);

//		renderer.debug.points.clear();

		{
			const auto s2_guard = glr.state.push_guard();
//			glr.state.disableDepthMask();
//			float noise_values[100][100];

			surface.points.reserve(count);
			for (int i = 0; i < count; ++i) {
				std::vector<SurfacePoint> temp;
				temp.reserve(count);
				for (int j = 0; j < count; ++j) {
					auto i_f = static_cast<float>(i);
					auto j_f = static_cast<float>(j);
					auto count_f = static_cast<float>(count);
					auto noise_value = libv::noise_simplex(5000, i_f / 50.f, j_f / 50.f);
//					auto noise_value = libv::noise_fractal(100, i_f / 50.f, j_f / 50.f, libv::noise_perlin<float>, 8, 0.3f);
//					auto noise_value = libv::noise_simplex(1, i_f / 50.f, j_f / 50.f, libv::CellularDistanceFunction::euclidean, libv::CellularReturnType::distance2);
					//auto noise_value = libv::ValueNoise::noise_value(0x5EED, i_f / 100.f, j_f / 100.f);
					//auto noise_value = libv::ValueNoise::noise_value(0x5EED, i_f / count.f, j_f / count.f);

//					const auto color = libv::vec4f{libv::color::hue_to_rgb(1.f / 3.f * (noise_value * 0.5f + 0.5f)), 1.f};
					const auto color = libv::vec4f{libv::vec3f::one(noise_value * 0.5f + 0.5f), 1.f};
//							libv::vec4f{1, 1, 1, 1}
//					renderer.debug.points.emplace_back(
//							libv::vec3f{i_f, j_f, noise_value*0.1f},
//							color
//					);
					temp.emplace_back(SurfacePoint{libv::vec3f{i_f / count_f, j_f / count_f, noise_value * 0.1f}, color});
//					surface.points[i][j] = SurfacePoint{libv::vec3f{i_f, j_f, noise_value * 0.1f}, color};
					colors.emplace_back(color);
				}
				surface.points.emplace_back(temp);
			}

			fmt::print("TimerNoiseGen: {:8.4f} ms", timerNoiseGen.timed_ms().count());
			std::cout << std::endl;

			heightMap.image(0, libv::vec2i{0, 0}, libv::vec2i{count, count}, colors.data());


//			renderer.debug.quads.emplace_back(
//					libv::vec3f{0, 0, 0},
//					libv::vec3f{0, 1, 0},
//					libv::vec3f{1, 1, 0},
//					libv::vec3f{1, 0, 0},
//					libv::vec4f{0.2f, 0.3f, 0.9f, noise_values}
//			);

//			renderer.surface.render(glr, renderer.resource_context.uniform_stream, surface);
			renderer.surfaceTexture.render(glr, renderer.resource_context.uniform_stream, heightMap);
		}

	}
};


class SurfaceViewer {
public:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
	SpaceFrame frame;

	libv::ctrl::Controls controls;

	libv::ui::UI ui;

//	space::Renderer renderer;
//	space::CameraPlayer camera;
//	space::CameraPlayer::screen_picker screen_picker;

public:
	SurfaceViewer() :
			ui([] {
				libv::ui::Settings settings;
				// TODO P1: Internalize used UI resources under space, currently: app/space/../../res/
				settings.res_font.base_path = "../../res/font/";
				settings.res_shader.base_path = "../../res/shader/";
				settings.res_texture.base_path = "../../res/texture/";

				settings.res_font.restict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_shader.restict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_texture.restict_under_base = false; // TODO P1: Should go away with proper res folder

				settings.track_style_scripts = true;
				return settings;
			}()) {
		frame.setSize(1024, 1024);

		CameraControl::register_controls(controls);
		CameraControl::bind_default_controls(controls);
//		CanvasControl::register_controls(controls);
//		CanvasControl::bind_default_controls(controls);

		controls.attach(frame);
		ui.attach(frame);

//		libv::ui::Label label("helloLabel");
//		label.text("Surface app");
//		label.font_color(libv::vec4f(1, 1, 1, 1));
//		label.align_horizontal(libv::ui::AlignHorizontal::center);
//		label.align_vertical(libv::ui::AlignVertical::center);
//		ui.add(label);

		libv::ui::CanvasAdaptorT<SurfaceCanvas> canvas("canvas", ui);
		canvas.z_index_offset(-100);
		canvas.event().focus.connect([this, canvas](const libv::ui::EventFocus& e) mutable {
			if (e.gain())
				controls.context_enter<BaseCameraOrbit>(&canvas.object().camera);
			else
				controls.context_leave_if_matches<BaseCameraOrbit>(&canvas.object().camera);
		});
		ui.add(canvas);




//		frame.onKey.output([canvas](const libv::input::EventKey& e) mutable {
//			if (e.keycode == libv::input::Keycode::H) {
//				canvas.object().camera.move_up(-1.f);
//			}
//		});
	}

	void execute() {
		frame.show();
		frame.join();
	}
};

} // namespace space


int main(int argc, const char** argv) {
	(void) argc;
	(void) argv;


	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
		space::log_space.info("Hello Surface!");

		// Change working directory
		if (std::filesystem::exists("app/space/"))
			std::filesystem::current_path("app/space/");
		else
			std::filesystem::current_path("../app/space/"); // During development binary artifacts created under /bin

		// Run the game
		space::SurfaceViewer surfaceViewer;
		surfaceViewer.execute();

	} catch (const std::exception& e) {
		space::log_space.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
