// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/base_component.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <cassert>
// pro
#include <libv/ui/context_focus_travers.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

BaseComponent::BaseComponent(std::string name) :
	name(std::move(name)) { }

BaseComponent::BaseComponent(UnnamedTag_t, const std::string_view type) :
	name(libv::concat(type, '-', nextID++)) { }

BaseComponent::~BaseComponent() {
	log_ui.error_if(isAttached(), "Component is destructed in an attached state: {}", path());
}

// -------------------------------------------------------------------------------------------------

std::string BaseComponent::path() const {
	std::string result = name;

	for (auto it = make_observer_ref(this); it != it->parent; it = it->parent)
		result = it->parent->name + '/' + std::move(result);

	return result;
}

ContextUI& BaseComponent::context() const noexcept {
	if (context_ == nullptr) {
		log_ui.fatal("Component has to be attached to acquire its context: {}", path());
		assert(false && "Component has to be attached to acquire its context");
	}
	return *context_;
}

// -------------------------------------------------------------------------------------------------

void BaseComponent::flagDirect(Flag_t flags_) noexcept {
	flags.set(flags_);
}

void BaseComponent::flagAncestors(Flag_t flags_) noexcept {
	for (auto it = parent; !it->flags.match_mask(flags_); it = it->parent)
		it->flags.set(flags_);
}

void BaseComponent::flagAuto(Flag_t flags_) noexcept {
	flagDirect(flags_ & Flag::mask_self);
	flagAncestors(flags_ & Flag::mask_propagate);
}

void BaseComponent::flagForce(Flag_t flags_) noexcept {
	flagDirect(flags_);
	flagAncestors(flags_);
}

