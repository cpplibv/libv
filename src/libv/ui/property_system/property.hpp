// Project: libv.ui, File: src/libv/ui/property_system/property.hpp

#pragma once

// std
#include <any>
#include <string_view>
#include <utility>
// pro
#include <libv/ui/component_system/flag.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using PropertyDynamic = std::any;

namespace pnm { // ---------------------------------------------------------------------------------

static constexpr std::string_view align_horizontal = "align_horizontal";
static constexpr std::string_view align_vertical = "align_vertical";
static constexpr std::string_view anchor = "anchor";
static constexpr std::string_view area_position = "area_position";
static constexpr std::string_view area_size = "area_size";
static constexpr std::string_view background = "background";
// static constexpr std::string_view bar_color = "bar_color";
// static constexpr std::string_view bar_image = "bar_image";
// static constexpr std::string_view bar_shader = "bar_shader";
static constexpr std::string_view bar_visual = "bar_visual";
static constexpr std::string_view caret = "caret";
static constexpr std::string_view caret_color = "caret_color";
static constexpr std::string_view caret_shader = "caret_shader";
static constexpr std::string_view color = "color";
static constexpr std::string_view column_count = "column_count";
static constexpr std::string_view focus_mode = "focus_mode";
static constexpr std::string_view focus_select_policy = "focus_select_policy";
static constexpr std::string_view font = "font";
static constexpr std::string_view font_color = "font_color";
static constexpr std::string_view font_shader = "font_shader";
static constexpr std::string_view font_size = "font_size";
static constexpr std::string_view margin = "margin";
static constexpr std::string_view orientation = "orientation";
static constexpr std::string_view orientation2 = "orientation2";
static constexpr std::string_view padding = "padding";
static constexpr std::string_view quad_shader = "quad_shader";
static constexpr std::string_view scroll_mode = "scroll_mode";
static constexpr std::string_view selection = "selection";
static constexpr std::string_view size = "size";
static constexpr std::string_view snap_to_edge = "snap_to_edge";
static constexpr std::string_view spacing = "spacing";
static constexpr std::string_view spacing2 = "spacing2";
static constexpr std::string_view squish = "squish";
static constexpr std::string_view text = "text";
static constexpr std::string_view text_off = "text_off";
static constexpr std::string_view text_on = "text_on";
static constexpr std::string_view value = "value";
static constexpr std::string_view value_high = "value_high";
static constexpr std::string_view value_low = "value_low";
static constexpr std::string_view value_range = "value_range";
static constexpr std::string_view value_step = "value_step";
static constexpr std::string_view z_index_offset = "z_index_offset";

} // namespace pnm ---------------------------------------------------------------------------------
namespace pgr { // ---------------------------------------------------------------------------------

static constexpr std::string_view appearance = "appearance";
static constexpr std::string_view behaviour = "behaviour";
static constexpr std::string_view common = "common";
static constexpr std::string_view font = "font";
static constexpr std::string_view layout = "layout";

} // namespace pgr ---------------------------------------------------------------------------------

enum class PropertyDriver : uint8_t {
	style = 0,
	manual,
	// animation
};

// -------------------------------------------------------------------------------------------------

struct AccessProperty;

class BaseProperty {
	friend AccessProperty;

private:
	PropertyDriver driver = PropertyDriver::style;
	bool changed = true;

public:
	constexpr inline bool consumeChange() noexcept {
		bool result = changed;
		changed = false;
		return result;
	}
};

template <typename T = void>
class Property : public BaseProperty {
	friend AccessProperty;

public:
	using value_type = T;

private:
	T value;

public:
	constexpr inline const value_type& operator()() const noexcept {
		return value;
	}
};

template <>
class Property<void> : public BaseProperty {
	friend AccessProperty;
};

