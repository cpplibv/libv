// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// libv
#include <libv/utility/intrusive_ptr.hpp>
#include <libv/utility/fixed_string.hpp>
// std
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
// pro
#include <libv/ui/flag.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/orientation2.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/squish.hpp>
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;

class Font2D;
using Font2D_view = std::shared_ptr<Font2D>;

class Texture2D;
using Texture2D_view = std::shared_ptr<Texture2D>;

class ShaderFont;
using ShaderFont_view = std::shared_ptr<ShaderFont>;

class ShaderImage;
using ShaderImage_view = std::shared_ptr<ShaderImage>;

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

// NOTE: It is not necessary to store the fixed_strings here, it is merely a forethought and typo protection

static constexpr libv::fixed_string align_horizontal = "align";
static constexpr libv::fixed_string align_vertical = "align_vertical";
static constexpr libv::fixed_string anchor = "anchor";
static constexpr libv::fixed_string anchor_content = "anchor_content";
static constexpr libv::fixed_string anchor_parent = "anchor_parent";
static constexpr libv::fixed_string anchor_target = "anchor_target";
static constexpr libv::fixed_string bg_color = "bg_color";
static constexpr libv::fixed_string bg_image = "bg_image";
static constexpr libv::fixed_string bg_shader = "bg_shader";
static constexpr libv::fixed_string caret_color = "caret_color";
static constexpr libv::fixed_string caret_shader = "caret_shader";
static constexpr libv::fixed_string color = "color";
static constexpr libv::fixed_string column_count = "column_count";
static constexpr libv::fixed_string focus_select_policy = "focus_select_policy";
static constexpr libv::fixed_string font = "font";
static constexpr libv::fixed_string font_color = "font_color";
static constexpr libv::fixed_string font_shader = "font_shader";
static constexpr libv::fixed_string font_size = "font_size";
static constexpr libv::fixed_string orientation = "orientation";
static constexpr libv::fixed_string orientation2 = "orientation2";
static constexpr libv::fixed_string quad_shader = "quad_shader";
static constexpr libv::fixed_string size = "size";
static constexpr libv::fixed_string snapToEdge = "snapToEdge";
static constexpr libv::fixed_string squish = "squish";
static constexpr libv::fixed_string text = "text";

} // namespace pnm ---------------------------------------------------------------------------------
namespace pgr { // ---------------------------------------------------------------------------------

// NOTE: It is not necessary to store the fixed_strings here, it is merely a forethought and typo protection

static constexpr libv::fixed_string appearance = "appearance";
static constexpr libv::fixed_string behaviour = "behaviour";
static constexpr libv::fixed_string common = "common";
static constexpr libv::fixed_string font = "font";
static constexpr libv::fixed_string layout = "layout";

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
	bool changed = false;

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

	template <typename Component, typename P>
	static constexpr inline bool setter(Component& component, P& property, PropertyDriver driver) noexcept {
		if (property.changed && driver != PropertyDriver::manual)
			return true;
		property.driver = driver;
		property.changed = true;
		component.flagAuto(property.invalidate);
		return false;
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
