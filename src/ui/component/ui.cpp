// File:   ui.hpp Author: Vader Created on 2016. január 7., 6:35

// hpp
#include <libv/ui/component/ui.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/log.hpp>
// pro
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Context& UI::getContext() {
	return context;
}

vec2i UI::getSize() const {
	return size;
}

void UI::setSize(vec2i size) {
	this->size = size;
}

// -------------------------------------------------------------------------------------------------

void UI::build() {
	if (!valid) {
		context.load();
	}
	if (content.isInvalid()) {
		LayoutInfo root(vec3f(size, 0));
		content.layout(root);
		content.build(context);
	}
	valid = true;
	LIBV_GL_CHECK();
}

void UI::destroy() {
	content.destroy(context);
	context.unload();
	valid = false;
	LIBV_GL_CHECK();
}

void UI::render() {
	context.refresh();

	context.gl.enable(gl::Capability::DepthTest);
	glDepthFunc(GL_LESS); // TODO P1: continue GL wrapping
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	context.gl.frontFaceCCW();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	LIBV_GL_CHECK();

//	context.gl.clearColor(0.236f, 0.311f, 0.311f, 1.f);
	context.gl.clear();
	context.gl.viewport(position, size);

	auto x0 = position.x;
	auto x1 = position.x + size.x;
	auto y0 = position.y;
	auto y1 = position.y + size.y;
	auto pStackGuard = context.gl.projection.pushGuard();
	context.gl.projection = libv::ortho<float>(x0, x1, y0, y1);

	content.render(context);

	LIBV_GL_CHECK();
}

// -------------------------------------------------------------------------------------------------

void UI::attach(observer_ptr<Frame> frame) {
	frame->onWindowSize.output([this](auto e) {
		this->setSize(e.size);
		this->content.set(Property::Size, px(e.size.x), px(e.size.y), 0);
	});
	frame->onClosed.output([this](auto /*frame*/) {
		this->destroy();
	});
}

void UI::refresh() {
	build();
	render();
}

void UI::add(const observer_ptr<Component>& component) {
	content.add(component);
}

void UI::add(const shared_ptr<Component>& component) {
	content.add(component);
}

// -------------------------------------------------------------------------------------------------

UI::UI() {
	content.setComponentID("UIRoot");
	content.setAlign(ui::ALIGN_BOTTOM_LEFT);
	content.setAnchor(ui::ALIGN_BOTTOM_LEFT);
	content.setOrient(ui::ORIENT_RIGHT_UP);
	content.set(Property::Size, percent(100), percent(100), 0);
	ParentAccessor::setUI(make_observer(content), make_observer(this));
}

UI::~UI() { }

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv