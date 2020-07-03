// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

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
#include <libv/ui/flag.hpp>
#include <libv/ui/generate_name.hpp>
#include <libv/ui/property.hpp> // TODO P1: Remove property.hpp from here (the variant is killing me)
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextFocusTraverse;
class ContextLayout1;
class ContextLayout2;
class ContextRender;
class ContextStyle;
class ContextUI;
class EventChar;
class EventFocus;
class EventKey;
class EventMouseButton;
class EventMouseMovement;
class EventMouseScroll;

// -------------------------------------------------------------------------------------------------

using ChildID = int32_t;
static constexpr ChildID ChildIDSelf = -2;
static constexpr ChildID ChildIDNone = -1;

struct BaseComponent {
	friend class AccessConnect;
	friend class AccessEvent;
	friend class AccessLayout;
	friend class AccessParent;
	friend class AccessProperty;
	friend class AccessRoot;

	friend class Component;

private:
	Flag_t flags = Flag::mask_init;
	ChildID childID = 0;
	uint32_t ref_count = 0;

	libv::vec3f layout_position_; /// Component position relative to parent in pixels
	libv::vec3f layout_size_; /// Component size in pixels
	// TODO P2: Measure the impact of removing lastDynamic field and remove it if acceptable
	libv::vec3f lastDynamic; /// Result of last layout pass1

private:
	Size size_;
	Anchor anchor_;

private:
	/// Never null, points to self if its a (temporal) root element otherwise points to parent
	libv::observer_ref<BaseComponent> parent = libv::make_observer_ref(this);
	/// Never null, points to the associated context
	libv::observer_ref<ContextUI> context_;
	/// Null if no style is assigned to the component
	libv::intrusive_ptr<Style> style_;

public:
	std::string name;

public:
	explicit BaseComponent(std::string name);
	explicit BaseComponent(GenerateName_t, const std::string_view type, size_t index);

	BaseComponent(const BaseComponent&) = delete;
	BaseComponent(BaseComponent&&) = delete;
	BaseComponent& operator=(const BaseComponent&) = delete;
	BaseComponent& operator=(BaseComponent&&) = delete;

	virtual ~BaseComponent();

public:
	[[nodiscard]] std::string path() const;

