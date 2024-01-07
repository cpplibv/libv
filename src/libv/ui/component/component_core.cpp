// Project: libv.ui, File: src/libv/ui/component/component_core.cpp

// hpp
#include <libv/ui/component/component_core.hpp>
// ext
#include <range/v3/view/reverse.hpp>
// pro
#include <libv/container/small_vector.hpp>
#include <libv/ui/component_system/core_ptr.hpp>
#include <libv/ui/context/context_event.hpp>
#include <libv/ui/context/context_focus.hpp>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/event/event_component.hpp>
#include <libv/ui/event/event_enable.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/style/style.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

libv::Nexus& get_nexus(const Component& component) noexcept {
	return get_core(component)->ui().event.nexus;
}

void mark_as_signal(Component& component) noexcept {
	get_core(component)->markAsSignal();
}

void mark_as_slot(Component& component) noexcept {
	get_core(component)->markAsSlot();
}

bool is_marked_as_signal(const Component& component) noexcept {
	return get_core(component)->isSignal();
}

bool is_marked_as_slot(const Component& component) noexcept {
	return get_core(component)->isSlot();
}

// ---

libv::Nexus& get_nexus(...) noexcept {
	return current_thread_context().event.nexus;
}

// =================================================================================================

CoreComponent::CoreComponent(std::string name) :
	ui_(&current_thread_context()),
	name(std::move(name)) { }

