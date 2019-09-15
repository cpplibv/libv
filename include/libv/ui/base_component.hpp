// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/input/event_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/reflection.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/intrusive_ptr.hpp>
#include <libv/utility/observer_ptr.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <string>
#include <string_view>
#include <type_traits>
// pro
#include <libv/ui/flag.hpp>
#include <libv/ui/property_set.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextFocusTravers;
class ContextLayout1;
class ContextLayout2;
class ContextRender;
class ContextUI;
class EventFocus;
class EventMouse;

// -------------------------------------------------------------------------------------------------

struct UnnamedTag_t {};
static constexpr UnnamedTag_t UnnamedTag;

using ChildID = int32_t;
static constexpr ChildID ChildIDSelf = -2;
static constexpr ChildID ChildIDNone = -1;

struct BaseComponent {
	friend class AccessEvent;
	friend class AccessLayout;
	friend class AccessParent;
	friend class AccessRoot;

private:
	Flag_t flags = Flag::mask_init;
	ChildID childID = 0;

	libv::vec3f position_; /// Component position relative to parent in pixels
	libv::vec3f size_; /// Component size in pixels
	// TODO P2: Measure the impact of removing lastDynamic field and remove if acceptable
	libv::vec3f lastDynamic; /// Result of last layout pass1

private:
	/// Never null, points to self if its a (temporal) root element otherwise points to direct parent
	libv::observer_ref<BaseComponent> parent = libv::make_observer_ref(this);
	/// Null before attach and after detach, never null inbetween
	libv::observer_ptr<ContextUI> context_ = nullptr;
	/// Null if no style is assigned to the component
	libv::intrusive_ptr<Style> style_;

private:
	static inline size_t nextID = 0;
public:
	std::string name;

public:
	BaseComponent(std::string name);
	BaseComponent(UnnamedTag_t, const std::string_view type);
	virtual ~BaseComponent();

public:
	[[nodiscard]] std::string path() const;
	[[nodiscard]] ContextUI& context() const noexcept;

	[[nodiscard]] inline bool isAttached() noexcept {
		return context_ != nullptr;
	}
	[[nodiscard]] inline bool isRendered() noexcept {
		return flags.match_any(Flag::render);
	}
	[[nodiscard]] inline bool isLayouted() noexcept {
		return flags.match_any(Flag::layout);
	}

	[[nodiscard]] inline const libv::vec3f& position() const noexcept {
		return position_;
	}
	[[nodiscard]] inline const libv::vec3f& size() const noexcept {
		return size_;
	}

protected:
	void flagSelf(Flag_t flags_) noexcept;
	void flagParents(Flag_t flags_) noexcept;

	void flagAuto(Flag_t flags_) noexcept;
	void flagForce(Flag_t flags_) noexcept;

private:
	Flag_t flagForParent() noexcept;

public:
	void focus() noexcept;
	void markRemove() noexcept;
	void style(libv::intrusive_ptr<Style> style) noexcept;

public:
	template <typename Property>
	inline void set(Property& property, typename Property::value_type value);
	template <typename PS>
	inline void set(PropertySet<PS>& properties);
	template <typename Property>
	inline void reset(Property& property);
	template <typename Property>
	[[nodiscard]] inline const typename Property::value_type& value(Property& property) const;

private:
	bool isFocusableComponent() const noexcept;

private:
	static void eventChar(BaseComponent& component, const libv::input::EventChar& event);
	static void eventKey(BaseComponent& component, const libv::input::EventKey& event);
	static void focusChange(BaseComponent& previous, BaseComponent& current);

private:
	virtual bool onChar(const libv::input::EventChar& event); /// returns true if event is absorbed
	virtual bool onKey(const libv::input::EventKey& event); /// returns true if event is absorbed
	virtual void onFocus(const EventFocus& event);
	virtual bool onMouse(const EventMouse& event);

private:
	void attach(BaseComponent& parent);
	void detach(BaseComponent& parent);
	void style();
	void styleScan();
	libv::observer_ptr<BaseComponent> focusTravers(const ContextFocusTravers& context, BaseComponent& current);
	void render(ContextRender& context);
	void layout1(const ContextLayout1& environment);
	void layout2(const ContextLayout2& environment);

private:
	virtual void doAttach();
	virtual void doDetach();
	virtual void doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback);
	virtual void doStyle();
	virtual void doStyle(ChildID childID);
	virtual libv::observer_ptr<BaseComponent> doFocusTravers(const ContextFocusTravers& context, ChildID current);
	virtual void doCreate(ContextRender& context);
	virtual void doDestroy(ContextRender& context);
	virtual void doRender(ContextRender& context);
	virtual void doLayout1(const ContextLayout1& environment);
	virtual void doLayout2(const ContextLayout2& environment);
	virtual void doForeachChildren(libv::function_ref<bool(BaseComponent&)> callback);
	virtual void doForeachChildren(libv::function_ref<void(BaseComponent&)> callback);

	// TWO PASS layout:
	// - Pass 1: calculate everything as content bottom-top and store the result
	// - Pass 2: calculate everything top-down
};