	[[nodiscard]] inline ContextUI& context() const noexcept {
		return *context_;
	}
	[[nodiscard]] inline bool isAttached() const noexcept {
		return parent != this;
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

	[[nodiscard]] inline libv::vec3f layout_position() const noexcept {
		return layout_position_;
	}
	[[nodiscard]] inline libv::vec2f layout_position2() const noexcept {
		return libv::vec::xy(layout_position_);
	}
	[[nodiscard]] inline libv::vec3f layout_size() const noexcept {
		return layout_size_;
	}
	[[nodiscard]] inline libv::vec2f layout_size2() const noexcept {
		return libv::vec::xy(layout_size_);
	}

public:
	[[nodiscard]] inline const Size& size() const noexcept {
		return size_;
	}
	inline void size(Size value) noexcept {
		size_ = value;
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
	}
	[[nodiscard]] inline Anchor anchor() const noexcept {
		return anchor_;
	}
	inline void anchor(Anchor value) noexcept {
		anchor_ = value;
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
	}

public:
	template <typename T>
	void access_properties(T& ctx);

protected:
	void flagDirect(Flag_t flags_) noexcept;
	void flagAncestors(Flag_t flags_) noexcept;

	void flagAuto(Flag_t flags_) noexcept;
	void flagForce(Flag_t flags_) noexcept;

private:
	void flagPurge(Flag_t flags_) noexcept;

protected:
	void watchChar(bool value) noexcept;
	void watchKey(bool value) noexcept;
	void watchFocus(bool value) noexcept;
	void watchMouse(Flag_t interest) noexcept;
	[[nodiscard]] bool isWatchChar() const noexcept;
	[[nodiscard]] bool isWatchKey() const noexcept;
	[[nodiscard]] bool isWatchFocus() const noexcept;
	[[nodiscard]] Flag_t isWatchMouse() const noexcept;

public:
	void focus() noexcept;
	void markRemove() noexcept;
	void style(libv::intrusive_ptr<Style> style) noexcept;

public:
	template <typename Property>
	inline void set(Property& property, typename Property::value_type value);
	template <typename Property>
	inline void reset(Property& property);

private:
	void _fire(std::type_index type, const void* event_ptr);

protected:
	template <typename Event>
	inline void fire(const Event& event);

private:
	static void connect(BaseComponent& signal, BaseComponent& slot, std::type_index type, std::function<void(void*, const void*)>&& callback);

private:
	ContextStyle makeStyleContext() noexcept;

private:
	bool isFocusableComponent() const noexcept;

private:
	static void eventChar(BaseComponent& component, const EventChar& event);
	static void eventKey(BaseComponent& component, const EventKey& event);
	static void focusGain(BaseComponent& component);
	static void focusLoss(BaseComponent& component);

private:
	virtual void onChar(const EventChar& event);
	virtual void onKey(const EventKey& event);
	virtual void onFocus(const EventFocus& event);
	virtual void onMouseButton(const EventMouseButton& event);
	virtual void onMouseMovement(const EventMouseMovement& event);
	virtual void onMouseScroll(const EventMouseScroll& event);

private:
	void attach(BaseComponent& parent);
	void detach(BaseComponent& parent);
	void style();
	void styleScan();
	libv::observer_ptr<BaseComponent> focusTraverse(const ContextFocusTraverse& context);
	void render(ContextRender& context);
	void layout1(const ContextLayout1& environment);
	void layout2(const ContextLayout2& environment);

private:
	virtual void doAttach();
	virtual void doDetach();
	virtual void doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback);
	virtual void doStyle(ContextStyle& context);
	virtual void doStyle(ContextStyle& context, ChildID childID);
	virtual libv::observer_ptr<BaseComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current);
	virtual void doCreate(ContextRender& context);
	virtual void doDestroy(ContextRender& context);
	virtual void doRender(ContextRender& context);
	virtual void doLayout1(const ContextLayout1& environment);
	virtual void doLayout2(const ContextLayout2& environment);
	virtual void doForeachChildren(libv::function_ref<bool(BaseComponent&)> callback);
	virtual void doForeachChildren(libv::function_ref<void(BaseComponent&)> callback);

	// TWO PASS layout: (Planning on changing to on demand pass 1 as of 2020 06 30)
	// - Pass 1: calculate everything as content bottom-top and store the result
	// - Pass 2: calculate everything top-down
};

// -------------------------------------------------------------------------------------------------

template <typename Property>
inline void BaseComponent::set(Property& property, typename Property::value_type value) {
	AccessProperty::driver(property, PropertyDriver::manual);
	if (value != property())
		AccessProperty::value(*this, property, std::move(value));
}

template <typename Property>
inline void BaseComponent::reset(Property& property) {
	AccessProperty::driver(property, PropertyDriver::style);
	flagAuto(Flag::pendingStyle);
}

// -------------------------------------------------------------------------------------------------

template <typename EventT>
inline void BaseComponent::fire(const EventT& event) {
	_fire(std::type_index(typeid(EventT)), &event);
}

// -------------------------------------------------------------------------------------------------

template <typename T>
void BaseComponent::access_properties(T& ctx) {
	ctx.synthetize(
			[](auto& c, auto v) { c.anchor(v); },
			[](const auto& c) { return c.anchor(); },
			pgr::layout, pnm::anchor,
			"Component's anchor point"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.size(v); },
			[](const auto& c) { return c.size(); },
			pgr::layout, pnm::size,
			"Component size in pixel, percent, ratio and dynamic units"
	);
}

// -------------------------------------------------------------------------------------------------

