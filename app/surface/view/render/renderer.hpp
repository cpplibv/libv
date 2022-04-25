// Project: libv, File: app/space/view/render/renderer.hpp

#pragma once

// fwd
#include <surface/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/text_layout.hpp>
//#include <libv/glr/layout_to_string.hpp>
// pro
//#include <surface/sim/universe.hpp>
#include <surface/view/camera.hpp>
//#include <surface/view/render/model.hpp>
#include <surface/view/render/shaders.hpp>

#include <surface/surface/chunk.hpp>


// =================================================================================================

// TODO P1: kill these ones, aka implement 'pass' / 'program' uniform block (requires glr shared block allocation)
inline float global_time = 0.0f;
inline int32_t global_test_mode = 0;

// =================================================================================================

namespace surface {
using Mesh = libv::glr::Mesh;

// -------------------------------------------------------------------------------------------------

struct RendererResourceContext {
	libv::rev::ShaderLoader shader_loader{"shader/"};
//	libv::rev::ModelLoader model_loader{"model/"};
	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	RendererResourceContext() {
		// Include the res/shader/ folder from libv
		shader_loader.add_include_directory("", "../../res/shader/");
//		// Include the res/model/ folder from libv
//		model_loader.add_include_directory("", "../../res/model/");
	}
};

// -------------------------------------------------------------------------------------------------


struct RendererGizmo {
	Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererGizmo(RendererResourceContext& rctx);

	void build_gizmo_lines(Mesh& mesh);
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererEditorGrid {
	Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererEditorGrid(RendererResourceContext& rctx);

//	void build_mesh();
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererText {
private:
	ShaderText shader;

	Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::DynamicDraw};

	struct Entry {
		libv::vec3f position;
		libv::vec2f screenOffset;
		libv::vec4f color;
		libv::ui::TextLayout layout;
	};
	std::vector<Entry> texts;

public:
	// TODO P3: Make default font private, and init it in ctor
	std::shared_ptr<libv::ui::Font2D> font;

public:
	explicit RendererText(RendererResourceContext& rctx);

	// TODO P3: A better lifetime/ownership model for rendered 'asset' reuse is necessary (same for debug)
	void add_text(
			libv::vec3f position,
			libv::vec2f screenOffset,
			std::string str,
			libv::vec4f color = libv::vec4f(1, 1, 1, 1),
			libv::ui::FontSize fontSize = libv::ui::FontSize{12},
			libv::ui::AlignHorizontal alignH = libv::ui::AlignHorizontal::center,
			libv::ui::AlignVertical alignV = libv::ui::AlignVertical::center);

	void clear_texts();
	void add_debug_coordinates_if_nothing_else();

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const CameraPlayer::screen_picker& screen_picker);
};

// -------------------------------------------------------------------------------------------------

struct Renderer {
	RendererResourceContext resource_context;

	RendererEditorGrid editorGrid{resource_context};
	RendererGizmo gizmo{resource_context};
	RendererText text{resource_context};

public:
	explicit Renderer(libv::ui::UI& ui);
	void prepare_for_render(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
