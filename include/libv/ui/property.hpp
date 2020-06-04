// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// libv
#include <libv/utility/intrusive_ptr.hpp>
//#include <libv/utility/fixed_string.hpp>
// std
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
// pro
#include <libv/ui/flag.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/orientation2.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/squish.hpp>
#include <libv/ui/property/texture_2D.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;

class ShaderFont;
using ShaderFont_view = std::shared_ptr<ShaderFont>;

class ShaderQuad;
using ShaderQuad_view = std::shared_ptr<ShaderQuad>;

using Style_view = libv::intrusive_ptr<Style>;

// -------------------------------------------------------------------------------------------------

using PropertyDynamic = std::variant<
		float,
		Style_view,

		AlignHorizontal,
		AlignVertical,
		Anchor,
		Color,
		ColumnCount,
		Font2D_view,
		FontSize,
		// FontColor,
		Texture2D_view,
		Orientation,
		Orientation2,
		ShaderFont_view,
		ShaderImage_view,
		ShaderQuad_view,
		Size,
		SnapToEdge,
		Squish,

		std::string // For debug only
>;

namespace pnm { // ---------------------------------------------------------------------------------

// NOTE: It is not necessary to store the fixed_strings in type and in here, it is merely a forethought and typo protection

static constexpr std::string_view align_horizontal = "align";
static constexpr std::string_view align_vertical = "align_vertical";
static constexpr std::string_view anchor = "anchor";
static constexpr std::string_view anchor_content = "anchor_content";
static constexpr std::string_view anchor_parent = "anchor_parent";
static constexpr std::string_view anchor_target = "anchor_target";
static constexpr std::string_view bar_color = "bar_color";
static constexpr std::string_view bar_image = "bar_image";
static constexpr std::string_view bar_shader = "bar_shader";
static constexpr std::string_view bg_color = "bg_color";
static constexpr std::string_view bg_image = "bg_image";
static constexpr std::string_view bg_shader = "bg_shader";
static constexpr std::string_view caret = "caret";
static constexpr std::string_view caret_color = "caret_color";
static constexpr std::string_view caret_shader = "caret_shader";
static constexpr std::string_view color = "color";
static constexpr std::string_view column_count = "column_count";
static constexpr std::string_view focus_select_policy = "focus_select_policy";
static constexpr std::string_view font = "font";
static constexpr std::string_view font_color = "font_color";
static constexpr std::string_view font_shader = "font_shader";
static constexpr std::string_view font_size = "font_size";
static constexpr std::string_view orientation = "orientation";
static constexpr std::string_view orientation2 = "orientation2";
static constexpr std::string_view quad_shader = "quad_shader";
static constexpr std::string_view size = "size";
static constexpr std::string_view snap_to_edge = "snap_to_edge";
static constexpr std::string_view squish = "squish";
static constexpr std::string_view text = "text";
static constexpr std::string_view value = "value";
static constexpr std::string_view value_max = "value_max";
static constexpr std::string_view value_min = "value_min";
static constexpr std::string_view value_range = "value_range";
static constexpr std::string_view value_step = "value_step";

} // namespace pnm ---------------------------------------------------------------------------------
namespace pgr { // ---------------------------------------------------------------------------------

// NOTE: It is not necessary to store the fixed_strings in type and in here, it is merely a forethought and typo protection

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

class BaseProperty {
	friend class AccessProperty;

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
	friend class AccessProperty;

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
	friend class AccessProperty;
};

template <typename T = void>
class PropertyB : public Property<T> {
public:
	static constexpr Flag_t invalidate = Flag::none;
};

template <typename T = void>
class PropertyR : public Property<T> {
public:
	static constexpr Flag_t invalidate = Flag::pendingRender;
};

template <typename T = void>
class PropertyL : public Property<T> {
public:
	static constexpr Flag_t invalidate = Flag::pendingLayout | Flag::pendingRender;
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

	template <typename Component, typename P, typename TC>
	static constexpr inline void value(Component& component, P& property, TC&& value) noexcept {
		property.value = std::move(value);
		property.changed = true;
		component.flagAuto(property.invalidate);
	}

	template <typename Component, typename P, typename F>
	static constexpr inline void setter(Component& component, P& property, PropertyDriver driver, F&& func) noexcept {
		if (property.changed && driver != PropertyDriver::manual)
			return;

		property.driver = driver;
		property.changed = true;
		component.flagAuto(property.invalidate);

		func();
	}

	// ---

	template <typename Component, typename P, typename TC>
	static constexpr inline void manual(Component& component, P& property, TC&& value) noexcept {
		property.driver = PropertyDriver::manual;

		if (property.value == value)
			return;

		property.value = std::forward<TC>(value);
		property.changed = true;
		component.flagAuto(property.invalidate);
	}
};

// -------------------------------------------------------------------------------------------------

// enable/disable and show/hide are Not a property but flags
//
// Heritage:
//		struct FontEffect {};
//		struct FontFamily {};
//		struct FontSize {}; //font-size/line-height
//		struct FontStyle {}; //font-style - Specifies the font style. Default value is "normal". See font-style for possible values
//		struct FontVariant {}; //font-variant - Specifies the font variant. Default value is "normal". See font-variant for possible values
//		struct FontWeight {}; //font-weight - Specifies the font weight. Default value is "normal". See font-weight for possible values
//
//		text						The text to display in the label. The text can contain newlines.
//
//		NoTextInteraction			No interaction with the text is possible.
//		LinksAccessibleByKeyboard	Links can be focused using tab and activated with enter.
//		LinksAccessibleByMouse		Links can be highlighted and activated with the mouse.
//		TabIndex / Focusable		If true, the widget accepts input focus. The default is false. (takeFocus/TakeFocus)
//		TextSelectableByKeyboard	Text can be selected with the cursor keys on the keyboard.
//		TextSelectableByMouse		Text can be selected with the mouse and copied to the clipboard using a context menu or standard keyboard shortcuts.
//
//		horizontally_stretchable	If the GUI element stretches its size horizontally to other elements
//		vertically_stretchable		If the GUI element stretches its size vertically to other elements
//		horizontally_squashable		If the GUI element can be squashed (by maximal with of some parent element) horizontally This is mainly meant to be used for scroll-pane The default value is false
//		vertically_squashable		If the GUI element can be squashed (by maximal height of some parent element) vertically This is mainly meant to be used for scroll-pane The default (parent) value for scroll pane is true, false otherwise
//		cell_spacing				Space between the table cell contents and border.
//		horizontal_spacing			Horizontal space between individual cells.
//		vertical_spacing			Vertical space between individual cells.

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
