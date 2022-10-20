// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
// pro
//#include <star/game/scene/surface/camera.hpp>
#include <star/game/scene/utility.hpp>
//#include <star/game/config/client_config.hpp>



#include <libv/ctrl/controls.hpp>

#include <libv/sun/camera.hpp>

#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/rev/settings.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/ui.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position   = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal     = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0     = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_bondID     = libv::glr::Attribute<4, libv::vec4i>{};
constexpr auto attribute_bondWeight = libv::glr::Attribute<5, libv::vec4f>{};
constexpr auto attribute_tangent    = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent  = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0   = libv::glr::Attribute<8, libv::vec2f>{};

//constexpr auto attribute_scale      = libv::glr::Attribute<11, float>{};
//constexpr auto attribute_rotation   = libv::glr::Attribute<12, float>{};
constexpr auto attribute_type       = libv::glr::Attribute<13, int32_t>{};
constexpr auto attribute_custom1    = libv::glr::Attribute<14, libv::vec4f>{};
constexpr auto attribute_custom0    = libv::glr::Attribute<15, libv::vec4f>{};
constexpr auto attribute_custom0_f2 = libv::glr::Attribute<15, libv::vec2f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern = libv::gl::TextureChannel{7};

struct UniformLayoutMatrices {
	// Could be split into two: Camera and Model
	// 			Camera: matV, matP, near, far, ...
	// 			Model: matM, matMVP

	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};

inline const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{0, "Matrices"};
//inline const auto uniformBlock_definitions = libv::glr::UniformBlockBinding{1, "SpriteDefinitions"};

inline const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);

// -------------------------------------------------------------------------------------------------

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

//using ShaderColor = libv::rev::Shader<UniformsColor>;
//using ShaderCommandArrow = libv::rev::Shader<UniformsCommandArrow>;
//using ShaderEditorBackground = libv::rev::Shader<UniformsEditorBackground>;
//using ShaderFleet = libv::rev::Shader<UniformsFleet>;
//using ShaderPlanet = libv::rev::Shader<UniformsPlanet>;
//using ShaderSurface = libv::rev::Shader<UniformsSurface>;
using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;
//using ShaderText = libv::rev::Shader<UniformsText>;
//using ShaderModel = libv::rev::Shader<UniformsModel>;
//using Shader = libv::rev::Shader<>;

// -------------------------------------------------------------------------------------------------

struct RendererResourceContext {
	libv::rev::ResourceManager loader{[] {
		libv::rev::Settings settings;
		settings.texture.base_path = "../../res/texture/";
		settings.shader.base_path = "../../res/shader/";
		settings.model.base_path = "../../res/model/";
//		settings.texture.base_path = "res/texture/";
//		settings.shader.base_path = "res/shader/";
//		settings.model.base_path = "res/model/";
		return settings;
	}()};
//	libv::rev::ShaderLoader shader_loader{"shader/"};
//	libv::rev::ModelLoader model_loader{"model/"};
	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	RendererResourceContext() {
		// Include the res/shader/ folder from libv
//		loader.shader.add_include_directory("", "../../res/shader/");
//		loader.shader.add_include_directory("", "res/shader/");
//		// Include the res/model/ folder from libv
//		model_loader.add_include_directory("", "../../res/model/");
	}
};

struct RendererEditorGrid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererEditorGrid(RendererResourceContext& rctx);

//	void build_mesh();
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

RendererEditorGrid::RendererEditorGrid(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "surface/editor_grid_plane.vs", "surface/editor_grid_plane.fs") {
	auto position = mesh_grid.attribute(attribute_position);
	auto index = mesh_grid.index();

	position(-1, -1, 0);
	position(+1, -1, 0);
	position(+1, +1, 0);
	position(-1, +1, 0);

	index.quad(0, 1, 2, 3); // Front face quad
	index.quad(0, 3, 2, 1); // Back face quad
}

void RendererEditorGrid::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.render(mesh_grid);
}

struct Renderer {
	RendererResourceContext resource_context;

	RendererEditorGrid editorGrid{resource_context};
//	RendererDebug debug{resource_context};
//	RendererGizmo gizmo{resource_context};
//	RendererText text{resource_context};
//	RendererSkybox sky{resource_context};
//	RendererVeggie veggie{resource_context};
//	RendererSurface surface{resource_context};
//	RendererSurfaceTexture surfaceTexture{resource_context};
//	RendererSprite sprite{resource_context.loader};

public:
//	explicit Renderer(libv::ui::UI& ui);
	explicit Renderer();
	void prepare_for_render(libv::glr::Queue& gl);
};

