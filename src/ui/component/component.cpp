// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <vl/ui/component/component.hpp>
// pro
#include <vl/ui/frame/frame.hpp>

namespace vl {
namespace ui {

//bool Component::isActive() const {
//	return false;
//}
//
//bool Component::isFocused() const {
//	return false;
//}

void Component::build(Renderer&) { }

void Component::destroy(Renderer&) { }

void Component::invalidate() {
	invalid = true;
}

void Component::render(Renderer&) { }

void Component::update() { }

void Component::setDisplayPosition(const ivec3& pos) {
	this->pos = pos;
	//boundsChanged()
	//invalidate();
}

void Component::setParent(ProtectedContainer* parent) {
	this->parent = parent;
}

void Component::setDisplaySize(const ivec3& size) {
	this->size = size;
	//onResize();
	//invalidate();
}

ivec3 Component::getDisplayPosition() const {
	return pos;
}

ivec3 Component::getDisplaySize() const {
	return size;
}

Frame* Component::getFrame() const {
	return frame;
}

ProtectedContainer* Component::getParent() const {
	return parent;
}

Component::Component() { }

Component::Component(ivec3 size) {
	properties.set(Property::Size, size);
}

Component::Component(ivec3 size, ivec3 pos) {
	properties.set(Property::Size, size);
	properties.set(Property::Position, pos);
}

Component::~Component() { }

} //namespace ui
} //namespace vl