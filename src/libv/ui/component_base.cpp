// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/component_base.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//LayoutInfo Component::layout(const LayoutInfo& parentLayout) {
//	LIBV_LOG_UI_COMPONENT_TRACE("Layout [%s]", componentID);
//
//	if (isInvalid() || lastParentLayoutInfo != parentLayout) {
//		lastParentLayoutInfo = parentLayout;
//		lastResultLayoutInfo = doLayout(parentLayout);
//		LIBV_LOG_UI_COMPONENT_TRACE("Result from Layout [%s] [%.1f,%.1f,%.1f]", componentID,
//				lastResultLayoutInfo.size.x, lastResultLayoutInfo.size.y, lastResultLayoutInfo.size.z);
//	} else
//		LIBV_LOG_UI_COMPONENT_TRACE("Result from Cache [%s] [%.1f,%.1f,%.1f]", componentID,
//				lastResultLayoutInfo.size.x, lastResultLayoutInfo.size.y, lastResultLayoutInfo.size.z);
//
//	return lastResultLayoutInfo;
//}

// -------------------------------------------------------------------------------------------------

ComponentBase::ComponentBase(std::string name) :
	name(std::move(name)) {
}

ComponentBase::ComponentBase(UnnamedTag, const std::string_view type) :
	name(libv::concat(type, '-', nextID++)) {
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::invalidate(Flag_t flags_) {
	flags.set(flags_);

	const auto flagsUp = flags_ & Flag::mask_propagateUp;
	for (auto it = parent; !it->flags.match_mask(flagsUp); it = it->parent)
		it->flags.set(flagsUp);

	// TODO P5: Could be restricted to only invalidate if layout is dependent on content
	//			(aka: strong and week invalidation)
	//			Example: invalidate(Flag::invalidLayout);
}

std::string ComponentBase::path() const {
	std::string result = name;

	for (auto it = make_observer_ref(this); it != it->parent; it = it->parent)
		result = it->parent->name + '/' + std::move(result);

	return result;
}

// -------------------------------------------------------------------------------------------------

void ComponentBase::attach(ContextUI& context) {
	log_ui.trace("Attaching {}", path());
	// <<< P1: Flag::invalidAttach is wrong! need strong/weak attach to not repeat an attach on a container
	if (flags.match_any(Flag::invalidAttach)) {
		doAttach(context);
		flags.reset(Flag::invalidAttach);
	}
}

void ComponentBase::create(ContextRender& context) {
	log_ui.trace("Creating {}", path());
	ContextRender currentContext{context.gl};
	doCreate(currentContext);
}

void ComponentBase::render(ContextRender& context) {
	ContextRender currentContext{
		context.gl,
		flags.match_any(Flag::renderChangePosition | Flag::renderChangeSize),
		flags.match_any(Flag::renderChangePosition),
		flags.match_any(Flag::renderChangeSize)};
	doRender(currentContext);

	flags.reset(Flag::renderChangePosition | Flag::renderChangeSize);
//	flags.reset(Flag::invalidRender);
}

void ComponentBase::destroy(ContextRender& context) {
	log_ui.trace("Destroying {}", path());
	ContextRender currentContext{context.gl};
	doDestroy(currentContext);
}

void ComponentBase::layoutPass1(const ContextLayoutPass1& environment) {
//	log_ui.trace("Layout Pass1 {}", path());
	if (flags.match_any(Flag::invalidLayout)) {
		this->doLayoutPass1(environment);
	}
	log_ui.trace("Layout content {:>11}, {}", lastContent, path());
}

void ComponentBase::layoutPass2(const ContextLayoutPass2& environment) {
//	log_ui.trace("Layout Pass2 {}", path());
	if (flags.match_any(Flag::invalidLayout) || environment.position != lastPosition || environment.size != lastSize) {
		this->doLayoutPass2(environment);

		if (environment.position != lastPosition) {
			flags.set(Flag::renderChangePosition);
			lastPosition = position = environment.position;
		}
		if (environment.size != lastSize) {
			flags.set(Flag::renderChangeSize);
			lastSize = size = environment.size;
		}

		flags.reset(Flag::invalidLayout);
	}
	log_ui.trace("Layout {:>11}, {:>11}, {}", lastPosition, lastSize, path());
}

void ComponentBase::foreachChildren(const std::function<void(ComponentBase&)>& callback) {
	doForeachChildren(callback);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
