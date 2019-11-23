// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/overlay_zoom.lpp>
// libv
#include <libv/glr/attribute.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_state.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

static constexpr auto line_vs = R"(
#version 330 core

layout(location = 0) in vec2 vertexPos;

out vec2 fragmentUV;

void main() {
	gl_Position = vec4(vertexPos, 0, 1);
})";

static constexpr auto line_fs = R"(
#version 330 core

uniform sampler2D sampler0;

out vec4 color0;

void main() {
	color0 = vec4(1, 0, 0, 1);
})";

static constexpr auto quad_vs = R"(
#version 330 core

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 vertexTex;

out vec2 fragmentUV;

void main() {
	gl_Position = vec4(vertexPos, 0, 1);
	fragmentUV = vertexTex;
})";

static constexpr auto quad_fs = R"(
#version 330 core

uniform sampler2D sampler0;

in vec2 fragmentUV;

out vec4 color0;

void main() {
	color0 = texture2D(sampler0, fragmentUV, 0).rgba;
})";

// -------------------------------------------------------------------------------------------------

OverlayZoom::OverlayZoom(BaseComponent& parent) :
	BaseComponent(parent, GenerateName, "overlay-zoom") {
	init();
}

OverlayZoom::OverlayZoom(BaseComponent& parent, std::string name) :
	BaseComponent(parent, std::move(name)) {
	init();
}

OverlayZoom::OverlayZoom(BaseComponent& parent, GenerateName_t, const std::string_view type) :
	BaseComponent(parent, GenerateName, type) {
	init();
}

OverlayZoom::~OverlayZoom() { }

// -------------------------------------------------------------------------------------------------

void OverlayZoom::control() {
	control_ = true;
	context().mouse.acquire(*this);
}

void OverlayZoom::view() {
	control_ = false;
	context().mouse.release(*this);
}

void OverlayZoom::disable() {
	control_ = false;
	context().mouse.release(*this);
}

libv::vec2f OverlayZoom::screen_BL() const {
	return displayPosition;
}

libv::vec2f OverlayZoom::screen_TR() const {
	return displayPosition + framebufferSize_.cast<float>() / zoom_ - 1.0f;
}

// -------------------------------------------------------------------------------------------------

void OverlayZoom::init() {
	program.vertex(quad_vs);
	program.fragment(quad_fs);
	program.assign(uniformSampler0, "sampler0", libv::gl::TextureChannel{0});

	lineProgram.vertex(line_vs);
	lineProgram.fragment(line_fs);
}

void OverlayZoom::update() {
	const auto fboSize = libv::vec::cast<float>(framebufferSize_);
	const auto matrix = libv::mat4f::ortho(displayPosition, fboSize / zoom_);

	{
		quad.clear();

		auto pos = quad.attribute(libv::glr::Attribute<0, libv::vec2f>{});
		auto tex = quad.attribute(libv::glr::Attribute<1, libv::vec2f>{});
		auto index = quad.index();

		const auto p0 = libv::vec::xy(matrix * libv::vec4f(fboSize * -0.f, 0, 1));
		const auto p1 = libv::vec::xy(matrix * libv::vec4f(fboSize * +1.f, 0, 1));

		pos(p0.x, p0.y);
		pos(p1.x, p0.y);
		pos(p1.x, p1.y);
		pos(p0.x, p1.y);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
	}
	{
		lines_border.clear();

		auto pos = lines_border.attribute(libv::glr::Attribute<0, libv::vec2f>{});
		auto index = lines_border.index();

		const auto p0 = libv::vec::xy(matrix * libv::vec4f(fboSize * -1.f - 0.0f, 0, 1));
		const auto p1 = libv::vec::xy(matrix * libv::vec4f(fboSize * -0.f - 0.5f, 0, 1));
		const auto p2 = libv::vec::xy(matrix * libv::vec4f(fboSize * +1.f + 0.5f, 0, 1));
		const auto p3 = libv::vec::xy(matrix * libv::vec4f(fboSize * +2.f + 0.0f, 0, 1));

		pos(p0.x, p1.y); pos(p3.x, p1.y);
		pos(p0.x, p2.y); pos(p3.x, p2.y);

		pos(p1.x, p0.y); pos(p1.x, p3.y);
		pos(p2.x, p0.y); pos(p2.x, p3.y);

		index.line(0, 1);
		index.line(2, 3);
		index.line(4, 5);
		index.line(6, 7);
	}
}

