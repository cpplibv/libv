// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/core_component.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <cassert>
// pro
#include <libv/ui/context_event.hpp>
#include <libv/ui/context_focus_traverse.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_mouse.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/context_ui_link.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

CoreComponent::CoreComponent(std::string name) :
	context_(current_thread_context()),
	name(std::move(name)) { }

CoreComponent::CoreComponent(GenerateName_t, const std::string_view type, size_t index) :
	context_(current_thread_context()),
	name(libv::concat(type, '-', index)) { }

CoreComponent::~CoreComponent() {
	log_ui.error_if(isAttached(), "Component is destructed in an attached state: {}", path());
}

// -------------------------------------------------------------------------------------------------

std::string CoreComponent::path() const {
	std::string result = name;

	for (auto it = make_observer_ref(this); it != it->parent; it = it->parent)
		result = it->parent->name + '/' + std::move(result);

	return result;
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::size(Size value) noexcept {
	size_ = value;
	flags.set_to(Flag::parentDependOnLayout, value.has_dynamic());

	for (auto it = make_observer_ref(this); it != it->parent && it->flags.match_any(Flag::parentDependOnLayout); it = it->parent)
		it->flagDirect(Flag::pendingLayoutSelf);

	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::flagDirect(Flag_t flags_) noexcept {
	flags.set(flags_);
}

void CoreComponent::flagAncestors(Flag_t flags_) noexcept {
	for (auto it = parent; !it->flags.match_mask(flags_); it = it->parent)
		it->flags.set(flags_);
}

void CoreComponent::flagAuto(Flag_t flags_) noexcept {
	flagDirect(flags_ & Flag::mask_self);
	flagAncestors(flags_ & Flag::mask_propagate);
}

void CoreComponent::flagForce(Flag_t flags_) noexcept {
	flagDirect(flags_);
	flagAncestors(flags_);
}

void CoreComponent::flagPurge(Flag_t flags_) noexcept {
	flags.reset(flags_ & Flag::mask_self);

	for (auto it = parent; true; it = it->parent) {
		Flag_t childFlags = Flag::none;

		it->doForeachChildren([&childFlags](CoreComponent& child) {
			childFlags |= child.flags;
		});

		if (it->flags.match_mask(Flag::mask_propagate, childFlags))
			return;

		it->flags.reset(Flag::mask_propagate);
		it->flags.set(calculatePropagateFlags(childFlags));

		if (it == it->parent)
			return;
	}
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::watchChar(bool value) noexcept {
	flags.set_to(Flag::watchChar, value);
}

void CoreComponent::watchKey(bool value) noexcept {
	flags.set_to(Flag::watchKey, value);
}

void CoreComponent::watchFocus(bool value) noexcept {
	if (value) {
		flagAuto(Flag::focusable | Flag::watchFocus);
		return;
	}

	if (!flags.match_any(Flag::focusableSelf))
		return;

	if (flags.match_mask(Flag::focusableChild))
		flags.reset(Flag::focusableSelf | Flag::watchFocus); // There is a child with focus, no need to purge flag
	else
		flagPurge(Flag::focusable | Flag::watchFocus);

	if (isAttached() && flags.match_any(Flag::focused)) {
		context().detachFocused(*this);
	}
}

void CoreComponent::watchMouse(bool value) noexcept {
	if (flags.match_full(Flag::watchMouse) == value)
		return;

	// NOTE: pendingAttachSelf flag is used instead of isAttached to allow usage of this function in doAttach
	if (!flags.match_any(Flag::pendingAttachSelf)) { // Already attached
		if (value)
			context().mouse.subscribe(*this);
		else
			context().mouse.unsubscribe(*this);
	}

	flags.set_to(Flag::watchMouse, value);
}

bool CoreComponent::isWatchChar() const noexcept {
	return flags.match_any(Flag::watchChar);
}

bool CoreComponent::isWatchKey() const noexcept {
	return flags.match_any(Flag::watchKey);
}

bool CoreComponent::isWatchFocus() const noexcept {
	return flags.match_any(Flag::watchFocus);
}

bool CoreComponent::isWatchMouse() const noexcept {
	return flags.match_any(Flag::watchMouse);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::focus() noexcept {
	// Note: focus has to be sync to allow correct event processing
	if (!isAttached())
		log_ui.error("Attempted to focus a non-attached component. Attempt ignored. {}", path());

	else if (!isFocusableComponent())
		log_ui.error("Attempted to focus a non-focusable component. Attempt ignored. {}", path());

	else
		context().focus(*this);
}

void CoreComponent::markRemove() noexcept {
	flagAuto(Flag::pendingDetach | Flag::pendingLayout);
	flags.reset(Flag::layout | Flag::render);

	doForeachChildren([](CoreComponent& child) {
		child.markRemove();
	});
}

void CoreComponent::style(libv::intrusive_ptr<Style> newStyle) noexcept {
	style_ = std::move(newStyle);
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::_fire(std::type_index type, const void* event_ptr) {
	// NOTE: isAttached() is an experiment to stop early events that would occur in setup (attach) codes
	if (isAttached() && flags.match_any(Flag::signal))
		context().event.fire(this, type, event_ptr);
}

// -------------------------------------------------------------------------------------------------

ContextStyle CoreComponent::makeStyleContext() noexcept {
	return ContextStyle{libv::make_observer(style_.get()), *this};
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::connect(CoreComponent& signal, CoreComponent& slot, std::type_index type, std::function<void(void*, const void*)>&& callback) {
	signal.context().event.connect(&signal, &slot, type, std::move(callback));

	signal.flagDirect(Flag::signal);
	slot.flagDirect(Flag::slot);
}

// -------------------------------------------------------------------------------------------------

bool CoreComponent::isFocusableComponent() const noexcept {
	return !flags.match_any(Flag::pendingDetachSelf) &&
			flags.match_any(Flag::render) &&
			flags.match_any(Flag::focusableSelf);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::eventChar(CoreComponent& component, const EventChar& event) {
	for (auto i = libv::make_observer_ref(component); i != i->parent; i = i->parent) {
		if (i->flags.match_any(Flag::watchChar))
			i->onChar(event);
		if (event.propagation_stopped())
			return;
	}
}

void CoreComponent::eventKey(CoreComponent& component, const EventKey& event) {
	for (auto i = libv::make_observer_ref(component); i != i->parent; i = i->parent) {
		if (i->flags.match_any(Flag::watchKey))
			i->onKey(event);
		if (event.propagation_stopped())
			return;
	}
}

void CoreComponent::focusGain(CoreComponent& component) {
	if (component.flags.match_any(Flag::watchFocus))
		component.onFocus(EventFocus{true});
	component.flags.set(Flag::focused);
}

void CoreComponent::focusLoss(CoreComponent& component) {
	if (component.flags.match_any(Flag::watchFocus))
		component.onFocus(EventFocus{false});
	component.flags.reset(Flag::focused);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::onChar(const EventChar& event) {
	(void) event;
}

void CoreComponent::onKey(const EventKey& event) {
	(void) event;
}

void CoreComponent::onFocus(const EventFocus& event) {
	(void) event;
}

void CoreComponent::onMouseButton(const EventMouseButton& event) {
	(void) event;
}

void CoreComponent::onMouseMovement(const EventMouseMovement& event) {
	(void) event;
}

void CoreComponent::onMouseScroll(const EventMouseScroll& event) {
	(void) event;
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::attach(CoreComponent& parent_) {
	if (flags.match_any(Flag::pendingAttachSelf)) {
		// NOTE: context_ is already set in the constructor
		parent = parent_;

		log_ui.trace("Attaching {}", path());

		doAttach();

		flagAncestors(calculatePropagateFlags(flags)); // Trigger flag propagation

		if (flags.match_any(Flag::watchMouse))
			context().mouse.subscribe(*this);

		flags.reset(Flag::pendingAttachSelf);
	}

	if (flags.match_any(Flag::pendingAttachChild)) {
		doForeachChildren([this](CoreComponent& child) {
			if (child.flags.match_any(Flag::pendingAttach))
				child.attach(*this);
		});
		flags.reset(Flag::pendingAttachChild);
	}
}

void CoreComponent::detach(CoreComponent& parent_) {
	if (flags.match_any(Flag::pendingDetachChild)) {
		Flag_t childFlags = Flag::none;

		doDetachChildren([this, &parent_, &childFlags](CoreComponent& child) {
			bool remove = child.flags.match_any(Flag::pendingDetachSelf);

			if (child.flags.match_any(Flag::pendingDetach))
				child.detach(*this);

			if (!remove)
				childFlags |= child.flags;

			return remove;
		});

		flags.reset(Flag::mask_propagate);
		flags.set(calculatePropagateFlags(childFlags));
	}

	if (flags.match_any(Flag::pendingDetachSelf)) {
		log_ui.trace("Detaching {}", path());

		if (flags.match_any(Flag::watchMouse))
			context().mouse.unsubscribe(*this);

		if (flags.match_any(Flag::focused)) {
			flags.reset(Flag::focusable);
			context().detachFocused(*this);
		}

//		if (flags.match_any(Flag::focusLinked))
//			context().detachFocusLinked(*this);

		if (flags.match_any(Flag::signal))
			context().event.disconnect_signal(this);

		if (flags.match_any(Flag::slot))
			context().event.disconnect_slot(this);

		doDetach();

		flags.reset(Flag::pendingDetach | Flag::signal | Flag::slot);

		childID = 0;
		flags = Flag::mask_init;
		parent = *this;
		layout_position_ = {};
		layout_size_ = {};
	}
}

void CoreComponent::style() {
	if (flags.match_any(Flag::pendingStyleSelf)) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		if (parent != this) // NOTE: Condition is required to Avoid root component edge case
			parent->doStyle(ctx, childID);
		flags.reset(Flag::pendingStyleSelf);
	}
	if (flags.match_any(Flag::pendingStyleChild)) {
		doForeachChildren([](CoreComponent& child) {
			if (child.flags.match_any(Flag::pendingStyle))
				child.style();
		});
		flags.reset(Flag::pendingStyleChild);
	}
}

void CoreComponent::styleScan() {
	if (flags.match_any(Flag::pendingStyleSelf) || (style_ && style_->isDirty())) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		parent->doStyle(ctx, childID);
	}
	doForeachChildren([](CoreComponent& child) {
		child.styleScan();
	});
	flags.reset(Flag::pendingStyle);
}

libv::observer_ptr<CoreComponent> CoreComponent::focusTraverse(const ContextFocusTraverse& context) {
	// Algorithm driver method, does not directly recurse, up walking

	libv::observer_ptr<CoreComponent> result = doFocusTraverse(context, ChildIDSelf);
	libv::observer_ref<CoreComponent> ancestor = *this;

	while (result == nullptr && ancestor != ancestor->parent) {
		result = ancestor->parent->doFocusTraverse(context, ancestor->childID);
		ancestor = ancestor->parent;
	}

	return result;
}

void CoreComponent::render(ContextRender& context) {
	if (isRendered()) {
		context.changedLayout = flags.match_any(Flag::updatedPosition | Flag::updatedSize);
		context.changedPosition = flags.match_any(Flag::updatedPosition);
		context.changedSize = flags.match_any(Flag::updatedSize);

		if (flags.match_any(Flag::pendingCreate)) {
			doCreate(context);
			flags.reset(Flag::pendingCreate);
		}

		{
	//	if (flags.match_any(Flag::pendingRender)) {
			doRender(context);

			doForeachChildren([&context](CoreComponent& child) {
				child.render(context);
			});
	//		flags.reset(Flag::pendingRender);
		}

		flags.reset(Flag::updatedPosition | Flag::updatedSize);
	}

	if (flags.match_any(Flag::pendingDetachSelf)) {
		doDestroy(context);
		return;
	}
}

libv::vec3f CoreComponent::layout1(const ContextLayout1& environment) {
	const auto result = doLayout1(environment);
	log_ui.trace("Layout dynamic {:>11}, {}", result, path());
	return result;
}

void CoreComponent::layout2(const ContextLayout2& environment) {
//	log_ui.trace("Layout Pass2 {}", path());
	bool boundsChanged = false;

	if (environment.position != layout_position_) {
		boundsChanged = true;
		flags.set(Flag::updatedPosition);
		layout_position_ = environment.position;
	}

	if (environment.size != layout_size_) {
		boundsChanged = true;
		flags.set(Flag::updatedSize);
		layout_size_ = environment.size;
	}

	if (boundsChanged && flags.match_any(Flag::watchMouse)) {
		context().mouse.update(
			*this,
			libv::vec::xy(environment.position),
			libv::vec::xy(environment.size),
			environment.mouseOrder);
	}

	if (boundsChanged || flags.match_any(Flag::pendingLayout)) {

		if (boundsChanged || flags.match_any(Flag::pendingLayoutSelf)) {
			// Layout self
			doLayout2(environment);
			log_ui.trace("Layout {:>11}, {:>11}, {}", layout_position_, layout_size_, path());

		} else {
			// Layout the children only
			doForeachChildren([&environment](CoreComponent& child) {
				child.layout2(ContextLayout2{child.layout_position_, child.layout_size_, MouseOrder{libv::to_value(environment.mouseOrder) + 1}});
			});
			log_ui.trace("   |   {:>11}, {:>11}, {}", layout_position_, layout_size_, path());
		}

		flags.reset(Flag::pendingLayout);
	}
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::doAttach() { }

void CoreComponent::doDetach() { }

void CoreComponent::doDetachChildren(libv::function_ref<bool(CoreComponent&)> callback) {
	(void) callback;
}

void CoreComponent::doStyle(ContextStyle& context) {
	(void) context;
}

void CoreComponent::doStyle(ContextStyle& context, ChildID childID) {
	(void) context;
	(void) childID;
}

libv::observer_ptr<CoreComponent> CoreComponent::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
	(void) context;

	if (current == ChildIDSelf || !isFocusableComponent())
		return nullptr;

	return libv::make_observer(this);
}

void CoreComponent::doCreate(ContextRender& context) {
	(void) context;
}

void CoreComponent::doDestroy(ContextRender& context) {
	(void) context;
}

void CoreComponent::doRender(ContextRender& context) {
	(void) context;
}

libv::vec3f CoreComponent::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	return {};
}

void CoreComponent::doLayout2(const ContextLayout2& environment) {
	(void) environment;
}

void CoreComponent::doForeachChildren(libv::function_ref<bool(CoreComponent&)> callback) {
	(void) callback;
}

void CoreComponent::doForeachChildren(libv::function_ref<void(CoreComponent&)> callback) {
	(void) callback;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