void BaseComponent::flagPurge(Flag_t flags_) noexcept {
	flags.reset(flags_ & Flag::mask_self);

	for (auto it = parent; true; it = it->parent) {
		Flag_t childFlags = Flag::none;

		it->doForeachChildren([&childFlags](BaseComponent& child) {
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

void BaseComponent::watchChar(bool value) noexcept {
	if (value)
		flags.set(Flag::watchChar);
	else
		flags.reset(Flag::watchChar);
}

void BaseComponent::watchKey(bool value) noexcept {
	if (value)
		flags.set(Flag::watchKey);
	else
		flags.reset(Flag::watchKey);
}

void BaseComponent::watchFocus(bool value) noexcept {
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

void BaseComponent::watchMouse(Flag_t interest) noexcept {
	if (interest & ~Flag::mask_watchMouse) {
		log_ui.error("Invalid argument passed to mouse watch: {}. Ignoring non mouse watch bits {}", interest, path());
		interest &= Flag::mask_watchMouse;
	}

	// NOTE: pendingAttachSelf flag is used instead of isAttached to allow usage of this function in doAttach
	if (!flags.match_any(Flag::pendingAttachSelf) && flags.match_mask(Flag::mask_watchMouse, interest)) { // Attached and there is a change
		if (!interest) // no watched mouse event left, unsubscribe
			context().mouse.unsubscribe(*this);

		else if (!(flags & Flag::mask_watchMouse)) // start watching mouse events, subscribe
			context().mouse.subscribe(*this, interest);

		else // not subscribe and not unsubscribe but there is a change, update
			context().mouse.update(*this, flags & interest & Flag::mask_watchMouse);
	}

	flags.reset(Flag::mask_watchMouse);
	flags.set(interest);
}

bool BaseComponent::isWatchChar() const noexcept {
	return flags.match_any(Flag::watchChar);
}

bool BaseComponent::isWatchKey() const noexcept {
	return flags.match_any(Flag::watchKey);
}

bool BaseComponent::isWatchFocus() const noexcept {
	return flags.match_any(Flag::watchFocus);
}

Flag_t BaseComponent::isWatchMouse() const noexcept {
	return flags & Flag::mask_watchMouse;
}

// -------------------------------------------------------------------------------------------------

void BaseComponent::focus() noexcept {
	// Note: focus has to be sync to allow correct event processing
	if (!isAttached())
		log_ui.error("Attempted to focus a non-attached component. Attempt ignored. {}", path());

	else if (!isFocusableComponent())
		log_ui.error("Attempted to focus a non-focusable component. Attempt ignored. {}", path());

	else
		context().focus(*this);
}

void BaseComponent::markRemove() noexcept {
	flagAuto(Flag::pendingDetach | Flag::pendingLayout);
	flags.reset(Flag::layout | Flag::render);

	doForeachChildren([](BaseComponent& child) {
		child.markRemove();
	});
}

void BaseComponent::style(libv::intrusive_ptr<Style> newStyle) noexcept {
	style_ = std::move(newStyle);
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

ContextStyle BaseComponent::makeStyleContext() noexcept {
	return ContextStyle{libv::make_observer(style_.get()), *this};
}

// -------------------------------------------------------------------------------------------------

bool BaseComponent::isFocusableComponent() const noexcept {
	return !flags.match_any(Flag::pendingDetachSelf) &&
			flags.match_any(Flag::render) &&
			flags.match_any(Flag::focusableSelf);
}

// -------------------------------------------------------------------------------------------------

void BaseComponent::eventChar(BaseComponent& component, const libv::input::EventChar& event) {
	for (auto i = libv::make_observer_ref(component); i != i->parent; i = i->parent)
		if (i->flags.match_any(Flag::watchChar) && i->onChar(event))
			return;
}

void BaseComponent::eventKey(BaseComponent& component, const libv::input::EventKey& event) {
	for (auto i = libv::make_observer_ref(component); i != i->parent; i = i->parent)
		if (i->flags.match_any(Flag::watchKey) && i->onKey(event))
			return;
}

void BaseComponent::focusChange(BaseComponent& previous, BaseComponent& current) {
	{
		EventFocus event{false, true, libv::make_observer_ref(previous), libv::make_observer_ref(current)};
		if (previous.flags.match_any(Flag::watchFocus))
			previous.onFocus(event);
		previous.flags.reset(Flag::focused);
	}

	{
		current.flags.set(Flag::focused);
		EventFocus event{true, false, libv::make_observer_ref(previous), libv::make_observer_ref(current)};
		if (current.flags.match_any(Flag::watchFocus))
			current.onFocus(event);
	}
}

// -------------------------------------------------------------------------------------------------

bool BaseComponent::onChar(const libv::input::EventChar& event) {
	(void) event;
	return false;
}

bool BaseComponent::onKey(const libv::input::EventKey& event) {
	(void) event;
	return false;
}

void BaseComponent::onFocus(const EventFocus& event) {
	(void) event;
}

bool BaseComponent::onMouse(const EventMouse& event) {
	(void) event;
	return false;
}

// -------------------------------------------------------------------------------------------------

void BaseComponent::attach(BaseComponent& parent_) {
	if (flags.match_any(Flag::pendingAttachSelf)) {
		context_ = libv::make_observer(parent_.context());
		parent = libv::make_observer_ref(parent_);

		log_ui.trace("Attaching {}", path());

		doAttach();

		flagAncestors(calculatePropagateFlags(flags)); // Trigger flag propagation

		if (flags.match_any(Flag::mask_watchMouse))
			context().mouse.subscribe(*this, flags & Flag::mask_watchMouse);

		flags.reset(Flag::pendingAttachSelf);
	}

	if (flags.match_any(Flag::pendingAttachChild)) {
		doForeachChildren([this](BaseComponent& child) {
			if (child.flags.match_any(Flag::pendingAttach))
				child.attach(*this);
		});
		flags.reset(Flag::pendingAttachChild);
	}
}

void BaseComponent::detach(BaseComponent& parent_) {
	if (flags.match_any(Flag::pendingDetachChild)) {
		Flag_t childFlags = Flag::none;

		doDetachChildren([this, &parent_, &childFlags](BaseComponent& child) {
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

		if (flags.match_any(Flag::mask_watchMouse))
			context().mouse.unsubscribe(*this);

		if (flags.match_any(Flag::focused)) {
			flags.reset(Flag::focusable);
			context().detachFocused(*this);
		}

//		if (flags.match_any(Flag::focusLinked))
//			context().detachFocusLinked(*this);

		doDetach();

		context_ = nullptr;
		parent = libv::make_observer_ref(this);

		flags.reset(Flag::pendingDetach);
	}
}

void BaseComponent::style() {
	if (flags.match_any(Flag::pendingStyleSelf)) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		parent->doStyle(ctx, childID);
		flags.reset(Flag::pendingStyleSelf);
	}
	if (flags.match_any(Flag::pendingStyleChild)) {
		doForeachChildren([](BaseComponent& child) {
			if (child.flags.match_any(Flag::pendingStyle))
				child.style();
		});
		flags.reset(Flag::pendingStyleChild);
	}
}

void BaseComponent::styleScan() {
	if (flags.match_any(Flag::pendingStyleSelf) || (style_ && style_->isDirty())) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		parent->doStyle(ctx, childID);
	}
	doForeachChildren([](BaseComponent& child) {
		child.styleScan();
	});
	flags.reset(Flag::pendingStyle);
}

libv::observer_ptr<BaseComponent> BaseComponent::focusTravers(const ContextFocusTravers& context, BaseComponent& current) {
	libv::observer_ptr<BaseComponent> result = current.doFocusTravers(context, ChildIDSelf);

	auto ancestor = libv::make_observer_ref(current);
	while (result == nullptr && ancestor != ancestor->parent) {
		result = ancestor->parent->doFocusTravers(context, ancestor->childID);
		ancestor = ancestor->parent;
	}

	return result;
}

void BaseComponent::render(ContextRender& context) {
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

			doForeachChildren([&context](BaseComponent& child) {
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

void BaseComponent::layout1(const ContextLayout1& environment) {
	bool dirty = flags.match_any(Flag::pendingLayout);

	if (dirty) {
		this->doLayout1(environment);
		log_ui.trace("Layout dynamic {:>11}, {}", lastDynamic, path());
	}
}

void BaseComponent::layout2(const ContextLayout2& environment) {
//	log_ui.trace("Layout Pass2 {}", path());
	bool boundsChanged = false;

	if (environment.position != position_) {
		boundsChanged = true;
		flags.set(Flag::updatedPosition);
		position_ = environment.position;
	}

	if (environment.size != size_) {
		boundsChanged = true;
		flags.set(Flag::updatedSize);
		size_ = environment.size;
	}

	if (boundsChanged && flags.match_any(Flag::mask_watchMouse)) {
		context().mouse.update(
			*this,
			libv::vec::xy(environment.position),
			libv::vec::xy(environment.size),
			environment.mouseOrder);
	}

	if (boundsChanged || flags.match_any(Flag::pendingLayout)) {
		this->doLayout2(environment);

		flags.reset(Flag::pendingLayout);
		log_ui.trace("Layout {:>11}, {:>11}, {}", position_, size_, path());
	}
}

// -------------------------------------------------------------------------------------------------

void BaseComponent::doAttach() { }

void BaseComponent::doDetach() { }

void BaseComponent::doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback) {
	(void) callback;
}

void BaseComponent::doStyle(ContextStyle& context) {
	(void) context;
}

void BaseComponent::doStyle(ContextStyle& context, ChildID childID) {
	(void) context;
	(void) childID;
}

libv::observer_ptr<BaseComponent> BaseComponent::doFocusTravers(const ContextFocusTravers& context, ChildID current) {
	(void) context;

	if (current == ChildIDSelf || !isFocusableComponent())
		return nullptr;

	return libv::make_observer(this);
}

void BaseComponent::doCreate(ContextRender& context) {
	(void) context;
}

void BaseComponent::doDestroy(ContextRender& context) {
	(void) context;
}

void BaseComponent::doRender(ContextRender& context) {
	(void) context;
}

void BaseComponent::doLayout1(const ContextLayout1& environment) {
	(void) environment;
}

void BaseComponent::doLayout2(const ContextLayout2& environment) {
	(void) environment;
}

void BaseComponent::doForeachChildren(libv::function_ref<bool(BaseComponent&)> callback) {
	(void) callback;
}

void BaseComponent::doForeachChildren(libv::function_ref<void(BaseComponent&)> callback) {
	(void) callback;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