//Renderer::Renderer(libv::ui::UI& ui) {
Renderer::Renderer() {
//	resource_context.loader.shader.attach_libv_ui_hub(ui.event_hub());
//	resource_context.loader.shader.attach_nexus(nexus);
//	text.font = ui.context().font("consola.ttf");
}

void Renderer::prepare_for_render(libv::glr::Queue& glr) {
	// shader_loader.update MUST run before any other render queue operation
	// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
	// | So this is a better place than before, still not the best, When UI gets rev updates in the future maybe there will be better solutions
	resource_context.loader.update(glr.out_of_order_gl());
}

struct CanvasSurface : libv::ui::CanvasBase {
public:
//	GameSession& game_session;
//
//	// TODO P1: app.space: Should not store it (?) only to bypass controls invalidation issue
	libv::ctrl::Controls& controls;

	Renderer renderer;

	libv::sun::CameraPlayer camera;
	libv::sun::CameraPlayer::screen_picker screen_picker;

private:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
//	Renderer& renderer;

//	float time = 0.0f;

public:
//	CanvasSurface(libv::ui::UI& ui) :
//		renderer(ui) {
	explicit CanvasSurface(libv::ctrl::Controls& controls) :
		controls(controls),
		renderer(),
		screen_picker(camera.picker({100, 100})) { // <<< 100 100
		camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

		controls.context_enter<libv::sun::BaseCameraOrbit>(&camera);
	}
//	SpaceCanvas(libv::ctrl::Controls& controls, Renderer& renderer, GameSession& game_session, bool main_canvas);
	~CanvasSurface() {
		controls.context_leave_if_matches<libv::sun::BaseCameraOrbit>(&camera);
	}

//public:
//	void enableControls(libv::ctrl::Controls& controls) {
//		if (!controlVar)
//			// TODO P1: app.space:
//			controlVar.emplace(
//					*this,
//					playout,
//					playout.simulation->universe->galaxy,
//	//				player,
//					game_session.player
//			);
//
//		controls.context_enter<CanvasControl>(&*controlVar);
//		controls.context_enter<libv::sun::BaseCameraOrbit>(&camera);
//	}
//
//	void disableControls(libv::ctrl::Controls& controls) {
//		controls.context_leave_if_matches<libv::sun::BaseCameraOrbit>(&camera);
//		controls.context_leave_if_matches<CanvasControl>(&*controlVar);
//	}

private:
	virtual void attach() override {
		focus();
	}
	virtual void update(libv::ui::time_duration delta_time) override {

	}
	virtual void render(libv::glr::Queue& glr) override {
		renderer.prepare_for_render(glr);

//		bool enablePostProcessing = true;

		// NOTE: screen_picker update has to be placed around render, as canvas_size is only set after layout
		// TODO P1: Move screen_picker update into a layout post hook so mouse event use the update value
		screen_picker = camera.picker(canvas_size);

//		if (enablePostProcessing) {
//			renderTarget.size(canvas_size.cast<int32_t>());
//			postProcessing.size(canvas_size.cast<int32_t>());
//		}

		const auto s_guard = glr.state.push_guard();

		glr.state.enableDepthTest();
		glr.state.depthFunctionLess();
		glr.state.enableDepthMask();

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		glr.state.cullBackFace();
		glr.state.enableCullFace();
		glr.state.frontFaceCCW();

		glr.state.clipPlanes(0);
		glr.state.polygonModeFill();
	//	gl.state.polygonModeLine();

		glr.projection = camera.projection(canvas_size);
		glr.view = camera.view();
		glr.model = libv::mat4f::identity();

//		const auto eye = glr.eye();

//		if (enablePostProcessing) {
//			// Set framebuffer to the post-processing target
//			glr.framebuffer_draw(renderTarget.framebuffer());
//		}

		glr.clearColor();
		glr.clearDepth();

//		// --- Render Cleanup? ---
//		// TODO P1: Find a better way of managing text (and debug shape) lifetimes
//		renderer.text.clear_texts();
//
//		// --- Render Opaque ---
//		render_opaque(glr, eye, playout.simulation->universe->galaxy);

		// --- Render EditorBackground/Sky ---
//		render_background(glr, playout.simulation->universe->galaxy);

//		// --- Render Transparent ---
//		render_transparent(glr, playout.simulation->universe->galaxy);

		// --- Render Debug shapes ---
//		const auto s2_guard = glr.state.push_guard();
//		glr.state.disableDepthMask();
//		renderer.debug.render(glr, renderer.resource_context.uniform_stream);

		// --- Render UI/HUD ---

		{
			{ // Grid
				const auto s2_guard = glr.state.push_guard();
				glr.state.disableDepthMask();
				renderer.editorGrid.render(glr, renderer.resource_context.uniform_stream);
			}

//			{ // Camera orbit point
//				const auto s2_guard = glr.state.push_guard();
//				glr.state.disableDepthMask();
//
//				const auto m_guard = glr.model.push_guard();
//				glr.model.translate(camera.orbit_point());
//				glr.model.scale(0.2f);
//				renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);
//			}
//
//			{ // Camera orientation gizmo in top right
//				const auto s2_guard = glr.state.push_guard();
//				glr.state.disableDepthTest();
//				glr.state.disableDepthMask();
//
//				const auto p_guard = glr.projection.push_guard();
//				const auto v_guard = glr.view.push_guard();
//				const auto m_guard = glr.model.push_guard();
//
//				const auto orientation_gizmo_size = 64.f; // The axes of the gizmo will be half of this size
//				const auto orientation_gizmo_margin = 4.f;
//
//				glr.projection = libv::mat4f::ortho(
//						-canvas_size + orientation_gizmo_size * 0.5f + orientation_gizmo_margin,
//						canvas_size,
//						-orientation_gizmo_size,
//						+orientation_gizmo_size);
//				glr.view = camera.orientation_view().translate(-1, 0, 0);
//				glr.model.scale(orientation_gizmo_size * 0.5f);
//
//				renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);
//			}
		}

		// --- Post Processing ---

//		if (enablePostProcessing) {
//			const auto& mainTexture = renderTarget.resolve(glr);
//			postProcessing.pass(glr, mainTexture);
//		}

		// Post-Processing sets the framebuffer back to the default
		// glr.framebuffer_draw_default();

		// --- Text ---

//		renderer.text.add_debug_coordinates_if_nothing_else();
//		renderer.text.render(glr, renderer.resource_context.uniform_stream, screen_picker);
	}
//	void render_opaque(libv::glr::Queue& glr, libv::vec3f eye, Galaxy& galaxy);

//	void render_background(libv::glr::Queue& glr, Galaxy& galaxy) {
//		if (!main_canvas)
//			return;
//
//		(void) galaxy;
//
//		const auto s_guard = glr.state.push_guard();
//		// No need to write depth data for the main background
//		glr.state.disableDepthMask();
//		renderer.editorBackground.render(glr, canvas_size);
//	}

//	void render_transparent(libv::glr::Queue& glr, Galaxy& galaxy);
};

