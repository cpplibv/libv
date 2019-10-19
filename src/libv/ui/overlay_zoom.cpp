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

void OverlayZoom::init() {
	program.vertex(quad_vs);
	program.fragment(quad_fs);
	program.assign(uniformSampler0, "sampler0", libv::gl::TextureChannel{0});

	lineProgram.vertex(line_vs);
	lineProgram.fragment(line_fs);
}

void OverlayZoom::update() {
	const auto fboSize = libv::vec::cast<float>(framebufferSize_);
	const auto matrix = libv::mat4f::ortho(libv::vec::cast<float>(displayPosition), fboSize / zoom_);

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
		lines.clear();

		auto pos = lines.attribute(libv::glr::Attribute<0, libv::vec2f>{});
		auto index = lines.index();

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

// -------------------------------------------------------------------------------------------------

bool OverlayZoom::onKey(const libv::input::EventKey& event) {
	(void) event;

	return true;
}

void OverlayZoom::onFocus(const EventFocus& event) {
	(void) event;
}

bool OverlayZoom::onMouseMovement(const EventMouseMovement& event) {
	if (context().state.mouse_pressed(libv::input::Mouse::Left)) {
		displayPosition -= event.mouse_movement;
		update();
	}

	return true;
}

bool OverlayZoom::onMouseScroll(const EventMouseScroll& event) {
	const auto fboSize = libv::vec::cast<float>(framebufferSize_);

	const auto old_zoom = zoom_;
	const auto new_zoom = std::max(1.0f, zoom_ * (1.0f + event.scroll_movement.y * 0.25f));
	zoom_ = new_zoom;

	displayPosition = displayPosition + fboSize / old_zoom / 2.0f - fboSize / new_zoom / 2.0f;
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
		gl.render(lines);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
