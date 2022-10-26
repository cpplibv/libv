// Project: libv.ui, File: src/libv/ui/component/scroll_pane.cpp

// hpp
#include <libv/ui/component/scroll_pane.hpp>
// std
#include <optional>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_slc.hpp>
#include <libv/ui/component/layout/layout_utility.hpp>
#include <libv/ui/component/scroll_area.hpp>
#include <libv/ui/component/slider.hpp>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreScrollPane : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	PropertyR<Background> background;

	PropertyL1L2LP<Spacing2> spacing2;
	PropertyL1L2LP<ScrollMode> mode;
//	BarPlacementHorizontal bar_placement_horizontal;
//	BarPlacementVertical bar_placement_vertical;
//	BarVisibility bar_visibility_horizontal;
//	BarVisibility bar_visibility_vertical;
//	PropertyL<libv::vec2f> area_position;
//	PropertyL<libv::vec2f> area_size;

	template <typename Access> static void access_properties(Access& access);
	template <typename Access> void access_children(Access&& access);
	template <typename Access> void access_layout(Access&& access);

public:
	ScrollArea area;
	Slider vbar;
	Slider hbar;

public:
//	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

public:
	virtual void doAttach() override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doStyle(StyleAccess& access) override;
//	virtual void doStyle(StyleAccess& access, ChildID childID) override;
//	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doRender(Renderer& r) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename Access>
void CoreScrollPane::access_properties(Access& access) {
	access.property(
			[](auto& c) -> auto& { return c.background; },
			// Access::core_property(&CoreScrollPane::background),
			Background::none(),
			pgr::appearance, pnm::background, "Background"
	);
	access.property(
			[](auto& c) -> auto& { return c.spacing2; },
			// Access::core_property(&CoreScrollPane::spacing2),
			Spacing2{0, 0},
			pgr::layout, pnm::spacing2, "Spacing between the component columns (X) and rows (Y)"
	);
	access.property(
			[](auto& c) -> auto& { return c.mode; },
			// Access::core_property(&CoreScrollPane::mode),
			ScrollMode::both,
			pgr::layout, pnm::scroll_mode, "Scroll area mode"
	);
////	access.synthesize(
////			[](auto& c, auto v) { c.handler().mode(std::move(v)); },
////			[](const auto& c) { return c.handler().mode(); },
////			pgr::behaviour, pnm::scroll_mode, "Scroll area mode"
////	);
//	access.synthesize(
//			[](auto& c, auto v) { c.handler().area_position(std::move(v)); },
//			[](const auto& c) { return c.handler().area_position(); },
//			pgr::behaviour, pnm::area_position, "Scroll area view position"
//	);
//	access.synthesize(
//			[](auto& c, auto v) { c.handler().area_size(std::move(v)); },
//			[](const auto& c) { return c.handler().area_size(); },
//			pgr::behaviour, pnm::area_size, "Scroll area view size"
//	);
}

template <typename Access>
void CoreScrollPane::access_children(Access&& access) {
	access(area);
	access(vbar);
	access(hbar);
}

template <typename Access>
void CoreScrollPane::access_layout(Access&& access) {
	//	static_layout_padding(padding) (
	//		static_layout_line(libv::ui::Orientation::right) (
	//			static_layout_line(libv::ui::Orientation::down) (
	//				content,
	//				vbar
	//			),
	//			hbar
	//		)
	//	)

	const auto is_vertical = info(mode()).vertical();
	const auto is_horizontal = info(mode()).horizontal();

	if (is_vertical && is_horizontal)
		access(
				SLC_Padding(this->padding_extent(),
					SLC_LineStatic(spacing2().x, Orientation::right,
						SLC_LineStatic(spacing2().y, Orientation::down,
							SLC_Component(area),
							SLC_Component(hbar)
						),
						SLC_Component(vbar)
					)
				)
		);

	else if (is_vertical)
		access(
				SLC_Padding(this->padding_extent(),
					SLC_LineStatic(spacing2().x, Orientation::right,
						SLC_Component(area),
						SLC_Component(vbar)
					)
				)
		);

	else if (is_horizontal)
		access(
				SLC_Padding(this->padding_extent(),
					SLC_LineStatic(spacing2().y, Orientation::down,
						SLC_Component(area),
						SLC_Component(hbar)
					)
				)
		);

	else
		access(
				SLC_Padding(this->padding_extent(),
					SLC_Component(area)
				)
		);
}

// -------------------------------------------------------------------------------------------------

void CoreScrollPane::doStyle(StyleAccess& access) {
	access.self(*this);
	access.nested(area, "area");
	access.nested(vbar, "vbar");
	access.nested(hbar, "hbar");
}

// -------------------------------------------------------------------------------------------------

//void CoreScrollPane::onMouseMovement(const EventMouseMovement& event) {
//	event.pass_through();
//}

void CoreScrollPane::onMouseScroll(const EventMouseScroll& event) {
	// <<< Event into h/v bar
}

// -------------------------------------------------------------------------------------------------

