// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/scroll_pane.hpp>
// std
#include <optional>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/property_access_context.hpp>

#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreScrollArea : public CoreComponent {
	friend class ScrollArea;
	[[nodiscard]] inline auto handler() { return ScrollArea{this}; }

private:
	struct Properties {
		PropertyL<ScrollAreaMode> mode;
//		PropertyL<libv::vec2f> area_position;
//		PropertyL<libv::vec2f> area_size;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

private:
//	Component client;
	std::optional<Component> client;
	libv::vec2f area_position;
	libv::vec2f area_size;

public:
	using CoreComponent::CoreComponent;

	inline void content(Component&& value) noexcept {
		client = std::move(value);
		flagForce(Flag::pendingAttachChild);
	}

	inline void content(const Component& value) noexcept {
		client = value;
		flagForce(Flag::pendingAttachChild);
	}

private:
	virtual void doAttach() override;
	virtual void doDetach() override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doRender(Renderer& r) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreScrollArea::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.mode; },
			ScrollAreaMode::vertical,
			pgr::appearance, pnm::scroll_area_mode,
			"Scroll area mode"
	);
//	ctx.synthetize(
//			[](auto& c, auto v) { c.handler().mode(std::move(v)); },
//			[](const auto& c) { return c.handler().mode(); },
//			pgr::behaviour, pnm::scroll_area_mode,
//			"Scroll area mode"
//	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().area_position(std::move(v)); },
			[](const auto& c) { return c.handler().area_position(); },
			pgr::behaviour, pnm::area_position,
			"Scroll area view position"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().area_size(std::move(v)); },
			[](const auto& c) { return c.handler().area_size(); },
			pgr::behaviour, pnm::area_size,
			"Scroll area view size"
	);
}

template <typename T>
void CoreScrollArea::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CoreScrollArea::doStyle(ContextStyle& ctx) {
	// TODO P2: Having property (aka this->property) mentioned here is incorrect, but the class based access context will solve he problem
	PropertyAccessContext<CoreScrollArea> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CoreScrollArea::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

void CoreScrollArea::doAttach() {
	// <<< P6: scroll area subscribe_region
//	context().mouse.subscribe_region(this);
}

void CoreScrollArea::doDetach() {
	// <<< P6: scroll area unsubscribe_region
//	context().mouse.unsubscribe_region(this);
}

libv::vec3f CoreScrollArea::doLayout1(const ContextLayout1& environment) {
	if (!client)
		return {};

	return AccessLayout::layout1(client->core(), ContextLayout1{environment.size - padding_size3()}) + padding_size3();
}

void CoreScrollArea::doLayout2(const ContextLayout2& environment) {
	if (!client)
		return;

	// <<< P5: Use client's anchor for (initial) positioning inside the scroll area
	const auto client_position = padding_LB3() + vec3f(0, environment.size.y - padding_size3().y, 0);
	auto client_size = environment.size;

	switch (property.mode()) {
	case ScrollAreaMode::both:
		client_size.x = -1;
		client_size.y = -1; break;
	case ScrollAreaMode::horizontal:
		client_size.x = -1; break;
	case ScrollAreaMode::vertical:
		client_size.y = -1; break;
	}

	AccessLayout::layout2(client->core(), environment.enter(client_position, client_size));
}

void CoreScrollArea::doRender(Renderer& r) {
	if (!client)
		return;

	// <<< P4: scroll area clip and translate

//	switch (property.mode()) {
//	case ScrollAreaMode::both:
//		environment.clip({}, layout_size2()); break;
//	case ScrollAreaMode::horizontal:
//		environment.clip({}, libv::vec2f(-1, layout_size2().y)); break;
//	case ScrollAreaMode::vertical:
//		environment.clip({}, libv::vec2f(layout_size2().x, -1)); break;
//	}

	r.clip(layout_position2(), layout_size2());
	//log_ui.info("Clip to: {}, {}", layout_position2(), layout_size2());
//	r.clip({}, layout_size2());

	r.translate({libv::vec::round(area_position), 0});
//	const auto pos = libv::vec::round(-area_position);
//	r.translate(pos.x, pos.y, 0);

	Renderer rc = r.enter(*client);
	AccessParent::render(client->core(), rc);

	r.translate({libv::vec::round(-area_position), 0}); // <<< P7: Need proper guarding
}

void CoreScrollArea::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	if (!client) {
		assert(false && "Internal error: Requesting detach for a not set client component");
		return;
	}

	callback(*client);
}