CoreComponent::~CoreComponent() {
	log_ui.error_if(isAttached(), "Component is destructed in an attached state: {}", path());
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::childID(ChildID id) noexcept {
	style_state(StyleState::first, id == 0);
	// style_state(StyleState::last, id == parent->???); // TODO P5: child index style selector for last
	style_state(StyleState::mod2, id % 2 == 0);
	style_state(StyleState::mod3, id % 3 == 0);
	childID_ = id;
}

// -------------------------------------------------------------------------------------------------

std::string CoreComponent::path() const {
	//	std::size_t path_length = name.size() + 1;
	//	for
	//		path_length += it->name.size() + 1;
	//
	//	std::size_t i = 1;
	//	std::string result(path_length, '/');
	//	for
	//  	strcpy i parent.name
	//  	i += parent.name.size()

	// ---

//	std::string result = name;
//
//	// Iterate every component except the root
//	for (auto it = parent_; it != it->parent_; it = it->parent_)
//		result = it->name + '/' + std::move(result);
//
//	// Place the root marker in the front
//	result = '/' + std::move(result);

	// ---

	std::string result = std::to_string(childID_) + ':' + name;

	// Iterate every component except the root
	for (auto it = parent_; it != it->parent_; it = it->parent_)
		result = std::to_string(it->childID_) + ':' + it->name + '/' + std::move(result);

	// Place the root marker in the front
	result = '/' + std::move(result);

	// ---

	return result;
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::flagAncestors(Flag_t flags_) noexcept {
	for (auto it = parent_; !it->flags.match_mask(flags_); it = it->parent_)
		it->flags.set(flags_);
}

void CoreComponent::flagDirect(Flag_t flags_) noexcept {
	flags.set(flags_);
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

	for (auto it = parent_; true; it = it->parent_) {
		Flag_t childFlags = Flag::none;

		it->doForeachChildren([&childFlags](Component& child) {
			childFlags |= get_core(child)->flags;
		});

		if (it->flags.match_mask(Flag::mask_propagate, childFlags))
			return;

		it->flags.reset(Flag::mask_propagate);
		it->flags.set(calculatePropagateFlags(childFlags));

		if (it == it->parent_)
			break;
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
		flagAuto(Flag::focusable);
	} else {
		if (!flags.match_any(Flag::focusableSelf))
			return;

		if (flags.match_mask(Flag::focusableChild))
			flags.reset(Flag::focusableSelf); // There is a child with focusable, no need to purge flag
		else
			flagPurge(Flag::focusable);

		if (isAttached() && flags.match_any(Flag::focused))
			ui().focus.detachFocused(*this);
	}
}

void CoreComponent::watchMouse(bool value) noexcept {
	if (flags.match_full(Flag::watchMouse) == value)
		return;

	if (isAttached()) { // Skip subscribe if not yet attached or not yet attaching
		if (value) {
			ui().mouse.subscribe(*this);
			if (flags.match_any(Flag::disabled))
				ui().mouse.enable(*this, false);
		} else {
			ui().mouse.unsubscribe(*this);
		}
	}

	flags.set_to(Flag::watchMouse, value);
}

void CoreComponent::floatRegion(bool value) noexcept {
	if (flags.match_full(Flag::floatRegion) == value)
		return;

	if (isAttached()) { // Skip subscribe if not yet attached or not yet attaching
		if (value) {
			ui().mouse.subscribe_region(*this);
			if (flags.match_any(Flag::disabled))
				ui().mouse.enable(*this, false);
		} else {
			ui().mouse.unsubscribe_region(*this);
		}
	}

	flags.set_to(Flag::floatRegion, value);
}

bool CoreComponent::isWatchChar() const noexcept {
	return flags.match_any(Flag::watchChar);
}

bool CoreComponent::isWatchKey() const noexcept {
	return flags.match_any(Flag::watchKey);
}

bool CoreComponent::isWatchFocus() const noexcept {
	return flags.match_any(Flag::focusableSelf);
}

bool CoreComponent::isWatchMouse() const noexcept {
	return flags.match_any(Flag::watchMouse);
}

bool CoreComponent::isFloatRegion() const noexcept {
	return flags.match_any(Flag::floatRegion);
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::show(bool value) noexcept {
	if (show() == value)
		return;

	flags.set_to(Flag::render, value);
	flagAuto(Flag::pendingRender);
}

bool CoreComponent::show() const noexcept {
	return flags.match_any(Flag::render);
}

void CoreComponent::auxEnableScan(bool value) noexcept {
	if (enable() == value)
		return;

	if (flags.match_any(Flag::disableControlled))
		return;

	flags.set_to(Flag::disabled, !value);
	style_state(StyleState::disable, !value);

	if (isAttached()) {
		if (flags.match_any(Flag::watchMouse | Flag::floatRegion))
			ui().mouse.enable(*this, value);

		fire(EventEnable{value});
	}

	doForeachChildren([value](Component& child) {
		get_core(child)->auxEnableScan(value);
	});
}

void CoreComponent::enable(bool value) noexcept {

	if (enable() == value) {
		flags.set_to(Flag::disableControlled, !value);
		return;
	}

	flags.set_to(Flag::disabled | Flag::disableControlled, !value);
	style_state(StyleState::disable, !value);

	if (isAttached()) {
		if (flags.match_any(Flag::watchMouse | Flag::floatRegion))
			ui().mouse.enable(*this, value);

		fire(EventEnable{value});
	}

	// Note: Full tree traversal could be optimized out but style states, enable getter function and (the possibly sparse) mouse watching children would complicate it
	doForeachChildren([value](Component& child) {
		get_core(child)->auxEnableScan(value);
	});
}

bool CoreComponent::enable() const noexcept {
	return !flags.match_any(Flag::disabled);
}

void CoreComponent::focus(FocusMode mode) noexcept {
	// Note: focus has to be sync to allow correct event processing
	if (!isAttached())
		log_ui.error("Attempted to focus a non-attached component. Attempt ignored. {}", path());

	else if (!isFocusableComponent())
		log_ui.error("Attempted to focus a non-focusable component. Attempt ignored. {}", path());

	else
		ui().focus.focus(*this, mode);
}

void CoreComponent::markRemove() noexcept {
	if (flags.match_full(Flag::pendingDetachSelf))
		return; // Early return optimization if we are already marked for remove

	flags.reset(Flag::layout | Flag::render);
	flagAuto(Flag::pendingDetach);

	// Parent has to be relayouted (could be optimized with another parentDependOnRemove like flag)
	markInvalidLayout(false, true);
}

void CoreComponent::markInvalidLayout(bool invalidate_layout1, bool invalidate_parent_layout) noexcept {

	// Invalidate self
	auto it = this;
	it->flagDirect(Flag::pendingLayoutSelf);

	// Invalidate direct parent, if we might changed our size
	if (invalidate_parent_layout || (invalidate_layout1 && it->parent_->flags.match_any(Flag::layoutDependsOnContent))) {
		// If size could have changed, our parent has to re-layout us
		it = it->parent_;
		it->flagDirect(Flag::pendingLayoutSelf | Flag::pendingLayoutChild);
	}

	// Invalidate anyone upstream whom might depend on our layout
	// NOTE: Testing self and flag the parent as parent has to re-layout us
	while (it->flags.match_any(Flag::layoutDependsOnContent)) {
		it = it->parent_;

		if (it->flags.match_any(Flag::pendingLayoutSelf)) {
			it->flagDirect(Flag::pendingLayoutChild);
			return; // Already has invalid self flag, upstream already know what to do, make sure child is set, HARD STOP
		}

		it->flagDirect(Flag::pendingLayoutSelf | Flag::pendingLayoutChild);

		if (it == it->parent_)
			return; // Reached root
	}

	// Indicate that there is an invalid children downstream
	while (true) {
		it = it->parent_;

		if (it->flags.match_any(Flag::pendingLayoutChild))
			return; // Already has invalid child flag, upstream already know what to do

		it->flagDirect(Flag::pendingLayoutChild);

		if (it == it->parent_)
			return; // Reached root
	}
}

void CoreComponent::style(libv::intrusive2_ptr<Style> newStyle) noexcept {
	style_ = std::move(newStyle);
	flagAuto(Flag::pendingStyle);
}

void CoreComponent::style(std::string_view style_name) {
	style(ui().style(style_name));
}

void CoreComponent::style_state(StyleState state_mask, bool value) noexcept {
	if (value && (state_mask == (style_state_ & state_mask)))
		return;
	if (!value && (StyleState::none == (style_state_ & state_mask)))
		return;

	style_state_ = value ? style_state_ | state_mask : style_state_ & ~state_mask;
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

libv::vec2f CoreComponent::calculate_local_mouse_coord() const noexcept {
	const auto mouse_global_coord = ui().state.mouse_position();
	const auto mouse_global_offset = ui().mouse.get_global_position(*this);
	const auto mouse_local_coord = mouse_global_coord - mouse_global_offset;
	return mouse_local_coord;
}

// -------------------------------------------------------------------------------------------------

bool CoreComponent::isFocusableComponent() const noexcept {
	if (!flags.match_any(Flag::focusableSelf))
		return false;

//	if (!flags.match_any(Flag::render))
//		return false;

	if (flags.match_any(Flag::disabled))
		return false;

	for (auto it = make_observer_ref(this); true; it = it->parent_) {
		if (it->flags.match_any(Flag::pendingDetachSelf))
			return false;
		if (it == it->parent_)
			break;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::eventMouseMovement(const EventMouseMovement& event) {
	if (event.enter)
		style_state(StyleState::hover, true);

	if (event.leave)
		style_state(StyleState::hover, false);

	onMouseMovement(event);
}

void CoreComponent::eventChar(CoreComponent& component, const EventChar& event) {
	for (auto it = make_observer_ref(&component); true; it = it->parent_) {
		if (it->flags.match_any(Flag::watchChar))
			it->onChar(event);
		if (event.propagation_stopped())
			break;
		if (it == it->parent_)
			break;
	}
}

void CoreComponent::eventKey(CoreComponent& component, const EventKey& event) {
	for (auto it = make_observer_ref(&component); true; it = it->parent_) {
		if (it->flags.match_any(Flag::watchKey))
			it->onKey(event);
		if (event.propagation_stopped())
			break;
		if (it == it->parent_)
			break;
	}
}

void CoreComponent::focusChange(CoreComponent* componentOld, bool activeOld, CoreComponent* componentNew, bool activeNew) {
	// Focus loss
	if (componentOld) {
		componentOld->flags.reset(Flag::focused);
		componentOld->style_state(StyleState::focus, false);

		if (activeOld) {
			const auto event = EventFocus(false);
			componentOld->onFocus(event);
			componentOld->fire(event);
		}
	}

	// Focus-within find common ancestors
	libv::small_vector<CoreComponent*, 32> withinNews;
	if (componentNew) {
		for (auto it = componentNew->parent_; true; it = it->parent_) {
			if ((it->style_state() & StyleState::focus_within) != StyleState::none)
				break;
			withinNews.emplace_back(it);
			if (it == it->parent_)
				break;
		}
	}
	const CoreComponent* commonAncestor =
			!withinNews.empty() ? withinNews.back()->parent_ :
			componentNew && (componentNew->style_state() & StyleState::focus_within) != StyleState::none ? componentNew :
			componentNew ? componentNew->parent_ :
			nullptr;

	// Focus-within loss from componentOld to commonAncestors
	if (componentOld && componentOld != commonAncestor) {
		for (auto it = componentOld->parent_; true; it = it->parent_) {
			if (it == commonAncestor)
				break;
			const auto event = EventFocus(false);
			it->onFocusWithin(event);
			it->style_state(StyleState::focus_within, false);
			if (it == it->parent_)
				break;
		}
	}

	// Focus-within gain from commonAncestors to componentNew
	for (const auto& it : withinNews | ranges::views::reverse) {
		const auto event = EventFocus(true);
		it->onFocusWithin(event);
		it->style_state(StyleState::focus_within, true);
	}

	// Focus gain
	if (componentNew) {
		assert(componentNew->flags.match_any(Flag::focusableSelf));
		componentNew->flags.set(Flag::focused);
		componentNew->style_state(StyleState::focus, activeNew);

		if (activeNew) {
			const auto event = EventFocus(true);
			componentNew->onFocus(event);
			componentNew->fire(event);
		}
	}
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

void CoreComponent::onFocusWithin(const EventFocus& event) {
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

void CoreComponent::attach(CoreComponent& new_parent) {
	if (flags.match_any(Flag::pendingAttachSelf)) {
		// NOTE: context_ is already set in the constructor
		parent_ = make_observer_ref(&new_parent);
		log_ui.trace("Attaching {}", path());

		// Inherit flags from parent
		if (parent_->flags.match_any(Flag::disabled))
			flags.set(Flag::disabled);

		// Attach to mouse context
		if (flags.match_any(Flag::watchMouse))
			ui().mouse.subscribe(*this);

		if (flags.match_any(Flag::floatRegion))
			ui().mouse.subscribe_region(*this);

		if (flags.match_any(Flag::watchMouse | Flag::floatRegion) && flags.match_any(Flag::disabled))
			ui().mouse.enable(*this, false);

		// Broadcast events
		doAttach();
		fire(EventAttach{});

		// Maintain flags
		flagAncestors(calculatePropagateFlags(flags)); // Trigger flag propagation

		flags.reset(Flag::pendingAttachSelf);
	}

	if (flags.match_any(Flag::pendingAttachChild)) {
		doForeachChildren([this](Component& child) {
			if (get_core(child)->flags.match_any(Flag::pendingAttach))
				get_core(child)->attach(*this);
		});
		flags.reset(Flag::pendingAttachChild);
	}
}

void CoreComponent::detachScan() {
	if (flags.match_any(Flag::pendingDetachSelf)) {
		detach();

	} else if (flags.match_any(Flag::pendingDetachChild)) {
		Flag_t childFlags = Flag::none;

		// Note: flag reset happens before doDetachChildren so any invalidation event (like pendingStyle)
		//			that concerns a children that was already passed (so childFlags did not account for it)
		//			is not discarded by it
		flags.reset(Flag::mask_propagate);

		doDetachChildren([this, &childFlags](Component& child) {
			const bool remove = get_core(child)->flags.match_any(Flag::pendingDetachSelf);

			if (get_core(child)->flags.match_any(Flag::pendingDetach))
				get_core(child)->detachScan();

			if (!remove)
				childFlags |= get_core(child)->flags;

			return remove;
		});

		flags.set(calculatePropagateFlags(childFlags));
	}
}

void CoreComponent::detach() {
	log_ui.trace("Detaching {}", path());

	doDetachChildren([](Component& child) {
		get_core(child)->detach();
		return true;
	});

	if (flags.match_any(Flag::watchMouse))
		ui().mouse.unsubscribe(*this);

	if (flags.match_any(Flag::floatRegion))
		ui().mouse.unsubscribe_region(*this);

	if (flags.match_any(Flag::focusable))
		flagPurge(Flag::focusable);

	if (flags.match_any(Flag::focused))
		ui().focus.detachFocused(*this);

//	if (flags.match_any(Flag::focusLinked))
//		uixt().detachFocusLinked(*this);

	fire(EventDetach{});

	if (flags.match_any(Flag::signal | Flag::slot))
		ui().event.nexus.disconnect_all(this);

	doDetach();

	childID_ = 0;
	style_state_ = StyleState::none;
	flags = Flag::mask_init;
	parent_ = make_observer_ref(this);
	layout_position_ = {};
	layout_size_ = {};
}

void CoreComponent::update() {
	if (flags.match_any(Flag::pendingDetachSelf))
		return; // Do not update detaching components

	doUpdate();

	doForeachChildren([](Component& child) {
		get_core(child)->update();
	});
}

void CoreComponent::styleScan() {
	const auto pendingSelf = flags.match_any(Flag::pendingStyleSelf);
	const auto pendingChild = flags.match_any(Flag::pendingStyleChild);

	if (!pendingSelf && !pendingChild)
		return;

	const auto modeSelf = pendingSelf ? StyleAccess::AccessModeSelf::style : StyleAccess::AccessModeSelf::noop;
	const auto modeChild = pendingChild ? StyleAccess::AccessModeChild::dirty : StyleAccess::AccessModeChild::none;

	StyleAccess sa{modeSelf, modeChild};
	doStyle(sa);
	if (pendingSelf && parent_ != this) // NOTE: parent_ != this Condition is required to avoid root component edge case (Root is a container and not a child of itself)
		parent_->doStyleChild(sa, childID_);

	flags.reset(Flag::pendingStyle);
}

void CoreComponent::styleScanAll() {
	const auto pendingSelf = flags.match_any(Flag::pendingStyleSelf) || (style_ && style_->isDirty());

	const auto modeSelf = pendingSelf ? StyleAccess::AccessModeSelf::style : StyleAccess::AccessModeSelf::noop;
	const auto modeChild = StyleAccess::AccessModeChild::all;

	StyleAccess sa{modeSelf, modeChild};
	doStyle(sa);
	if (pendingSelf && parent_ != this) // NOTE: parent_ != this Condition is required to avoid root component edge case (Root is a container and not a child of itself)
		parent_->doStyleChild(sa, childID_);

	flags.reset(Flag::pendingStyle);
}

libv::observer_ptr<CoreComponent> CoreComponent::focusTraverse(const ContextFocusTraverse& context) {
	// Algorithm driver method, does not directly recurse, only does the up walking

	libv::observer_ptr<CoreComponent> result = doFocusTraverse(context, ChildIDSelf);
	libv::observer_ref<CoreComponent> ancestor = make_observer_ref(this);

	while (result == nullptr && ancestor != ancestor->parent_) {
		result = ancestor->parent_->doFocusTraverse(context, ancestor->childID_);
		ancestor = ancestor->parent_;
	}

	return result;
}

libv::vec2f CoreComponent::layout1(const ContextLayout1& layoutEnv) {
	log_ui.trace("Dynamic enter                     , unlimited {} limit {} parent {} {}", layoutEnv.unlimited, layoutEnv.limit, layoutEnv.parent, path());
	const auto result = doLayout1(layoutEnv);
	log_ui.trace("Dynamic result           {: :>4}, unlimited {} limit {} parent {} {}", result, layoutEnv.unlimited, layoutEnv.limit, layoutEnv.parent, path());
	return result;
}

void CoreComponent::layout2FloatPositionUpdateScan(libv::vec2f floatPosition, int32_t depth) {
	if (flags.match_any(Flag::floatRegion))
		// Float positions can't change under a float region component
		return;

	log_ui.trace("MScan   {: :>4}, {: :>4}, A {}", layout_position_, layout_size_, path());

	doForeachChildren([parentFloatPosition = floatPosition, parentDepth = depth](Component& child) {
		// Scan children and mouse update if needed
		auto& child_core = *get_core(child);

		const auto childFloatPosition = child_core.layout_position() + parentFloatPosition;
		const auto size = child_core.layout_size();
		const auto childDepth = parentDepth + 1;

		if (child_core.flags.match_any(Flag::watchMouse | Flag::floatRegion))
			child_core.ui().mouse.update(child_core, childFloatPosition, size, MouseOrder{childDepth + child_core.property.z_index_offset()});

		child_core.layout2FloatPositionUpdateScan(childFloatPosition, childDepth);
	});
}

void CoreComponent::layout2(const ContextLayout2& layoutEnv) {
	const bool changedSize = layoutEnv.size != layout_size_;
	const bool changedPosition = layoutEnv.position != layout_position_;
	const bool changedBounds = changedSize || changedPosition;

	const bool changedFloatPosition = layoutEnv.float_position_changed || changedPosition;

	if (changedPosition) {
		flags.set(Flag::updatedPosition);
		layout_position_ = layoutEnv.position;
	}

	if (changedSize) {
		flags.set(Flag::updatedSize);
		layout_size_ = layoutEnv.size;
	}

	if (changedBounds)
		flagAuto(Flag::pendingRender);

	if ((changedFloatPosition || changedBounds) && flags.match_any(Flag::watchMouse | Flag::floatRegion))
		// We update the mouse context
		ui().mouse.update(*this, layoutEnv.float_position, layoutEnv.size, MouseOrder{layoutEnv.depth + property.z_index_offset()});

	if (!changedFloatPosition && !changedBounds && !flags.match_any(Flag::pendingLayout))
		// No need to re-layout this component sub-tree
		return;

	if (changedFloatPosition && !changedBounds && !flags.match_any(Flag::pendingLayout)) {
		// No need to re-layout this component sub-tree, but the float position changed
		// so mouse context has to be updated
		layout2FloatPositionUpdateScan(layoutEnv.float_position, layoutEnv.depth);
		return;
	}

	// Update float_position_changed based on this component and the current layout request
	layoutEnv.float_position_changed = (layoutEnv.float_position_changed || changedFloatPosition) && !flags.match_any(Flag::floatRegion);

	if (changedBounds || flags.match_any(Flag::pendingLayoutSelf)) {
		// Layout self and the children with the derived class
		// TODO P5: A way to print, log or visualize layout events without always printing it into the log
		// if (enableUILayoutTrace)
		log_ui.trace("Layout enter  {: :>4}, {: :>4}, {} {}", layoutEnv.position, layoutEnv.size, layoutEnv.float_position_changed ? "A" : " ", path());
		doLayout2(layoutEnv);
		log_ui.trace("Layout result {: :>4}, {: :>4}, {} {}", layout_position_, layout_size_, layoutEnv.float_position_changed ? "A" : " ", path());
	} else if (flags.match_any(Flag::pendingLayoutChild)) {
		log_ui.trace("   |   enter  {: :>4}, {: :>4}, {} {}", layout_position_, layout_size_, layoutEnv.float_position_changed ? "A" : " ", path());
		// No need to re-layout this component, layout the children only
		doForeachChildren([&layoutEnv, changedFloatPosition](Component& child) {
			get_core(child)->layout2(layoutEnv.enter(
					get_core(child)->layout_position(),
					get_core(child)->layout_size()
			));
		});
		log_ui.trace("   |   result {: :>4}, {: :>4}, {} {}", layout_position_, layout_size_, layoutEnv.float_position_changed ? "A" : " ", path());
	}

	flags.reset(Flag::pendingLayout);
}

void CoreComponent::render(Renderer& r) {
	if (isRendered()) {
		r.changed_layout = flags.match_any(Flag::updatedPosition | Flag::updatedSize);
		r.changed_position = flags.match_any(Flag::updatedPosition);
		r.changed_size = flags.match_any(Flag::updatedSize);

		if (flags.match_any(Flag::pendingCreate)) {
			doCreate(r);
			flags.reset(Flag::pendingCreate);
		}

		{ // if (flags.match_any(Flag::pendingRender)) {
			const auto offset = layout_position();
			r.translate(offset);

			doRender(r);

			// TODO P4: Renderer proper translate guarding (same in scroll area)
			r.translate(-offset);
		}

		flags.reset(Flag::updatedPosition | Flag::updatedSize | Flag::pendingRender);
	}

	if (flags.match_any(Flag::pendingDetachSelf)) {
		renderDestroy(r);
	}
}

void CoreComponent::renderDestroy(Renderer& r) {
	doForeachChildren([&r](Component& child) {
		// For now we can get away with not entering render contexts
		get_core(child)->renderDestroy(r);

		//Renderer rc = r.enter(child);
		//get_core(child)->renderDestroy(rc);
	});

	doDestroy(r);
}

// -------------------------------------------------------------------------------------------------

libv::vec4f CoreComponent::getInnerContentBounds() {
	return {padding_LB(), layout_size() - padding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::doAttach() { }

void CoreComponent::doDetach() { }

void CoreComponent::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	(void) callback;
}

void CoreComponent::doStyle(StyleAccess& access) {
	access.self(*this);
}

void CoreComponent::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

libv::observer_ptr<CoreComponent> CoreComponent::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
	(void) context;

	if (current == ChildIDSelf || !isFocusableComponent())
		return nullptr;

	return libv::make_observer_ptr(this);
}

libv::vec2f CoreComponent::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	return padding_size();
}

void CoreComponent::doLayout2(const ContextLayout2& environment) {
	(void) environment;
}

void CoreComponent::doUpdate() {
}

void CoreComponent::doCreate(Renderer& context) {
	(void) context;
}

void CoreComponent::doRender(Renderer& context) {
	(void) context;
}

void CoreComponent::doDestroy(Renderer& context) {
	(void) context;
}

void CoreComponent::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	(void) callback;
}

void CoreComponent::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	(void) callback;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
