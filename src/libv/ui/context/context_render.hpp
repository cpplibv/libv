// Project: libv.ui, File: src/libv/ui/context/context_render.hpp

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
#include <libv/math/vec.hpp>
// std
#include <functional>
#include <memory>
#include <span>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/shader_font.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/shader_quad.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Component;
class Renderer;

// -------------------------------------------------------------------------------------------------

class ContextRender {
private:
	std::unique_ptr<class ImplContextRender> self;

public:
	ContextRender();
	~ContextRender();

public:
	[[nodiscard]] Renderer root_renderer(const Component& root, libv::glr::Queue& glr, time_point time_frame, time_duration time, libv::vec2f mouse_position, libv::vec2f ui_size);
	void execute_render(libv::glr::Queue& glr);
};

// -------------------------------------------------------------------------------------------------

class Renderer {
private:
	class ImplContextRender& context;
	libv::glr::Queue& glr;

private:
	const Component& current_component;
	libv::vec2f clip_pos;
	libv::vec2f clip_size;

public:
	bool changed_layout = false;   /// Components size or position has changed since last render call
	bool changed_position = false; /// Components position has changed since last render call
	bool changed_size = false;     /// Components size has changed since last render call

public:
	Renderer(class ImplContextRender& context, libv::glr::Queue& glr, const Component& root) noexcept;
	Renderer(const Renderer& parent_renderer, const Component& child) noexcept;

public: // --- Context ---
	[[nodiscard]] time_point time_frame() const noexcept;

public: // --- State ---
	void translate(libv::vec3f value) noexcept;
	void clip(libv::vec2f pos, libv::vec2f size) noexcept;

public: // --- Low level ---
	//	Additional low level stuff, or maybe just a GL object itself? | If that happens the client need immediate mode switch
	//			uniform
	//			buffer
	//			framebuffer

	void begin_triangles();
	void end(const Texture2D_view& texture, const ShaderImage_view& shader);
	void vertex(libv::vec3f pos, libv::vec2f uv, libv::vec4f color);
	void vertex(libv::vec3f pos, libv::vec2f uv, libv::vec4f tile, libv::vec4f color);
	void index_strip(std::span<const uint32_t> indices);
	void index_strip(std::initializer_list<const uint32_t> indices);

//	// Line vertex
//	void vertex_2(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});
//	// Line strip vertex
//	void vertex_2s(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});
//	// Triangle vertex
//	void vertex_3(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});
//	// Triangle strip vertex
//	void vertex_3s(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});
//	// Quad vertex
//	void vertex_4(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});
//	// Quad strip vertex
//	void vertex_4s(libv::vec2f pos, libv::vec2f uv = {0, 0}, libv::vec4f color = {1, 1, 1, 1});

//public: // --- Mid level ---
//	void quad(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00 = {0, 0}, libv::vec2f uv11 = {1, 1}, libv::vec4f color = {1, 1, 1, 1});

public: // --- High level ---
	void quad(libv::vec2f pos, libv::vec2f size, libv::vec4f color, const ShaderQuad_view& shader);
	void text(libv::vec2f pos, class TextLayout& text, libv::vec4f color, const Font2D_view& font, const ShaderFont_view& shader);
	void texture_2D(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f color, const Texture2D_view& texture, const ShaderImage_view& shader);
	void texture_2D(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f tile, libv::vec4f color, const Texture2D_view& texture, const ShaderImage_view& shader);

public: // --- Component level ---
	[[nodiscard]] Renderer enter(const Component& child);

public: // --- Native level ---
	void native(std::function<void(libv::glr::Queue&)>);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
