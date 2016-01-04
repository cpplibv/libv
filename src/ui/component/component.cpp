// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/component/component.hpp>
// pro
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Component::build(Renderer&) {
	LIBV_UI_COMPONENT_TRACE("Build Component");
	validate();
}

void Component::destroy(Renderer&) {
	LIBV_UI_COMPONENT_TRACE("Destroy Component");
}

void Component::invalidate() {
	LIBV_UI_COMPONENT_TRACE("Invalidate Component");
	invalid = true;
}

void Component::render(Renderer&) {
}

void Component::update() { }

// -------------------------------------------------------------------------------------------------

void Component::validate() {
	invalid = false;
}

bool Component::isInvalid() const {
	return invalid;
}

// -------------------------------------------------------------------------------------------------

void Component::setDisplayPosition(const ivec3& position) {
	this->displayPosition = position;
	//boundsChanged()
	//invalidate();
}

void Component::setParent(Container* parent) {
	this->parent = parent;
	this->frame_ = parent->frame();
}

void Component::setDisplaySize(const ivec3& size) {
	this->displaySize = size;
	//onResize();
	//invalidate();
}

// -------------------------------------------------------------------------------------------------

ivec3 Component::getDisplayPosition() const {
	return displayPosition;
}

ivec3 Component::getDisplaySize() const {
	return displaySize;
}

Frame* Component::frame() const {
	return frame_;
}

Container* Component::getParent() const {
	return parent;
}

// -------------------------------------------------------------------------------------------------

Component::Component() { }

Component::Component(ivec3 size) {
	set(Property::Size, size);
}

Component::Component(ivec3 size, ivec3 pos) {
	set(Property::Size, size);
	set(Property::Position, pos);
}

Component::~Component() { }

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv