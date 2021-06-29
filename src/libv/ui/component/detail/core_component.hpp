// Project: libv.ui, File: src/libv/ui/component/detail/core_component.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/intrusive_ptr.hpp>
#include <libv/utility/observer_ptr.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <string>
#include <string_view>
#include <typeindex>
// pro
#include <libv/ui/component/detail/flag.hpp>
#include <libv/ui/component/detail/generate_name.hpp>
#include <libv/ui/event/detail/internal_event_linkage.hpp>
#include <libv/ui/property.hpp> // TODO P1: Remove property.hpp from here (the std::variant is killing me)
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using ChildID = int32_t;
static constexpr ChildID ChildIDSelf = -2;
static constexpr ChildID ChildIDNone = -1;

class CoreComponent {
	friend struct AccessEvent;
	friend struct AccessLayout;
	friend struct AccessParent;
	friend struct AccessProperty;
	friend struct AccessRoot;

	friend class Component;

private:
	Flag_t flags = Flag::mask_init;
	ChildID childID = 0;
	uint32_t ref_count = 0;

	libv::vec3f layout_position_; /// Component position relative to parent in pixels
	libv::vec3f layout_size_;     /// Component size in pixels

private:
	Size size_;
	Anchor anchor_;
	Margin margin_; /// x: left, y: down, z: right, w: top
	Padding padding_; /// x: left, y: down, z: right, w: top

//	Margin margin_{10, 10, 10, 10}; // Theme::default_margin
//	Padding padding_{5, 5, 5, 5}; // Theme::default_padding

private:
	/// Never null, points to self if its a (temporal) root element otherwise points to parent
	libv::observer_ref<CoreComponent> parent_ = libv::make_observer_ref(this);
	/// Never null, points to the associated context
	libv::observer_ref<ContextUI> context_;
	/// Null if no style is assigned to the component
	libv::intrusive_ptr<Style> style_;

public:
	std::string name;

public:
	explicit CoreComponent(std::string name);

	CoreComponent(const CoreComponent&) = delete;
	CoreComponent(CoreComponent&&) = delete;
	CoreComponent& operator=(const CoreComponent&) = delete;
	CoreComponent& operator=(CoreComponent&&) = delete;

	virtual ~CoreComponent();

public:
	[[nodiscard]] inline libv::observer_ref<CoreComponent> parent() const noexcept {
		return parent_;
	}

	[[nodiscard]] std::string path() const;

	[[nodiscard]] inline ContextUI& context() const noexcept {
		return *context_;
	}
	[[nodiscard]] inline bool isAttached() const noexcept {
		return parent_ != this;
	}
	[[nodiscard]] inline bool isRendered() const noexcept {
		return flags.match_any(Flag::render);
	}
	[[nodiscard]] inline bool isLayouted() const noexcept {
		return flags.match_any(Flag::layout);
	}
	[[nodiscard]] inline bool isFocused() const noexcept {
		return flags.match_any(Flag::focused);
	}
	[[nodiscard]] inline bool isSignal() const noexcept {
		return flags.match_any(Flag::signal);
	}
	[[nodiscard]] inline bool isSlot() const noexcept {
		return flags.match_any(Flag::slot);
	}

	[[nodiscard]] inline libv::vec3f layout_position() const noexcept {
		return layout_position_;
	}
	[[nodiscard]] inline libv::vec2f layout_position2() const noexcept {
		return libv::vec2f{layout_position_.x, layout_position_.y};
	}
	[[nodiscard]] inline libv::vec3f layout_size() const noexcept {
		return layout_size_;
	}
	[[nodiscard]] inline libv::vec2f layout_size2() const noexcept {
		return libv::vec2f{layout_size_.x, layout_size_.y};
	}

public:
	[[nodiscard]] inline const Size& size() const noexcept {
		return size_;
	}
	void size(Size value) noexcept;

	[[nodiscard]] inline Anchor anchor() const noexcept {
		return anchor_;
	}
	inline void anchor(Anchor value) noexcept {
		anchor_ = value;
		markInvalidLayout();
	}

	inline void margin(Margin value) noexcept {
		margin_ = value;
		markInvalidLayout();
	}
	inline void margin(float left_down_right_top) noexcept {
		margin({left_down_right_top, left_down_right_top, left_down_right_top, left_down_right_top});
	}
	inline void margin(float left_right, float down_top) noexcept {
		margin({left_right, down_top, left_right, down_top});
	}
	inline void margin(float left, float down, float right, float top) noexcept {
		margin({left, down, right, top});
	}
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] inline Margin margin() const noexcept {
		return margin_;
	}
	/// x: left, y: down
	[[nodiscard]] inline libv::vec2f margin_LB() const noexcept {
		return xy(margin_);
	}
	/// x: left, y: down, z: 0
	[[nodiscard]] inline libv::vec3f margin_LB3() const noexcept {
		return {margin_LB(), 0.0f};
	}
	/// x: right, y: top
	[[nodiscard]] inline libv::vec2f margin_RT() const noexcept {
		return zw(margin_);
	}
	/// x: right, y: top, z: 0
	[[nodiscard]] inline libv::vec3f margin_RT3() const noexcept {
		return {margin_RT(), 0.0f};
	}
	/// x: left + right, y: down + top
	[[nodiscard]] inline libv::vec2f margin_size() const noexcept {
		return margin_LB() + margin_RT();
	}
	/// x: left + right, y: down + top, z: 0
	[[nodiscard]] inline libv::vec3f margin_size3() const noexcept {
		return {margin_size(), 0.0f};
	}

	inline void padding(Padding value) noexcept {
		padding_ = value;
		markInvalidLayout();
	}
	inline void padding(float left_down_right_top) noexcept {
		padding({left_down_right_top, left_down_right_top, left_down_right_top, left_down_right_top});
	}
	inline void padding(float left_right, float down_top) noexcept {
		padding({left_right, down_top, left_right, down_top});
	}
	inline void padding(float left, float down, float right, float top) noexcept {
		padding({left, down, right, top});
	}
	/// x: left, y: down, z: right, w: top
	[[nodiscard]] inline Padding padding() const noexcept {
		return padding_;
	}
	/// x: left, y: down
	[[nodiscard]] inline libv::vec2f padding_LB() const noexcept {
		return xy(padding_);
	}
	/// x: left, y: down, z: 0
	[[nodiscard]] inline libv::vec3f padding_LB3() const noexcept {
		return {padding_LB(), 0.0f};
	}
	/// x: right, y: top
	[[nodiscard]] inline libv::vec2f padding_RT() const noexcept {
		return zw(padding_);
	}
	/// x: right, y: top, z: 0
	[[nodiscard]] inline libv::vec3f padding_RT3() const noexcept {
		return {padding_RT(), 0.0f};
	}
	/// x: left + right, y: down + top
	[[nodiscard]] inline libv::vec2f padding_size() const noexcept {
		return padding_LB() + padding_RT();
	}
	/// x: left + right, y: down + top, z: 0
	[[nodiscard]] inline libv::vec3f padding_size3() const noexcept {
		return {padding_size(), 0.0f};
	}

public:
	template <typename T>
	void access_properties(T& ctx);

public:
	void flagAncestors(Flag_t flags_) noexcept;
	void flagDirect(Flag_t flags_) noexcept;
	void flagAuto(Flag_t flags_) noexcept;
	void flagForce(Flag_t flags_) noexcept;

private:
	void flagPurge(Flag_t flags_) noexcept;

protected:
	void watchChar(bool value) noexcept;
	void watchKey(bool value) noexcept;
	void watchFocus(bool value) noexcept;
	void watchMouse(bool value) noexcept;
	void floatRegion(bool value) noexcept;
	[[nodiscard]] bool isWatchChar() const noexcept;
	[[nodiscard]] bool isWatchKey() const noexcept;
	[[nodiscard]] bool isWatchFocus() const noexcept;
	[[nodiscard]] bool isWatchMouse() const noexcept;

public:
	[[nodiscard]] bool isFloatRegion() const noexcept;

public:
	void focus() noexcept;
	void markRemove() noexcept;
	void markInvalidLayout() noexcept;
	void style(libv::intrusive_ptr<Style> style) noexcept;

public:
	[[nodiscard]] libv::vec2f calculate_local_mouse_coord() const noexcept;

public:
	template <typename Property>
	inline void set(Property& property, typename Property::value_type value);
	template <typename Property>
	inline void reset(Property& property);

public:
	template <typename Event>
	inline void fire(const Event& event);

private:
	ContextStyle makeStyleContext() noexcept;

private:
	bool isFocusableComponent() const noexcept;

private:
	static void eventChar(CoreComponent& component, const EventChar& event);
	static void eventKey(CoreComponent& component, const EventKey& event);
	static void focusGain(CoreComponent& component);
	static void focusLoss(CoreComponent& component);

private:
	virtual void onChar(const EventChar& event);
	virtual void onKey(const EventKey& event);
	virtual void onFocus(const EventFocus& event);
	virtual void onMouseButton(const EventMouseButton& event);
	virtual void onMouseMovement(const EventMouseMovement& event);
	virtual void onMouseScroll(const EventMouseScroll& event);

private:
	void attach(CoreComponent& parent);
	void detach();
	void detachScan();
	void update();
	void style();
	void styleScan();
	libv::observer_ptr<CoreComponent> focusTraverse(const ContextFocusTraverse& context);
	libv::vec3f layout1(const ContextLayout1& environment);
	void layout2(const ContextLayout2& environment);
	void render(Renderer& r);
	void renderDestroy(Renderer& r);