struct AccessEvent {
	static inline decltype(auto) onMouseButton(BaseComponent& component, const EventMouseButton& event) {
		return component.onMouseButton(event);
	}
	static inline decltype(auto) onMouseMovement(BaseComponent& component, const EventMouseMovement& event) {
		return component.onMouseMovement(event);
	}
	static inline decltype(auto) onMouseScroll(BaseComponent& component, const EventMouseScroll& event) {
		return component.onMouseScroll(event);
	}
};

struct AccessParent {
	[[nodiscard]] static inline auto& childID(BaseComponent& component) noexcept {
		return component.childID;
	}
	[[nodiscard]] static inline const auto& childID(const BaseComponent& component) noexcept {
		return component.childID;
	}
	static inline decltype(auto) isFocusableComponent(const BaseComponent& component) {
		return component.isFocusableComponent();
	}
	static inline decltype(auto) isFocusableChild(const BaseComponent& component) {
		return component.flags.match_any(Flag::focusableChild);
	}
	static inline decltype(auto) doFocusTraverse(BaseComponent& component, const ContextFocusTraverse& context, ChildID current) {
		return component.doFocusTraverse(context, current);
	}
};

struct AccessConnect {
	static inline void connect(BaseComponent& signal, BaseComponent& slot, std::type_index type, std::function<void(void*, const void*)>&& callback) {
		BaseComponent::connect(signal, slot, type, std::move(callback));
	}
};

struct AccessLayout {
	static inline decltype(auto) layout1(BaseComponent& component, const ContextLayout1& environment) {
		return component.layout1(environment);
	}
	static inline decltype(auto) layout2(BaseComponent& component, const ContextLayout2& environment) {
		return component.layout2(environment);
	}
	[[nodiscard]] static inline auto& lastDynamic(BaseComponent& component) {
		return component.lastDynamic;
	}
	[[nodiscard]] static inline const auto& lastDynamic(const BaseComponent& component) {
		return component.lastDynamic;
	}
};

struct AccessRoot : AccessEvent, AccessLayout, AccessParent {
	[[nodiscard]] static inline auto& layout_position(BaseComponent& component) noexcept {
		return component.layout_position_;
	}
	[[nodiscard]] static inline const auto& layout_position(const BaseComponent& component) noexcept {
		return component.layout_position_;
	}
	[[nodiscard]] static inline auto& layout_size(BaseComponent& component) noexcept {
		return component.layout_size_;
	}
	[[nodiscard]] static inline const auto& layout_size(const BaseComponent& component) noexcept {
		return component.layout_size_;
	}

	static inline decltype(auto) flagAuto(BaseComponent& component, Flag_t flags_) noexcept {
		return component.flagAuto(flags_);
	}

	static inline decltype(auto) eventChar(BaseComponent& component, const EventChar& event) {
		return BaseComponent::eventChar(component, event);
	}
	static inline decltype(auto) eventKey(BaseComponent& component, const EventKey& event) {
		return BaseComponent::eventKey(component, event);
	}

	static inline decltype(auto) attach(BaseComponent& component, BaseComponent& parent) {
		return component.attach(parent);
	}
	static inline decltype(auto) detach(BaseComponent& component, BaseComponent& parent) {
		return component.detach(parent);
	}
	static inline decltype(auto) style(BaseComponent& component) {
		return component.style();
	}
	static inline decltype(auto) styleScan(BaseComponent& component) {
		return component.styleScan();
	}
	static inline decltype(auto) focusGain(BaseComponent& component) {
		return BaseComponent::focusGain(component);
	}
	static inline decltype(auto) focusLoss(BaseComponent& component) {
		return BaseComponent::focusLoss(component);
	}
	static inline decltype(auto) focusTraverse(BaseComponent& component, const ContextFocusTraverse& context) {
		return component.focusTraverse(context);
	}
	static inline decltype(auto) render(BaseComponent& component, ContextRender& context) {
		return component.render(context);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