libv::observer_ptr<CoreComponent> CoreScrollArea::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
//	const ChildID dir = context.isForward() ? +1 : -1;
//	const ChildID end = context.isForward() ? static_cast<ChildID>(children.size()) : -1;
//	ChildID begin = context.isForward() ? 0 : static_cast<ChildID>(children.size() - 1);
//
//	if (current == ChildIDNone) {
//		// unrelated component is focused, focus self or iterate every children
//		if (AccessParent::isFocusableComponent(*this))
//			return libv::make_observer(this);
//
//	} else if (current == ChildIDSelf) {
//		// this component itself is currently focused, iterate every children
//
//	} else {
//		// one of the children is currently focused, iterate remaining children
//		begin = current + dir;
//	}
//
//	if (!AccessParent::isFocusableChild(*this))
//		return nullptr;
//
//	for (ChildID i = begin; i != end; i += dir)
//		if (auto hit = AccessParent::doFocusTraverse(children[i].core(), context, ChildIDNone))
//			return hit;

	// <<< P8: focus traverse inside scroll area

	return nullptr;
}

void CoreScrollArea::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	if (!client)
		return;

	callback(*client);
}

void CoreScrollArea::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	if (!client)
		return;

	callback(*client);
}

// =================================================================================================

ScrollArea::ScrollArea(std::string name) :
	ComponentHandler<CoreScrollArea, EventHostGeneral<ScrollArea>>(std::move(name)) { }