/// PropertyB: Behaviour, no change in layout nor render
template <typename T = void>
class PropertyB : public Property<T> {
public:
	static constexpr bool invalidate_render = false;
	static constexpr bool invalidate_layout1 = false;
	static constexpr bool invalidate_layout2 = false;
	static constexpr bool invalidate_parent_layout = false;
};

template <typename T = void>
class PropertyR : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = false;
	static constexpr bool invalidate_layout2 = false;
	static constexpr bool invalidate_parent_layout = false;
};

template <typename T = void>
class PropertyL1 : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = true;
	static constexpr bool invalidate_layout2 = false;
	static constexpr bool invalidate_parent_layout = false;
};

template <typename T = void>
class PropertyL2 : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = false;
	static constexpr bool invalidate_layout2 = true;
	static constexpr bool invalidate_parent_layout = false;
};

template <typename T = void>
class PropertyLP : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = false;
	static constexpr bool invalidate_layout2 = false;
	static constexpr bool invalidate_parent_layout = true;
};

template <typename T = void>
class PropertyL1L2 : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = true;
	static constexpr bool invalidate_layout2 = true;
	static constexpr bool invalidate_parent_layout = false;
};

template <typename T = void>
class PropertyL1L2LP : public Property<T> {
public:
	static constexpr bool invalidate_render = true;
	static constexpr bool invalidate_layout1 = true;
	static constexpr bool invalidate_layout2 = true;
	static constexpr bool invalidate_parent_layout = true;
};

// -------------------------------------------------------------------------------------------------

struct AccessProperty {
	template <typename P>
	static constexpr inline auto driver(const P& property) noexcept {
		return property.driver;
	}

	template <typename P>
	static constexpr inline void driver(P& property, PropertyDriver driver) noexcept {
		property.driver = driver;
	}

	template <typename P, typename TC>
	static constexpr inline void force_value(P& property, TC&& value) noexcept {
		property.value = std::forward<TC>(value);
	}

	template <typename Component, typename P, typename TC>
	static constexpr inline void value(Component& component, P& property, TC&& value) noexcept {
		if (property.value == value)
			return;

		property.value = std::forward<TC>(value);
		property.changed = true;
		if (property.invalidate_layout1 || property.invalidate_layout2 || property.invalidate_parent_layout)
			component.markInvalidLayout(property.invalidate_layout1, property.invalidate_parent_layout);
		if (property.invalidate_render)
			component.flagAuto(Flag::pendingRender);
	}

	template <typename Component, typename P, typename F>
	static constexpr inline void setter(Component& component, P& property, PropertyDriver driver, F&& func) noexcept {
		if (driver != PropertyDriver::manual)
			return;

		func();

		property.driver = driver;
		property.changed = true;
		if (property.invalidate_layout1 || property.invalidate_layout2 || property.invalidate_parent_layout)
			component.markInvalidLayout(property.invalidate_layout1, property.invalidate_parent_layout);
		if (property.invalidate_render)
			component.flagAuto(Flag::pendingRender);
	}

	template <typename Component, typename P>
	static constexpr inline void changed(Component& component, P& property) noexcept {
		property.changed = true;
		if (property.invalidate_layout1 || property.invalidate_layout2 || property.invalidate_parent_layout)
			component.markInvalidLayout(property.invalidate_layout1, property.invalidate_parent_layout);
		if (property.invalidate_render)
			component.flagAuto(Flag::pendingRender);
	}

	// ---

	template <typename Component, typename P, typename TC>
	static constexpr inline void manual(Component& component, P& property, TC&& value) noexcept {
		property.driver = PropertyDriver::manual;

		if (property.value == value)
			return;

		property.value = std::forward<TC>(value);
		property.changed = true;
		if (property.invalidate_layout1 || property.invalidate_layout2 || property.invalidate_parent_layout)
			component.markInvalidLayout(property.invalidate_layout1, property.invalidate_parent_layout);
		if (property.invalidate_render)
			component.flagAuto(Flag::pendingRender);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
