// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// libv
#include <libv/utility/intrusive_ptr.hpp>
#include <libv/utility/fixed_string.hpp>
// std
#include <memory>
#include <string>
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
static constexpr libv::fixed_string color = "color";
static constexpr libv::fixed_string column_count = "column_count";
static constexpr libv::fixed_string cursor_color = "cursor_color";
static constexpr libv::fixed_string cursor_shader = "cursor_shader";
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

} // namespace pnm ---------------------------------------------------------------------------------

template <typename T>
concept bool C_Property = requires(T var) {
		{ T::name } -> std::string_view;
		{ T::invalidate } -> Flag_t;

		typename T::tagProperty;
		typename T::value_type;
};

template <typename T, Flag_t::value_type I, libv::fixed_string Name>
class Property {
	friend class AccessProperty;

	T value;
	bool manual = false;
	bool changed = false;

public:
	using tagProperty = void;
	using value_type = T;

	static constexpr std::string_view name = Name;
	static constexpr Flag_t invalidate{I};

	constexpr inline const T& operator()() const noexcept {
		return value;
	}

	bool consumeChange() {
		bool result = changed;
		changed = false;
		return result;
	}
};

//template <typename T, Flag_t::value_type I, auto Set, auto Get>
//class Property7DD {
//	friend class AccessProperty;
//
//	bool manual = false;
//	bool changed = false;
//
//public:
//	static constexpr Flag_t invalidate{I};
//	using value_type = T;
//};

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
		// Squish,

		std::string // For debug only
>;

// -------------------------------------------------------------------------------------------------

struct AccessProperty {
public:
	template <typename Property>
	static constexpr inline void manual(Property& property, bool manual) noexcept {
		property.manual = manual;
	}
	template <typename Property>
	static constexpr inline bool manual(const Property& property) noexcept {
		return property.manual;
	}
	template <typename Property>
	static constexpr inline void value(Property& property, typename Property::value_type value) noexcept {
		property.value = std::move(value);
		property.changed = true;
	}
	template <typename Property>
	static constexpr inline const auto& value(const Property& property) noexcept {
		return property.value;
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