ScrollArea::ScrollArea(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreScrollArea, EventHostGeneral<ScrollArea>>(gen, type) { }

ScrollArea::ScrollArea(core_ptr core) noexcept :
	ComponentHandler<CoreScrollArea, EventHostGeneral<ScrollArea>>(core) { }

// -------------------------------------------------------------------------------------------------

void ScrollArea::mode(ScrollAreaMode value) noexcept {
	AccessProperty::manual(self(), self().property.mode, value);
}

ScrollAreaMode ScrollArea::mode() const noexcept {
	return self().property.mode();
}

void ScrollArea::content(Component&& value) noexcept {
	self().content(std::move(value));
}

void ScrollArea::content(const Component& value) noexcept {
	self().content(value);
}

Component& ScrollArea::content() noexcept {
	assert(self().client && "No content assigned");
	return *self().client;
}

const Component& ScrollArea::content() const noexcept {
	assert(self().client && "No content assigned");
	return *self().client;
}

// -------------------------------------------------------------------------------------------------

void ScrollArea::area_position(libv::vec2f value) noexcept {
	self().area_position = value;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

[[nodiscard]] libv::vec2f ScrollArea::area_position() const noexcept {
	return self().area_position;
}

void ScrollArea::area_size(libv::vec2f value) noexcept {
	self().area_size = value;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

[[nodiscard]] libv::vec2f ScrollArea::area_size() const noexcept {
	return self().area_size;
}

// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

//class CoreScrollPane : public CoreComponent {
//	friend class ScrollPane;
//	[[nodiscard]] inline auto handler() { return ScrollPane{this}; }
//
//private:
//	struct Properties {
//		ScrollPaneMode mode;
//
//		BarPlacementHorizontal bar_placement_horizontal;
//		BarPlacementVertical bar_placement_vertical;
//
//		BarVisibility bar_visibility_horizontal;
//		BarVisibility bar_visibility_vertical;
//	} property;
//
//	struct ChildProperties {
//	};
//
//	template <typename T> static void access_properties(T& ctx);
//	template <typename T> static void access_child_properties(T& ctx);
//
////	static ComponentPropertyDescription description;
////	static ComponentPropertyDescription child_description;
//
//public:
//	using CoreComponent::CoreComponent;
//
//private:
//	virtual void onMouseButton(const EventMouseButton& event) override;
//	virtual void onMouseMovement(const EventMouseMovement& event) override;
//	virtual void onMouseScroll(const EventMouseScroll& event) override;
//
//private:
//	virtual void doAttach() override;
//	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
//	virtual void doLayout2(const ContextLayout2& environment) override;
//	virtual void doRender(Renderer& r) override;
//	virtual void doStyle(ContextStyle& context) override;
//	virtual void doStyle(ContextStyle& context, ChildID childID) override;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//void CoreScrollPane::access_properties(T& ctx) {
//	(void) ctx;
//}
//
//template <typename T>
//void CoreScrollPane::access_child_properties(T& ctx) {
//	(void) ctx;
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void CoreScrollPane::doStyle(ContextStyle& ctx) {
//	// TODO P2: Having property (aka this->property) mentioned here is incorrect, but the class based access context will solve he problem
//	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
//	access_properties(setter);
//	CoreComponent::access_properties(setter);
//}
//
//void CoreScrollPane::doStyle(ContextStyle& ctx, ChildID childID) {
//	(void) ctx;
//	(void) childID;
//}
//
//// -------------------------------------------------------------------------------------------------
//
//libv::vec3f CoreScrollPane::doLayout1(const ContextLayout1& environment) {
//	(void) environment;
//
//	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
//		if (fix == libv::Approx(0.f)) {
//			return fix;
//		} else if (percent == libv::Approx(100.f)) {
//			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
//			return fix * 2.f;
//		} else {
//			return fix / (1.f - percent * 0.01f);
//		}
//	};
//
//	auto result = libv::vec3f{};
//
//	for (auto& child : children | view_layouted()) {
//		AccessLayout::layout1(child.core(), ContextLayout1{});
//		libv::meta::for_constexpr<0, 3>([&](auto dim) {
//			result[dim] = libv::max(
//					result[dim],
//					resolvePercent(
//							child.size()[dim].pixel + (child.size()[dim].dynamic ? AccessLayout::lastDynamic(child.core())[dim] : 0.f),
//							child.size()[dim].percent, child.core())
//			);
//		});
//	}
//
//	return result;
//}
//
//void CoreScrollPane::doLayout2(const ContextLayout2& environment) {
//	for (auto& child : children | view_layouted()) {
//		AccessLayout::layout2(
//				child.core(),
//				ContextLayout2{
//					environment.position,
//					environment.size,
//					MouseOrder{libv::to_value(environment.mouseOrder) + 1}
//				}
//		);
//	}
//}
//
//// =================================================================================================
//
//ScrollPane::ScrollPane(std::string name) :
//	ComponentHandler<CoreScrollPane, EventHostGeneral<ScrollPane>>(std::move(name)) { }
//
//ScrollPane::ScrollPane(GenerateName_t gen, const std::string_view type) :
//	ComponentHandler<CoreScrollPane, EventHostGeneral<ScrollPane>>(gen, type) { }
//
//ScrollPane::ScrollPane(core_ptr core) noexcept :
//	ComponentHandler<CoreScrollPane, EventHostGeneral<ScrollPane>>(core) { }
//
//// -------------------------------------------------------------------------------------------------
//
////void ScrollPane::add(Component component) {
////	self().add(std::move(component));
////}
////
////void ScrollPane::remove(Component& component) {
////	self().remove(component);
////}
////
////void ScrollPane::clear() {
////	self().clear();
////}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