protected:
	virtual void doAttach();
	virtual void doDetach();
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback);
	virtual void doUpdate();
	virtual void doStyle(ContextStyle& context);
	virtual void doStyle(ContextStyle& context, ChildID childID);
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current);
	virtual libv::vec3f doLayout1(const ContextLayout1& environment);
	virtual void doLayout2(const ContextLayout2& environment);
	virtual void doCreate(Renderer& r);
	virtual void doRender(Renderer& r);
	virtual void doDestroy(Renderer& r);
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback);
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback);
};

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.ui: Remove set/reset from here (?)
template <typename Property>
inline void CoreComponent::set(Property& property, typename Property::value_type value) {
	AccessProperty::driver(property, PropertyDriver::manual);
	if (value != property())
		AccessProperty::value(*this, property, std::move(value));
}

template <typename Property>
inline void CoreComponent::reset(Property& property) {
	AccessProperty::driver(property, PropertyDriver::style);
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

template <typename Event>
inline void CoreComponent::fire(const Event& event) {
	detail::internal_fire(this, std::type_index(typeid(Event)), &event);
}

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreComponent::access_properties(T& ctx) {
	ctx.synthetize(
			[](auto& c, auto v) { c.size(v); },
			[](const auto& c) { return c.size(); },
			pgr::layout, pnm::size,
			"Component size in pixel, percent, ratio and dynamic units"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.anchor(v); },
			[](const auto& c) { return c.anchor(); },
			pgr::layout, pnm::anchor,
			"Component's anchor point"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.margin(v); },
			[](const auto& c) { return c.margin(); },
			pgr::layout, pnm::margin,
			"Component's margin"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.padding(v); },
			[](const auto& c) { return c.padding(); },
			pgr::layout, pnm::padding,
			"Component's padding"
	);
}

// -------------------------------------------------------------------------------------------------

struct AccessEvent {
	static inline decltype(auto) onMouseButton(CoreComponent& component, const EventMouseButton& event) {
		return component.onMouseButton(event);
	}
	static inline decltype(auto) onMouseMovement(CoreComponent& component, const EventMouseMovement& event) {
		return component.onMouseMovement(event);
	}
	static inline decltype(auto) onMouseScroll(CoreComponent& component, const EventMouseScroll& event) {
		return component.onMouseScroll(event);
	}
};

struct AccessParent {
	[[nodiscard]] static inline auto& childID(CoreComponent& component) noexcept {
		return component.childID;
	}
	[[nodiscard]] static inline const auto& childID(const CoreComponent& component) noexcept {
		return component.childID;
	}
	static inline decltype(auto) isFocusableComponent(const CoreComponent& component) {
		return component.isFocusableComponent();
	}
	static inline decltype(auto) isFocusableChild(const CoreComponent& component) {
		return component.flags.match_any(Flag::focusableChild);
	}
	static inline decltype(auto) doFocusTraverse(CoreComponent& component, const ContextFocusTraverse& context, ChildID current) {
		return component.doFocusTraverse(context, current);
	}
	static inline decltype(auto) update(CoreComponent& component) {
		return component.update();
	}
	static inline decltype(auto) render(CoreComponent& component, Renderer& r) {
		return component.render(r);
	}
};

struct AccessLayout {
	static inline decltype(auto) layout1(CoreComponent& component, const ContextLayout1& environment) {
		return component.layout1(environment);
	}
	static inline decltype(auto) layout2(CoreComponent& component, const ContextLayout2& environment) {
		return component.layout2(environment);
	}
};

struct AccessRoot : AccessEvent, AccessLayout, AccessParent {
	[[nodiscard]] static inline auto& layout_position(CoreComponent& component) noexcept {
		return component.layout_position_;
	}
	[[nodiscard]] static inline const auto& layout_position(const CoreComponent& component) noexcept {
		return component.layout_position_;
	}
	[[nodiscard]] static inline auto& layout_size(CoreComponent& component) noexcept {
		return component.layout_size_;
	}
	[[nodiscard]] static inline const auto& layout_size(const CoreComponent& component) noexcept {
		return component.layout_size_;
	}

	static inline decltype(auto) flagAuto(CoreComponent& component, Flag_t flags_) noexcept {
		return component.flagAuto(flags_);
	}

	static inline decltype(auto) eventChar(CoreComponent& component, const EventChar& event) {
		return CoreComponent::eventChar(component, event);
	}
	static inline decltype(auto) eventKey(CoreComponent& component, const EventKey& event) {
		return CoreComponent::eventKey(component, event);
	}

	static inline decltype(auto) attach(CoreComponent& component, CoreComponent& parent) {
		return component.attach(parent);
	}
	static inline decltype(auto) detachScan(CoreComponent& component) {
		return component.detachScan();
	}
	static inline decltype(auto) style(CoreComponent& component) {
		return component.style();
	}
	static inline decltype(auto) styleScan(CoreComponent& component) {
		return component.styleScan();
	}
	static inline decltype(auto) focusGain(CoreComponent& component) {
		return CoreComponent::focusGain(component);
	}
	static inline decltype(auto) focusLoss(CoreComponent& component) {
		return CoreComponent::focusLoss(component);
	}
	static inline decltype(auto) focusTraverse(CoreComponent& component, const ContextFocusTraverse& context) {
		return component.focusTraverse(context);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