// -------------------------------------------------------------------------------------------------

struct SceneSurface {
	std::string some_data;

	[[nodiscard]] libv::ui::Component create_scene(libv::Nexus& nexus) const {
		auto layers = libv::ui::PanelAnchor::n("layers");

		layers.add_na<libv::ui::CanvasAdaptorT<CanvasSurface>>("canvas",
				requireBean<libv::ctrl::Controls>(nexus, "Surface", "Controls"));

		auto line = layers.add_n<libv::ui::PanelLine>("line");
		line.anchor(libv::ui::Anchor::bottom_center);
		line.orientation(libv::ui::Orientation::down);
		line.size(libv::ui::Size{libv::ui::dynamic(), libv::ui::dynamic()});

		auto label = line.add_a<libv::ui::Label>("Surface: " + some_data);
		label.align_horizontal(libv::ui::AlignHorizontal::center);
		label.align_vertical(libv::ui::AlignVertical::center);
		label.font_color({1, 1, 1, 1});
		label.size(libv::ui::Size{libv::ui::dynamic(), libv::ui::dynamic()});

		auto btn = line.add_a<libv::ui::Button>("Back to Main Menu");
		btn.size(libv::ui::Size{libv::ui::dynamic(), libv::ui::dynamic()});
		btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
			switchParentScene("main", source, createSceneMainMenu(nexus));
		});

		return layers;
	}
};

libv::ui::Component createSceneSurface(libv::Nexus& nexus) {
	return assembleScene<SceneSurface>("Hi")(nexus);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