// -------------------------------------------------------------------------------------------------

struct AccessEvent {
	static inline decltype(auto) onMouse(BaseComponent& component, const EventMouse& event) {
		return component.onMouse(event);
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
	static inline decltype(auto) doFocusTravers(BaseComponent& component, const ContextFocusTravers& context, ChildID current) {
		return component.doFocusTravers(context, current);
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
	[[nodiscard]] static inline auto& position(BaseComponent& component) noexcept {
		return component.position_;
	}
	[[nodiscard]] static inline const auto& position(const BaseComponent& component) noexcept {
		return component.position_;
	}
	[[nodiscard]] static inline auto& size(BaseComponent& component) noexcept {
		return component.size_;
	}
	[[nodiscard]] static inline const auto& size(const BaseComponent& component) noexcept {
		return component.size_;
	}

	static inline decltype(auto) eventChar(BaseComponent& component, const libv::input::EventChar& event) {
		return BaseComponent::eventChar(component, event);
	}
	static inline decltype(auto) eventKey(BaseComponent& component, const libv::input::EventKey& event) {
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
	static inline decltype(auto) focusChange(BaseComponent& previous, BaseComponent& current) {
		return BaseComponent::focusChange(previous, current);
	}
	static inline decltype(auto) focusTravers(BaseComponent& component, const ContextFocusTravers& context, BaseComponent& current) {
		return component.focusTravers(context, current);
	}
	static inline decltype(auto) render(BaseComponent& component, ContextRender& context) {
		return component.render(context);
	}

	static inline decltype(auto) setContext(BaseComponent& component, ContextUI& context) {
		component.context_ = libv::make_observer(context);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Property>
inline void BaseComponent::set(Property& property, typename Property::value_type value) {
	AccessProperty::manual(property, true);
	const bool change = AccessProperty::value(property) != value;
	if (change) {
		AccessProperty::value(property, std::move(value));
		flagAuto(property.invalidate);
	}
}

template <typename PS>
inline void BaseComponent::set(PropertySet<PS>& properties) {
	// TODO P5: I think it is possible to "type erase" these style set functions with a minimal code on the user site (? template instantiated function pointers)
	if (style_ == nullptr) {
		libv::meta::foreach_member_reference(properties, [this](auto& property) {
			using value_type = typename std::remove_reference_t<decltype(property)>::value_type;
			constexpr bool is_fallback_function = std::is_invocable_v<decltype(property.fallback), ContextUI&>;

			if (AccessProperty::manual(property))
				return;

			value_type value;

			if constexpr (is_fallback_function)
				value = property.fallback(context());
			else
				value = property.fallback;

			const bool change = value != AccessProperty::value(property);

			if (change) {
				AccessProperty::value(property, std::move(value));
				flagAuto(property.invalidate);
			}
		});
	} else {
		libv::meta::foreach_member_reference(properties, [this](auto& property) {
			using value_type = typename std::remove_reference_t<decltype(property)>::value_type;
			constexpr bool is_fallback_function = std::is_invocable_v<decltype(property.fallback), ContextUI&>;

			if (AccessProperty::manual(property))
				return;

			value_type value;

			const auto& value_by_style = style_->get_optional<value_type>(property.name);
			if (value_by_style)
				value = *value_by_style;
			else
				if constexpr (is_fallback_function)
					value = property.fallback(context());
				else
					value = property.fallback;

			const bool change = value != AccessProperty::value(property);

			if (change) {
				AccessProperty::value(property, std::move(value));
				flagAuto(property.invalidate);
			}
		});
	}
}

template <typename Property>
inline void BaseComponent::reset(Property& property) {
	using value_type = typename Property::value_type;
	constexpr bool is_fallback_function = std::is_invocable_v<decltype(property.fallback), ContextUI&>;

	if (AccessProperty::manual(property))
		return;

	AccessProperty::manual(property, false);
	value_type value;

	if (style_ == nullptr) {
		if constexpr (is_fallback_function)
			value = property.fallback(context());
		else
			value = property.fallback;
	} else {
		const auto& value_by_style = style_->get_optional<value_type>(property.name);
		if (value_by_style)
			value = *value_by_style;
		else
			if constexpr (is_fallback_function)
				value = property.fallback(context());
			else
				value = property.fallback;
	}

	const bool change = value != AccessProperty::value(property);

	if (change) {
		AccessProperty::value(property, std::move(value));
		flagAuto(property.invalidate);
	}
}

template <typename Property>
inline const typename Property::value_type& BaseComponent::value(Property& property) const {
	return AccessProperty::value(property);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
