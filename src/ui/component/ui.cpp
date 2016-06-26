// File:   ui.hpp Author: Vader Created on 2016. január 7., 6:35

// hpp
#include <libv/ui/component/ui.hpp>
// ext
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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

bool UI::isInvalid() const {
	return !valid || content.isInvalid();
}

void UI::validate() {
	valid = true;
}

void UI::invalidate() {
	valid = false;
}

// -------------------------------------------------------------------------------------------------

ivec2 UI::getPosition() const {
	return position;
}

void UI::setPosition(ivec2 position) {
	this->position = position;
}

ivec2 UI::getSize() const {
	return size;
}

void UI::setSize(ivec2 size) {
	this->size = size;
}

// -------------------------------------------------------------------------------------------------

void UI::build() {
	LayoutInfo root(vec3(size, 0));
	content.layout(root);
	content.build(gl);
	validate();
	checkGL();
}

void UI::destroy() {
	content.destroy(gl);
	invalidate();
	checkGL();
}

void UI::render() {
	glClearColor(0.236f, 0.311f, 0.311f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl::viewport(position, size);

	auto x0 = position.x;
	auto x1 = position.x + size.x;
	auto y0 = position.y;
	auto y1 = position.y + size.y;
	gl.pushMatrixProjection(glm::ortho<float>(x0, x1, y0, y1));

	content.render(gl);

	gl.popMatrixProjection();
	checkGL();
}

// -------------------------------------------------------------------------------------------------

void UI::attach(observer_ptr<Frame> frame) {
	frame->onWindowSize.output([this](auto e) {
		this->setSize(e.size);
		this->invalidate();
	});
	frame->onClosed.output([this](auto /*frame*/) {
		this->destroy();
	});
}

void UI::refresh() {
	if (isInvalid())
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
	content.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
	content.setAlignContent(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
	content.setOrient(ui::PanelFlow::ORIENT_RIGHT_UP);
	content.set(Property::Size, percent(100), percent(100), 0);
}

UI::~UI() { }

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv