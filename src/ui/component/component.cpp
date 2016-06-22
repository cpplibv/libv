// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/component/component.hpp>
// pro
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Component::validate() {
	LIBV_UI_COMPONENT_TRACE("Validate [%s]", componentID);
	invalid = false;
}

void Component::invalidate() {
	if (!invalid) {
		LIBV_UI_COMPONENT_TRACE("Invalidate [%s]", componentID);
		invalid = true;
		if (parent)
			parent->invalidate();
	}
}

bool Component::isInvalid() const {
	return invalid;
}

// -------------------------------------------------------------------------------------------------

//void Component::visit(std::function<void(Component&)>& func) {
//	doVisit(func);
//}
//
//void Component::doVisit(std::function<void(Component&)>& func) {
//	func(*this);
//}

// -------------------------------------------------------------------------------------------------

LayoutInfo Component::layout(const LayoutInfo& parentLayout) {
	LIBV_UI_COMPONENT_TRACE("Layout [%s]", componentID);
	auto l = doLayout(parentLayout);
	LIBV_UI_COMPONENT_TRACE("Result Layout [%s] [%f,%f,%f]", componentID, l.size.x, l.size.y, l.size.z);
	return l;
}

void Component::build(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Build [%s]", componentID);
	doBuild(renderer);
	validate();
}

void Component::destroy(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy [%s]", componentID);
	doDestroy(renderer);
}

void Component::render(Renderer& renderer) {
//	LIBV_UI_COMPONENT_TRACE("Render [%s]", componentID); // Until no better log sys too hot log msg
	doRender(renderer);
}

// -------------------------------------------------------------------------------------------------

LayoutInfo Component::doLayout(const LayoutInfo&) {
	auto property = get(Property::Size);
	return LayoutInfo(property ? *property : vec3());
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