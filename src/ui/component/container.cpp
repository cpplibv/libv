// File: Container.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include <libv/ui/component/container.hpp>
// ext
#include <glm/gtc/matrix_transform.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/layout/properties.hpp>
#include <libv/ui/layout/layout_manager.hpp>

//TODO P4: default layout mrg (dummy fix size max overlapping? ot flow?)
//TODO P4: invalidate in not virtual!
//TODO P4: build could be eliminated? do your stuff in render?
//TODO P4: more definite size management (mostly for frame! and its event) (sorry about this task)

#include <iostream>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Container::add(const observer_ptr<Component>& component) {
	assert(!component->getParent());

	component->setParent(this);
	components.emplace_back(component);
}

void Container::add(const shared_ptr<Component>& component) {
	assert(!component->getParent());

	component->setParent(this);
	components.emplace_back(component);
}

//void Container::remove(adaptive_ptr<Component> component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//}

void Container::setLayout(const observer_ptr<LayoutManager>& manager) {
	this->layoutManager = manager;
}

void Container::setLayout(const shared_ptr<LayoutManager>& manager) {
	this->layoutManager = manager;
}

// -------------------------------------------------------------------------------------------------

Container::const_iterator Container::begin() const {
	return components.begin();
}

Container::const_iterator Container::end() const {
	return components.end();
}

Container::const_reverse_iterator Container::rbegin() const {
	return components.rbegin();
}

Container::const_reverse_iterator Container::rend() const {
	return components.rend();
}

Container::iterator Container::begin() {
	return components.begin();
}

Container::iterator Container::end() {
	return components.end();
}

Container::reverse_iterator Container::rbegin() {
	return components.rbegin();
}

Container::reverse_iterator Container::rend() {
	return components.rend();
}

// -------------------------------------------------------------------------------------------------

void Container::buildComponents(Renderer& renderer) {
	for (auto& component : components) {
		component->build(renderer);
	}
}

void Container::destroyComponents(Renderer& renderer) {
	for (auto& component : components) {
		component->destroy(renderer);
	}
}

void Container::invalidateComponents() {
	for (auto& component : components) {
		component->invalidate();
	}
}

void Container::renderComponents(Renderer& gl) {
	for (auto& component : components) {
		const auto position = component->getDisplayPosition();
		const auto size = component->getDisplaySize();

		gl.pushMatrixView(glm::ortho(
				position.x, position.x + size.x,
				position.y, position.y + size.y, 1000, -1000));
		component->render(gl);
		gl.popMatrixView();
	}
}

void Container::updateComponents() {
	for (auto& component : components) {
		component->update();
	}
}

// -------------------------------------------------------------------------------------------------

void Container::build(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Build Container");
	if (layoutManager) {
		layoutManager->layout(components.begin(), components.end(), this);
	}
	buildComponents(renderer);

	Component::validate();
}

void Container::destroy(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy Container");
	destroyComponents(renderer);
}

void Container::invalidate() {
	LIBV_UI_COMPONENT_TRACE("Invalidate Container");
	Component::invalidate();
	invalidateComponents();
}

void Container::render(Renderer& renderer) {
	renderComponents(renderer);
}

void Container::update() {
	updateComponents();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv


//	ivec3 size = v->getDisplaySize();
//	ivec3 pos = v->getDisplayPosition();
//
//	glPushMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glViewport(pos.x, pos.y, size.x, size.y);
//	gluOrtho2D(0, size.x, 0, size.y);
//
//	glBegin(GL_TRIANGLE_STRIP);
//	glColor4f(v->id, 0, v->blue, 1);
//	glVertex3f(0, 0, 0);
//	glVertex3f(size.x, 0, 0);
//	glVertex3f(0, size.y, 0);
//	glVertex3f(size.x, size.y, 0);
//	glEnd();
//	glPopMatrix();