void OverlayZoom::update_cursor() {
	const auto mouse_position = context().state.mouse_position();
	const auto fboSize = libv::vec::cast<float>(framebufferSize_);
	const auto matrix = libv::mat4f::ortho(displayPosition, fboSize / zoom_);

	{
		lines_cursor.clear();

		auto pos = lines_cursor.attribute(libv::glr::Attribute<0, libv::vec2f>{});
		auto index = lines_cursor.index();

		// NOTE: Only works because zoom is always power of 2, otherwise zoom would have to be accounted in next nearest pixel center calculation
		const auto p0 = libv::vec::xy(matrix * libv::vec4f(mouse_position - 0.0f, 0, 1)) - 1.f / fboSize;
		const auto p1 = libv::vec::xy(matrix * libv::vec4f(mouse_position + 1.0f, 0, 1)) + 1.f / fboSize;

		pos(p0.x, p0.y);
		pos(p0.x, p1.y);
		pos(p1.x, p1.y);
		pos(p1.x, p0.y);

		index.line(0, 1);
		index.line(1, 2);
		index.line(2, 3);
		index.line(3, 0);
	}
}

// -------------------------------------------------------------------------------------------------

bool OverlayZoom::onKey(const libv::input::EventKey& event) {
	(void) event;

	return true;
}

void OverlayZoom::onFocus(const EventFocus& event) {
	(void) event;
}

bool OverlayZoom::onMouseMovement(const EventMouseMovement& event) {
	if (!control_)
		return false;

	if (context().state.mouse_pressed(libv::input::Mouse::Left)) {
		displayPosition -= event.mouse_movement / zoom_;
		update();
	}

	return true;
}

bool OverlayZoom::onMouseScroll(const EventMouseScroll& event) {
	if (!control_)
		return false;

	const auto old_zoom = zoom_;
	auto new_zoom = std::clamp(old_zoom * std::pow(1.25f, event.scroll_movement.y), 0.512f, 330.872245f);
	if (new_zoom > 0.85f && new_zoom < 1.2f) // stabilization around 1.0f
		new_zoom = 1.0f;
	zoom_ = new_zoom;

	// Zoom to center:
	//	const auto fboSize = libv::vec::cast<float>(framebufferSize_);
	//	displayPosition += fboSize / old_zoom / 2.0f - fboSize / new_zoom / 2.0f;
	// Zoom to cursor:
	displayPosition += context().state.mouse_position() / old_zoom - context().state.mouse_position() / new_zoom;
	update();

	return true;
}

// -------------------------------------------------------------------------------------------------

void OverlayZoom::doAttach() {
	watchFocus(true);
	watchKey(true);
	watchMouse(Flag::watchMousePosition | Flag::watchMouseScroll);
}

void OverlayZoom::doLayout2(const ContextLayout2& environment) {
	framebufferSize_ = libv::vec::xy(environment.size.cast<int32_t>());

	framebufferColor0 = libv::glr::Texture2D::R8_G8_B8_A8{};
	framebufferColor0.storage(1, framebufferSize_);
	framebufferColor0.set(libv::gl::MagFilter::Nearest);

	framebuffer.attach2D(libv::gl::Attachment::Color0, framebufferColor0);
	update();
}

void OverlayZoom::doRender(ContextRender& context) {
	auto& gl = context.gl;

	const auto fboSize = libv::vec::cast<float>(framebufferSize_);
	const auto blitPosition = libv::vec::max(displayPosition, libv::vec2f{0, 0}).cast<int32_t>();
	const auto blitSize = libv::vec::min(displayPosition + fboSize / zoom_, fboSize).cast<int32_t>();

	gl.blit_from_default(framebuffer,
			blitPosition, blitSize,
			blitPosition, blitSize,
			libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);

	gl.setClearColor(0, 0, 0, 1);
	gl.clearColor();
	gl.clearDepth();

	const auto guard_s = gl.state.push_guard();

	gl.state.blendSrc_One();
	gl.state.blendDst_Zero();
	gl.state.disableDepthTest();

	{
		gl.program(program);
		gl.texture(framebufferColor0, libv::gl::TextureChannel{0});
		gl.render(quad);
	}
	{
		gl.program(lineProgram);
		gl.render(lines_border);
	}
	{
		update_cursor();
		gl.program(lineProgram);
		gl.render(lines_cursor);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
