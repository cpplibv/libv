// Project: libv.ui, File: src/libv/ui/overlay_zoom.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/overlay_zoom.lpp>
// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/renderbuffer.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
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
	color0 = texture(sampler0, fragmentUV, 0).rgba;
})";

// -------------------------------------------------------------------------------------------------

class CoreOverlayZoom : public CoreComponent {
	friend class OverlayZoom;
	[[nodiscard]] inline auto handler() { return OverlayZoom{this}; }

private:
	libv::vec2i framebufferSize_;

	libv::vec2f displayPosition = {};
	float zoom_ = 1.f;
	bool control_ = false;

private:
	libv::glr::Framebuffer framebuffer;
	libv::glr::Texture2D::R8_G8_B8_A8 framebufferColor0;

	libv::glr::Mesh quad{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::DynamicDraw};
	libv::glr::Program program;
	libv::glr::Uniform_texture uniformSampler0;

	libv::glr::Mesh lines_border{libv::gl::Primitive::Lines, libv::gl::BufferUsage::DynamicDraw};
	libv::glr::Program lineProgram;

	libv::glr::Mesh lines_cursor{libv::gl::Primitive::Lines, libv::gl::BufferUsage::DynamicDraw};

public:
	using CoreComponent::CoreComponent;

private:
	void init();
	void update();
	void update_cursor();

private:
	virtual void onKey(const EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;

public:
	void postRender(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

void CoreOverlayZoom::init() {
	program.vertex(quad_vs);
	program.fragment(quad_fs);
	program.assign(uniformSampler0, "sampler0", libv::gl::TextureChannel{0});

	lineProgram.vertex(line_vs);
	lineProgram.fragment(line_fs);
}

void CoreOverlayZoom::update() {
	// TODO P5: make sure zoom is px aligned and the lines are pixel perfect

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

void CoreOverlayZoom::update_cursor() {
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

void CoreOverlayZoom::onKey(const EventKey& event) {
	(void) event;
}

void CoreOverlayZoom::onFocus(const EventFocus& event) {
	(void) event;
}

void CoreOverlayZoom::onMouseMovement(const EventMouseMovement& event) {
	if (!control_)
		return;

	if (context().state.mouse_pressed(libv::input::MouseButton::Left)) {
		displayPosition -= event.mouse_movement / zoom_;
		update();
	}

	event.stop_propagation();
}

void CoreOverlayZoom::onMouseScroll(const EventMouseScroll& event) {
	if (!control_)
		return;

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

	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

void CoreOverlayZoom::doAttach() {
	watchFocus(true);
	watchKey(true);
	watchMouse(true);

	// TODO P4: Fix ability to instantly acquire mouse on component creation (before attach run)
	// vvv Start of workaround
	if (control_)
		context().mouse.acquire(*this);
	// ^^^ End of workaround
}

void CoreOverlayZoom::doLayout2(const ContextLayout2& environment) {
	framebufferSize_ = libv::vec::xy(libv::lround(environment.size).cast<int32_t>());

	framebufferColor0 = libv::glr::Texture2D::R8_G8_B8_A8{};
	framebufferColor0.storage(1, framebufferSize_);
	framebufferColor0.set(libv::gl::MagFilter::Nearest);

	framebuffer.attach2D(libv::gl::Attachment::Color0, framebufferColor0);
	update();
}

void CoreOverlayZoom::doRender(Renderer& r) {
	(void) r;
}

void CoreOverlayZoom::postRender(libv::glr::Queue& gl) {
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

// =================================================================================================

core_ptr OverlayZoom::create_core(std::string name) {
	auto core = create_core_ptr<CoreOverlayZoom>(std::move(name));
	// TODO P5: Try to find a way to get back the ctors
	static_cast<CoreOverlayZoom*>(core)->init();
	return core;
}

// -------------------------------------------------------------------------------------------------

void OverlayZoom::control() {
	self().control_ = true;
	// TODO P4: Fix ability to instantly acquire mouse on component creation (before attach run)
	// vvv Start of workaround (comment / uncomment)
	//self().context().mouse.acquire(self());
	// ^^^ Start of workaround
}

void OverlayZoom::view() {
	self().control_ = false;
	self().context().mouse.release(self());
}

void OverlayZoom::disable() {
	self().control_ = false;
	self().context().mouse.release(self());
}

libv::vec2f OverlayZoom::screen_BL() const {
	return self().displayPosition;
}

libv::vec2f OverlayZoom::screen_TR() const {
	return self().displayPosition + self().framebufferSize_.cast<float>() / self().zoom_ - 1.0f;
}

void OverlayZoom::postRender(libv::glr::Queue& gl) {
	self().postRender(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
