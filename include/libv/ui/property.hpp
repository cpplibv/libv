// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

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

// -------------------------------------------------------------------------------------------------

template <typename T>
class BaseProperty {
	friend class AccessProperty;

	T value;
	bool manual = false;

public:
	using value_type = T;

public:
	inline const T& operator()() const noexcept {
		return value;
	}
};

// -------------------------------------------------------------------------------------------------

struct PropertyAlignHorizontal : BaseProperty<AlignHorizontal> {
	static constexpr std::string_view name = "align";
	static constexpr std::string_view description = "Horizontal align of the inner content of the component";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = value_type::Left;
};

struct PropertyAlignVertical : BaseProperty<AlignVertical> {
	static constexpr std::string_view name = "align_vertical";
	static constexpr std::string_view description = "Vertical align of the inner content of the component";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = value_type::Top;
};

struct PropertyAnchor : BaseProperty<Anchor> {
	static constexpr std::string_view name = "anchor";
	static constexpr std::string_view description = "Component's anchor point";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = {0.5f, 0.5f, 0.5f};
};

struct PropertyAnchorContent : BaseProperty<Anchor> {
	static constexpr std::string_view name = "anchor_content";
	static constexpr std::string_view description = "Content's anchor point";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = {0.5f, 0.5f, 0.5f};
};

struct PropertyAnchorParent : BaseProperty<Anchor> {
	static constexpr std::string_view name = "anchor_parent";
	static constexpr std::string_view description = "Parent's floating anchor point";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = {0.5f, 0.5f, 0.5f};
};

struct PropertyAnchorTarget : BaseProperty<Anchor> {
	static constexpr std::string_view name = "anchor_target";
	static constexpr std::string_view description = "Child's floating anchor point";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = {0.5f, 0.5f, 0.5f};
};

struct PropertyColumnCount : BaseProperty<ColumnCount> {
	static constexpr std::string_view name = "column_count";
	static constexpr std::string_view description = "Column count of the secondary dimension";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static constexpr value_type fallback = 2;
};

struct PropertyColor : BaseProperty<Color> {
	static constexpr std::string_view name = "color";
	static constexpr std::string_view description = "Primary color of the displayed component";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static constexpr value_type fallback = {1.f, 1.f, 1.f, 1.f};
};

struct PropertyFont : BaseProperty<Font2D_view> {
	static constexpr std::string_view name = "font";
	static constexpr std::string_view description = "Font file path";
	static constexpr Flag_t invalidate = Flag::pendingLayout | Flag::pendingRender;
	static value_type fallback(ContextUI& context);
};

struct PropertyFontColor : BaseProperty<Color> {
	static constexpr std::string_view name = "font_color";
	static constexpr std::string_view description = "Font color";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static constexpr value_type fallback = {1.f, 1.f, 1.f, 1.f};
};

struct PropertyFontSize : BaseProperty<FontSize> {
	static constexpr std::string_view name = "font_size";
	static constexpr std::string_view description = "Font size in pixel";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = value_type{12};
};

struct PropertyImage : BaseProperty<Texture2D_view> {
	static constexpr std::string_view name = "image";
	static constexpr std::string_view description = "Image file path";
	static constexpr Flag_t invalidate = Flag::pendingLayout | Flag::pendingRender;
	static value_type fallback(ContextUI& context);
};

struct PropertyOrientation : BaseProperty<Orientation> {
	static constexpr std::string_view name = "orientation";
	static constexpr std::string_view description = "Orientation of subsequent components";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = Orientation::LEFT_TO_RIGHT;
};

struct PropertyOrientation2 : BaseProperty<Orientation2> {
	static constexpr std::string_view name = "orientation2";
	static constexpr std::string_view description = "Two dimensional orientation of subsequent components";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = Orientation2::RIGHT_DOWN;
};

struct PropertyShaderFont : BaseProperty<ShaderFont_view> {
	static constexpr std::string_view name = "font_shader";
	static constexpr std::string_view description = "Font shader";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static value_type fallback(ContextUI& context);
};

struct PropertyShaderImage : BaseProperty<ShaderImage_view> {
	static constexpr std::string_view name = "image_shader";
	static constexpr std::string_view description = "Image shader";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static value_type fallback(ContextUI& context);
};

struct PropertyShaderQuad : BaseProperty<ShaderQuad_view> {
	static constexpr std::string_view name = "quad_shader";
	static constexpr std::string_view description = "Quad shader";
	static constexpr Flag_t invalidate = Flag::pendingRender;
	static value_type fallback(ContextUI& context);
};

struct PropertySize : BaseProperty<Size> {
	static constexpr std::string_view name = "size";
	static constexpr std::string_view description = "Component size in pixel, percent, ratio and dynamic units";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
//	static Flag_t invalidate(const value_type& value);
	static constexpr value_type fallback = value_type{};
};

struct PropertySnapToEdge : BaseProperty<SnapToEdge> {
	static constexpr std::string_view name = "snap_to_edge";
	static constexpr std::string_view description = "Snap to edge any child that otherwise would hang out";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = false;
};

struct PropertySquish : BaseProperty<Squish> {
	static constexpr std::string_view name = "squish";
	static constexpr std::string_view description = "Squish any child that otherwise would hang out";
	static constexpr Flag_t invalidate = Flag::pendingLayout;
	static constexpr value_type fallback = false;
};

// -------------------------------------------------------------------------------------------------

using Debug = std::string;
struct PropertyDebug : BaseProperty<Debug> {
	static constexpr std::string_view name = "debug";
	static constexpr std::string_view description = "Debug value";
	static constexpr Flag_t invalidate = Flag::none;
	// static constexpr value_type fallback = ""; // C++20
	static inline const value_type fallback = "";
};

// -------------------------------------------------------------------------------------------------

using PropertyDynamic = std::variant<
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

		Debug

		// ---

//		PropertyAlignHorizontal,
//		PropertyAlignVertical,
//		PropertyAnchor,
//		PropertyAnchorContent,
//		PropertyAnchorParent,
//		PropertyAnchorTarget,
//		PropertyColumnCount,
//		PropertyColor,
//		PropertyFont,
//		PropertyFontColor,
//		PropertyFontSize,
//		PropertyImage,
//		PropertyOrientation,
//		PropertyOrientation2,
//		PropertyShaderFont,
//		PropertyShaderImage,
//		PropertyShaderQuad,
//		PropertySize,
//		PropertySnapToEdge,
//		PropertySquish,
//
//		PropertyDebug
>;

// -------------------------------------------------------------------------------------------------

//template <typename T>
//class Property {
//	friend class AccessProperty;
//
//	T value;
//	bool manual = false;
//
//public:
//	using value_type = T;
//
//public:
//	inline operator const T&() const noexcept {
//		return value;
//	}
////	inline const T* operator->() const noexcept {
////		return &value;
////	}
//	inline const T& operator*() const noexcept {
//		return value;
//	}
//};

struct AccessProperty {
public:
	template <typename T>
	static constexpr inline void manual(BaseProperty<T>& property, bool manual) noexcept {
		property.manual = manual;
	}
	template <typename T>
	static constexpr inline bool manual(const BaseProperty<T>& property) noexcept {
		return property.manual;
	}
	template <typename T>
	static constexpr inline void value(BaseProperty<T>& property, T value) noexcept {
		property.value = std::move(value);
	}
	template <typename T>
	static constexpr inline const T& value(const BaseProperty<T>& property) noexcept {
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