void CoreScrollPane::doAttach() {
	flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
	vbar.orientation(Orientation::down);
	hbar.orientation(Orientation::right);

	// <<< value_range s
	vbar.value_range(1);
	hbar.value_range(1);
	vbar.value(0);
	hbar.value(0);

	// <<< Event reentry loops
	auto handler = ScrollPane{this};
	vbar.event().change.connect_system(handler, [this](const EventScrollChange& event) {
		area.area_position({area.area_position().x, static_cast<float>(event.request)});
	});
	hbar.event().change.connect_system(handler, [this](const EventScrollChange& event) {
//		area.area_position({-static_cast<float>(event.request), area.area_position().y});
		area.area_position({static_cast<float>(event.request), area.area_position().y});
	});
}

void CoreScrollPane::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	access_children(callback);
}

libv::vec3f CoreScrollPane::doLayout1(const ContextLayout1& layout_env) {
	// +------------+---+
	// |            | v |
	// |    area    | . |
	// |            | b |
	// +------------+ a |
	// |    hbar    | r |
	// +------------+---+

	libv::vec2f result;
	access_layout([&](auto&& plan) {
		result = libv::ui::layoutSLCPass1(layout_env.size, plan);
	});
	return {result, 0};
}

void CoreScrollPane::doLayout2(const ContextLayout2& layout_env) {
	hbar.value_min(0);
	hbar.value_max(layout_env.size.x);
	vbar.value_min(0);
	vbar.value_max(layout_env.size.y);
//	vbar.value_step(10 * 2);
//	hbar.value_step(10 * 2);

	access_layout([&](auto&& plan) {
		libv::ui::layoutSLCPass2(layout_env.size, layout_env, plan);
	});
}

void CoreScrollPane::doRender(Renderer& r) {
	const auto is_vertical = info(mode()).vertical();
	const auto is_horizontal = info(mode()).horizontal();

	background().render(r, {0, 0}, layout_size2(), *this);

	AccessParent::render(ref_core(area), r.enter(area));

	if (is_horizontal)
		AccessParent::render(ref_core(hbar), r.enter(hbar));

	if (is_vertical)
		AccessParent::render(ref_core(vbar), r.enter(vbar));
}

//libv::observer_ptr<CoreComponent> CoreScrollPane::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
//	if (current == ChildIDNone) {
//		// unrelated component is focused, focus self or iterate every children
//		if (AccessParent::isFocusableComponent(*this))
//			return libv::make_observer_ptr(this);
//
//	} else if (current == ChildIDSelf) {
//		// this component itself is currently focused, iterate every children
//
//	} else {
//		// one of the children is currently focused, iterate remaining children
//		if (!context.isForward())
//			if (AccessParent::isFocusableComponent(*this))
//				return libv::make_observer_ptr(this);
//
//		return nullptr;
//	}
//
//	if (!client || !AccessParent::isFocusableChild(*this))
//		return nullptr;
//
//	if (auto hit = AccessParent::doFocusTraverse(client->core(), context, ChildIDNone))
//		return hit;
//
//	return nullptr;
//}

void CoreScrollPane::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	// Ignoring short-circuit logic
	access_children(callback);
}

void CoreScrollPane::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	access_children(callback);
}

// =================================================================================================

core_ptr ScrollPane::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool ScrollPane::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void ScrollPane::background(Background value) {
	AccessProperty::manual(self(), self().background, std::move(value));
}

[[nodiscard]] const Background& ScrollPane::background() const noexcept {
	return self().background();
}

// -------------------------------------------------------------------------------------------------

void ScrollPane::mode(ScrollMode value) noexcept {
	AccessProperty::manual(self(), self().mode, value);
}

ScrollMode ScrollPane::mode() const noexcept {
	return self().mode();
}

void ScrollPane::content(Component&& value) noexcept {
	self().area.content(std::move(value));
}

void ScrollPane::content(const Component& value) noexcept {
	self().area.content(value);
}

Component& ScrollPane::content() noexcept {
	return self().area.content();
}

const Component& ScrollPane::content() const noexcept {
	return self().area.content();
}

// -------------------------------------------------------------------------------------------------

void ScrollPane::area_position(libv::vec2f value) noexcept {
	self().area.area_position(value);
}

libv::vec2f ScrollPane::area_position() const noexcept {
	return self().area.area_position();
}

//void ScrollPane::area_size(libv::vec2f value) noexcept {
//	self().area_size = value;
//	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
//}

libv::vec2f ScrollPane::area_size() const noexcept {
	return self().area.area_size();
}

void ScrollPane::spacing2(Spacing2 value) {
	AccessProperty::manual(self(), self().spacing2, value);
}

Spacing2 ScrollPane::spacing2() const noexcept {
	return self().spacing2();
}

Slider& ScrollPane::bar_vertical() noexcept {
	return self().vbar;
}

const Slider& ScrollPane::bar_vertical() const noexcept {
	return self().vbar;
}

Slider& ScrollPane::bar_horizontal() noexcept {
	return self().hbar;
}

const Slider& ScrollPane::bar_horizontal() const noexcept {
	return self().hbar;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
