// Project: libv.ui, File: src/libv/ui/component/scroll_area.cpp

// hpp
#include <libv/ui/component/scroll_area.hpp>
#include <libv/meta/for_constexpr.hpp>
// std
#include <optional>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_utility.hpp>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreScrollArea : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	PropertyR<Background> background;

	PropertyL1L2LP<ScrollMode> mode;

	// struct ChildProperties {
	// };

	template <typename Access> static void access_properties(Access& access);
	template <typename Access> static void access_child_properties(Access& access);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
//	Component client;
	std::optional<Component> client;
	libv::vec2f area_position;
	libv::vec2f area_size;

public:
	inline void content(Component&& value) noexcept {
		client = std::move(value);
		flagForce(Flag::pendingAttachChild);
	}

	inline void content(const Component& value) noexcept {
		client = value;
		flagForce(Flag::pendingAttachChild);
	}

public:
	virtual void onMouseMovement(const EventMouseMovement& event) override;
//	virtual void onMouseScroll(const EventMouseScroll& event) override;

public:
	virtual void doAttach() override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doStyle(StyleAccess& access) override;
	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doRender(Renderer& r) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename Access>
void CoreScrollArea::access_properties(Access& access) {
	access.property(
			[](auto& c) -> auto& { return c.background; },
			Background::none(),
			pgr::appearance, pnm::background, "Background"
	);
	access.property(
			[](auto& c) -> auto& { return c.mode; },
			ScrollMode::vertical,
			pgr::appearance, pnm::scroll_mode,
			"Scroll area mode"
	);
//	access.synthesize(
//			[](auto& c, auto v) { c.handler().mode(std::move(v)); },
//			[](const auto& c) { return c.handler().mode(); },
//			pgr::behaviour, pnm::scroll_mode,
//			"Scroll area mode"
//	);
	access.synthesize(
			[](auto& c, auto v) { c.handler().area_position(std::move(v)); },
			[](const auto& c) { return c.handler().area_position(); },
			pgr::behaviour, pnm::area_position,
			"Scroll area view position"
	);
	access.synthesize(
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

void CoreScrollArea::doStyle(StyleAccess& access) {
	access.self(*this);
	if (client)
		access.child(*client);
}

void CoreScrollArea::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

void CoreScrollArea::onMouseMovement(const EventMouseMovement& event) {
	event.pass_through();
}

//void CoreScrollArea::onMouseScroll(const EventMouseScroll& event) {
//
//}

// -------------------------------------------------------------------------------------------------

void CoreScrollArea::doAttach() {
	floatRegion(true);
}

libv::vec3f CoreScrollArea::doLayout1(const ContextLayout1& layout_env) {
	if (!client || !client->core().isLayouted())
		return padding_size3();

	const auto is_vertical = mode() != ScrollMode::horizontal;
	const auto is_horizontal = mode() != ScrollMode::vertical;

	const auto env_size = layout_env.size - padding_size3();
	const auto client_env_size = libv::vec3f{
			is_horizontal ? -1.0f : env_size.x,
			is_vertical ? -1.0f : env_size.y,
			env_size.z};

	auto result = libv::vec3f{};

	const auto client_dynamic = client->size().has_dynamic() ?
			AccessLayout::layout1(client->core(), ContextLayout1{client_env_size}) :
			libv::vec3f{};

	libv::meta::for_constexpr<0, 3>([&](auto i) {
		result[i] = std::max(
				result[i],
				resolvePercent(
						client->size()[i].pixel + (client->size()[i].dynamic ? client_dynamic[i] : 0.f),
						client->size()[i].percent, client->core())
		);
	});

	return result + padding_size3();
}

void CoreScrollArea::doLayout2(const ContextLayout2& layout_env) {
	if (!client || !client->core().isLayouted())
		return;

	const auto is_vertical = mode() != ScrollMode::horizontal;
	const auto is_horizontal = mode() != ScrollMode::vertical;

	const auto env_size = layout_env.size - padding_size3();
	const auto client_env_size = libv::vec3f{
			is_horizontal ? -1.0f : env_size.x,
			is_vertical ? -1.0f : env_size.y,
			env_size.z};

	// Size ---

	auto size = libv::vec3f{};

	const auto client_dynamic = client->size().has_dynamic() ?
			AccessLayout::layout1(client->core(), ContextLayout1{client_env_size}) :
			libv::vec3f{};

	const auto client_area_size = libv::vec3f{
			is_horizontal ? client_dynamic.x : env_size.x,
			is_vertical ? client_dynamic.y : env_size.y,
			env_size.z};

	if (xy(client_area_size) != area_size) { // Update area size
		const auto old_area_size = area_size;
		area_size = xy(client_area_size);
		fire(EventScrollArea{area_position, old_area_size});
	}

	libv::meta::for_constexpr<0, 3>([&](auto i) {
		const auto has_ratio = client->size()[i].ratio != 0.f;

		if (has_ratio)
			size[i] = client_area_size[i]; // NOTE: Ratio uses the client area size
		else
			size[i] =
					client->size()[i].pixel +
					client->size()[i].percent * 0.01f * env_size[i] + // NOTE: Percent uses the scroll-area size
					(client->size()[i].dynamic ? client_dynamic[i] : 0.f);
	});

	// Position ---

	const auto client_area_anchor = info(client->anchor()).rate();
	const auto client_anchor = info(client->anchor()).rate();
	// NOTE: For now client_area_anchor is the same as client_anchor, if needed this could be a property of the scroll area

	const auto position =
			+ padding_LB3()
			+ client_area_anchor * layout_env.size
			- client_area_anchor * client_area_size
			+ client_anchor * client_area_size
			- client_anchor * size; // NOTE: Anchor uses layout env size and client size

	const auto roundedPosition = libv::vec::round(position);
	const auto roundedSize = libv::vec::round(position + size) - roundedPosition;

	AccessLayout::layout2(client->core(), layout_env.enter(roundedPosition, roundedSize));
}

void CoreScrollArea::doRender(Renderer& r) {
	background().render(r, {0, 0}, layout_size2(), *this);

	if (!client)
		return;

	r.clip(padding_extent().left_bottom(), layout_size2() - padding_extent().size());

	const auto rounded_area_position = libv::vec3f(libv::vec::round(area_position - padding_extent().left_bottom()), 0.f);
	r.translate(rounded_area_position);

	Renderer rc = r.enter(*client);
	AccessParent::render(client->core(), rc);

	// TODO P4: Renderer proper translate and clip guarding (same in component_core.render)
	r.translate(-rounded_area_position);
}

void CoreScrollArea::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	if (!client) {
		assert(false && "Internal error: Requesting detach for a not set client component");
		return;
	}

	callback(*client);
}

libv::observer_ptr<CoreComponent> CoreScrollArea::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
	if (current == ChildIDNone) {
		// unrelated component is focused, focus self or iterate every children
		if (AccessParent::isFocusableComponent(*this))
			return libv::make_observer_ptr(this);

	} else if (current == ChildIDSelf) {
		// this component itself is currently focused, iterate every children

	} else {
		// one of the children is currently focused, iterate remaining children
		if (!context.isForward())
			if (AccessParent::isFocusableComponent(*this))
				return libv::make_observer_ptr(this);

		return nullptr;
	}

	if (!client || !AccessParent::isFocusableChild(*this))
		return nullptr;

	if (auto hit = AccessParent::doFocusTraverse(client->core(), context, ChildIDNone))
		return hit;

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

core_ptr ScrollArea::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool ScrollArea::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void ScrollArea::background(Background value) {
	AccessProperty::manual(self(), self().background, std::move(value));
}

[[nodiscard]] const Background& ScrollArea::background() const noexcept {
	return self().background();
}

// -------------------------------------------------------------------------------------------------

void ScrollArea::mode(ScrollMode value) noexcept {
	AccessProperty::manual(self(), self().mode, value);
}

ScrollMode ScrollArea::mode() const noexcept {
	return self().mode();
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
	self().context().mouse.update_region(self(), libv::vec::round(value));
}

[[nodiscard]] libv::vec2f ScrollArea::area_position() const noexcept {
	return self().area_position;
}

//void ScrollArea::area_size(libv::vec2f value) noexcept {
//	self().area_size = value;
//	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
//}

[[nodiscard]] libv::vec2f ScrollArea::area_size() const noexcept {
	return self().area_size;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
