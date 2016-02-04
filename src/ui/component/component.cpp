// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/component/component.hpp>
// pro
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Component::setDisplayPosition(vec3 position) {
	this->displayPosition = position;
	invalidate();
}

void Component::setDisplaySize(vec3 size) {
	this->displaySize = size;
	invalidate();
}

vec3 Component::getDisplayPosition() const {
	return displayPosition;
}

vec3 Component::getDisplaySize() const {
	return displaySize;
}

// -------------------------------------------------------------------------------------------------

void Component::validate() {
	LIBV_UI_COMPONENT_TRACE("Validate Component [%s]", componentID);
	invalid = false;
}

void Component::invalidate() {
	LIBV_UI_COMPONENT_TRACE("Invalidate Component [%s]", componentID);
	invalid = true;
}

bool Component::isInvalid() const {
	return invalid;
}

// -------------------------------------------------------------------------------------------------

void Component::visit(std::function<void(libv::observer_ptr<Component>)>& func) {
	doVisit(func);
}

void Component::doVisit(std::function<void(libv::observer_ptr<Component>)>& func) {
	func(make_observer(this));
}

// -------------------------------------------------------------------------------------------------

void Component::build(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Build Component [%s]", componentID);
	doBuild(renderer);
	validate();
}

void Component::destroy(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy Component [%s]", componentID);
	doDestroy(renderer);
}

void Component::render(Renderer& renderer) {
	if (isInvalid())
		build(renderer);

	doRender(renderer);
}

// -------------------------------------------------------------------------------------------------

Component::Component() { }

Component::Component(const std::string& componentID, const std::string& componentClass) :
	componentID(componentID),
	componentClass(componentClass) { }

Component::~Component() { }

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv