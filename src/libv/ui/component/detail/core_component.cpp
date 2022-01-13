// Project: libv.ui, File: src/libv/ui/component/detail/core_component.cpp

// hpp
#include <libv/ui/component/detail/core_component.hpp>
// pro
#include <libv/ui/component/detail/core_ptr.hpp>
#include <libv/ui/context/context_event.hpp>
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
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

CoreComponent::CoreComponent(std::string name) :
	context_(&current_thread_context()),
	name(std::move(name)) { }

CoreComponent::~CoreComponent() {
	log_ui.error_if(isAttached(), "Component is destructed in an attached state: {}", path());
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

	std::string result = std::to_string(childID) + ':' + name;

	// Iterate every component except the root
	for (auto it = parent_; it != it->parent_; it = it->parent_)
		result = std::to_string(it->childID) + ':' + it->name + '/' + std::move(result);

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

	if (isAttached()) { // Skip subscribe if not yet attached or not yet attaching
		if (value) {
			context().mouse.subscribe(*this);
			if (flags.match_any(Flag::disabled))
				context().mouse.enable(*this, false);
		} else {
			context().mouse.unsubscribe(*this);
		}
	}

	flags.set_to(Flag::watchMouse, value);
}

void CoreComponent::floatRegion(bool value) noexcept {
	if (flags.match_full(Flag::floatRegion) == value)
		return;

	if (isAttached()) { // Skip subscribe if not yet attached or not yet attaching
		if (value) {
			context().mouse.subscribe_region(*this);
			if (flags.match_any(Flag::disabled))
				context().mouse.enable(*this, false);
		} else {
			context().mouse.unsubscribe_region(*this);
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
	return flags.match_any(Flag::watchFocus);
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
			context().mouse.enable(*this, value);

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
			context().mouse.enable(*this, value);

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

void CoreComponent::style(libv::intrusive_ptr<Style> newStyle) noexcept {
	style_ = std::move(newStyle);
	flagAuto(Flag::pendingStyle);
}

void CoreComponent::style(std::string_view style_name) {
	style(context().style(style_name));
}

void CoreComponent::style_state(StyleState state, bool value) noexcept {
	if ((value && style_state_ == (style_state_ & state)) || (!value && (style_state_ & state) == StyleState::none))
		return;

	style_state_ = value ? style_state_ | state : style_state_ & ~state;
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

libv::vec2f CoreComponent::calculate_local_mouse_coord() const noexcept {
	const auto mouse_global_coord = context().state.mouse_position();
	const auto mouse_global_offset = context().mouse.get_global_position(*this);
	const auto mouse_local_coord = mouse_global_coord - mouse_global_offset;
	return mouse_local_coord;
}

// -------------------------------------------------------------------------------------------------

ContextStyle CoreComponent::makeStyleContext() noexcept {
	return ContextStyle{libv::make_observer_ptr(style_.get()), *this};
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

void CoreComponent::focusGain(CoreComponent& component) {
	if (!component.flags.match_any(Flag::watchFocus))
		return;

	component.flags.set(Flag::focused);
	const auto event = EventFocus(true);
	component.onFocus(event);
	component.fire(event);
}

void CoreComponent::focusLoss(CoreComponent& component) {
	if (!component.flags.match_any(Flag::watchFocus))
		return;

	component.flags.reset(Flag::focused);
	const auto event = EventFocus(false);
	component.onFocus(event);
	component.fire(event);
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
			context().mouse.subscribe(*this);

		if (flags.match_any(Flag::floatRegion))
			context().mouse.subscribe_region(*this);

		if (flags.match_any(Flag::watchMouse | Flag::floatRegion) && flags.match_any(Flag::disabled))
			context().mouse.enable(*this, false);

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

		doDetachChildren([this, &childFlags](Component& child) {
			bool remove = get_core(child)->flags.match_any(Flag::pendingDetachSelf);

			if (get_core(child)->flags.match_any(Flag::pendingDetach))
				get_core(child)->detachScan();

			if (!remove)
				childFlags |= get_core(child)->flags;

			return remove;
		});

		flags.reset(Flag::mask_propagate);
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
		context().mouse.unsubscribe(*this);

	if (flags.match_any(Flag::floatRegion))
		context().mouse.unsubscribe_region(*this);

	if (flags.match_any(Flag::focused)) {
		flagPurge(Flag::focusable);
		context().detachFocused(*this);
	}

//	if (flags.match_any(Flag::focusLinked))
//		context().detachFocusLinked(*this);

	fire(EventDetach{});

	detail::internal_disconnect(this);

	doDetach();

	childID = 0;
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

void CoreComponent::style() {
	if (flags.match_any(Flag::pendingStyleSelf)) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		if (parent_ != this) // NOTE: Condition is required to avoid root component edge case
			parent_->doStyle(ctx, childID);
	}
	if (flags.match_any(Flag::pendingStyleChild)) {
		doForeachChildren([](Component& child) {
			if (get_core(child)->flags.match_any(Flag::pendingStyle))
				get_core(child)->style();
		});
	}
	flags.reset(Flag::pendingStyle);
}

void CoreComponent::styleScan() {
	if (flags.match_any(Flag::pendingStyleSelf) || (style_ && style_->isDirty())) {
		ContextStyle ctx = makeStyleContext();
		doStyle(ctx);
		parent_->doStyle(ctx, childID);
	}
	doForeachChildren([](Component& child) {
		get_core(child)->styleScan();
	});
	flags.reset(Flag::pendingStyle);
}

libv::observer_ptr<CoreComponent> CoreComponent::focusTraverse(const ContextFocusTraverse& context) {
	// Algorithm driver method, does not directly recurse, only does the up walking

	libv::observer_ptr<CoreComponent> result = doFocusTraverse(context, ChildIDSelf);
	libv::observer_ref<CoreComponent> ancestor = make_observer_ref(this);

	while (result == nullptr && ancestor != ancestor->parent_) {
		result = ancestor->parent_->doFocusTraverse(context, ancestor->childID);
		ancestor = ancestor->parent_;
	}

	return result;
}

libv::vec3f CoreComponent::layout1(const ContextLayout1& layout_env) {
	const auto result = doLayout1(layout_env);
	log_ui.trace("Layout dynamic {:>11}, {}", result, path());
	return result;
}

void CoreComponent::layout2FloatPositionUpdateScan(libv::vec3f floatPosition, int32_t depth) {
	if (flags.match_any(Flag::floatRegion))
		// Float positions can't change under a float region component
		return;

	log_ui.trace("MScan  {:>11}, {:>11}, A {}", xy(layout_position_), xy(layout_size_), path());

	doForeachChildren([parentFloatPosition = floatPosition, parentDepth = depth](Component& child) {
		// Scan children and mouse update if needed
		auto& child_core = *get_core(child);

		const auto childFloatPosition = child_core.layout_position() + parentFloatPosition;
		const auto size = child_core.layout_size();
		const auto childDepth = parentDepth + 1;

		if (child_core.flags.match_any(Flag::watchMouse | Flag::floatRegion))
			child_core.context().mouse.update(child_core, childFloatPosition, size, libv::ui::MouseOrder{childDepth + child_core.property.z_index_offset()});

		child_core.layout2FloatPositionUpdateScan(childFloatPosition, childDepth);
	});
}

void CoreComponent::layout2(const ContextLayout2& layout_env) {
	bool changedSize = layout_env.size != layout_size_;
	bool changedPosition = layout_env.position != layout_position_;
	bool changedBounds = changedSize || changedPosition;

	bool changedFloatPosition = layout_env.float_position_changed || changedPosition;

	if (changedPosition) {
		flags.set(Flag::updatedPosition);
		layout_position_ = layout_env.position;
	}

	if (changedSize) {
		flags.set(Flag::updatedSize);
		layout_size_ = layout_env.size;
	}

	if (changedBounds)
		flagAuto(Flag::pendingRender);

	if ((changedFloatPosition || changedBounds) && flags.match_any(Flag::watchMouse | Flag::floatRegion))
		// We update the mouse context
		context().mouse.update(*this, layout_env.float_position, layout_env.size, libv::ui::MouseOrder{layout_env.depth + property.z_index_offset()});

	if (!changedFloatPosition && !changedBounds && !flags.match_any(Flag::pendingLayout))
		// No need to re-layout this component sub-tree
		return;

	if (changedFloatPosition && !changedBounds && !flags.match_any(Flag::pendingLayout)) {
		// No need to re-layout this component sub-tree, but the float position changed
		// so mouse context has to be updated
		layout2FloatPositionUpdateScan(layout_env.float_position, layout_env.depth);
		return;
	}

	// Update float_position_changed based on this component and the current layout request
	layout_env.float_position_changed = (layout_env.float_position_changed || changedFloatPosition) && !flags.match_any(Flag::floatRegion);

	if (changedBounds || flags.match_any(Flag::pendingLayoutSelf)) {
		// Layout self and the children with the derived class
		doLayout2(layout_env);
		log_ui.trace("Layout {:>11}, {:>11}, {} {}", xy(layout_position_), xy(layout_size_), layout_env.float_position_changed ? "A" : " ", path());

	} else if (flags.match_any(Flag::pendingLayoutChild)) {
		// No need to re-layout this component, layout the children only
		doForeachChildren([&layout_env, changedFloatPosition](Component& child) {
			get_core(child)->layout2(layout_env.enter(
					get_core(child)->layout_position(),
					get_core(child)->layout_size()
			));
		});
		log_ui.trace("   |   {:>11}, {:>11}, {} {}", xy(layout_position_), xy(layout_size_), layout_env.float_position_changed ? "A" : " ", path());
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
	return {padding_LB(), layout_size2() - padding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreComponent::doAttach() { }

void CoreComponent::doDetach() { }

void CoreComponent::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	(void) callback;
}

void CoreComponent::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreComponent> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
}

void CoreComponent::doStyle(ContextStyle& context, ChildID childID) {
	(void) context;
	(void) childID;
}

libv::observer_ptr<CoreComponent> CoreComponent::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
	(void) context;

	if (current == ChildIDSelf || !isFocusableComponent())
		return nullptr;

	return libv::make_observer_ptr(this);
}

libv::vec3f CoreComponent::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	return padding_size3();
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
