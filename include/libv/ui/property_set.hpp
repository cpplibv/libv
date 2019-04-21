// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
// std
#include <string_view>
#include <type_traits>
// pro
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class PropertySet : public CRTP {
public:
	PropertySet() {
		const auto ar = [this](auto& target, const std::string_view name, const std::string_view description, const auto& fallback) {
			(void) name;
			(void) description;
			target = fallback;
		};

		this->properties(ar);
	}

	PropertySet(const Style& style) {
		set(style);
	}

	~PropertySet() {
	}

	void set(const Style& style) {
		const auto ar = [this, &style](auto& target, const std::string_view name, const std::string_view description, const auto& fallback) {
			(void) description;
			using T = std::decay_t<decltype(target)>;

			const auto& value = style.get_optional<T>(name);
			target = value ? std::get<T>(*value) : fallback;
		};

		this->properties(ar);
	}

	inline void set(const libv::intrusive_ptr<Style>& style) {
		set(*style);
	}

	//	constexpr int32_t num_properties() const {
	//		int32_t i = 0;
	//		this->properties([&i](const auto&&...){ i++; });
	//		return i;
	//	}

	/// [this](const auto& target, const std::string_view name, const std::string_view description, const auto& fallback) {
	/// 	log_ui.debug("{} = {} (default: {}) {}", name, target, fallback, description);
	/// }
	template <typename F>
	void foreach(F&& f) {
		this->properties(f);
	}
};

// -------------------------------------------------------------------------------------------------

namespace PS {
struct Component {
	Size size;

	template <typename Archive>
	constexpr void properties(Archive& ar) {
		ar(size, "size", "Component layout size", Size{});
	}

//	template <typename Archive>
//	constexpr void properties(Archive& ar) {
//		(void) ar;
//	}
};

struct Label : Component {
	AlignHorizontal align;
	Font2D_view font;
	Color font_color;
	FontSize font_size;

	template <typename Archive>
	constexpr void properties(Archive& ar) {
		Component::properties(ar);

		// <<< P4: default font should not be null
		ar(font, "font", "Font file path", nullptr);
		ar(font_color, "font_color", "Font color", Color{1, 1, 1, 1});
		ar(font_size, "font_size", "Font size in pixel", FontSize{14});
		ar(align, "align", "Text vertical alignment", AlignHorizontal::Left);
	}
};
} // namespace PS

using PropertySetLabel = PropertySet<PS::Label>;

// ar & Descriptor(font, "font", "The font to use in the label. The label can only contain text in single font.");
// ar & Descriptor(shader, "shader", "Font shader");
// ar & Descriptor(color, "color", "Font color", Color{1, 1, 1, 1});

// -------------------------------------------------------------------------------------------------

// Property evaluation is during attach time - Or its own time(?)
// NOTE: enable/disable and show/hide are Not a property but flags

// Heritage:
//		Normal - only effecting current
//		   background
//		   border
//		   layout
//		   margin
//		   onEvent
//		   padding

	//		   name
	//		   size
	//		   color
	//		   font_size
	//		   font_color

//		Inheritance - inheritance until overridden
//		   font
//		   font_shader
//		   icon_enable
//
//		Additive - additive union
//		   icon_set

//		struct FontEffect {};
//		struct FontFamily {};
//		struct FontSize {}; //font-size/line-height
//		struct FontStyle {}; //font-style - Specifies the font style. Default value is "normal". See font-style for possible values
//		struct FontVariant {}; //font-variant - Specifies the font variant. Default value is "normal". See font-variant for possible values
//		struct FontWeight {}; //font-weight - Specifies the font weight. Default value is "normal". See font-weight for possible values
//
//		struct Alignment {};
//		struct Orientation {};
//		struct TextAlignment {};
//		struct Anchor {};
//		struct FontColor { vec4f value; };
//		struct Image {};
//		struct Background {};
//		struct Layout {};
//
//		Text					The text to display in the label. The text can contain newlines.
//		textvariable			Associates a Tkinter variable (usually a StringVar) with the label. If the variable is changed, the label text is updated. (textVariable/Variable)
//		wordWrap
//
//		align						Horizontal align of the inner content of the component
//		vertical_align				Vertical align of the inner content of the component
//		size_min
//		size_max
//		indent
//		margin
//
//		NoTextInteraction			No interaction with the text is possible.
//		LinksAccessibleByKeyboard	Links can be focused using tab and activated with enter.
//		LinksAccessibleByMouse		Links can be highlighted and activated with the mouse.
//		TabIndex / Focusable		If true, the widget accepts input focus. The default is false. (takeFocus/TakeFocus)
//		TextSelectableByKeyboard	Text can be selected with the cursor keys on the keyboard.
//		TextSelectableByMouse		Text can be selected with the mouse and copied to the clipboard using a context menu or standard keyboard shortcuts.
//		ar & Descriptor(Property::Size, Mandatory, "Determines the maximum size of the text");
//		ar & Descriptor(Property::Align, Default(LEFT), "Alignment of the text");
//		ar & Descriptor(Property::Orient, Default(RIGHT), "Orientation of the text");
//
//		cell_spacing				Space between the table cell contents and border.
//		horizontally_stretchable	If the GUI element stretches its size horizontally to other elements
//		vertically_stretchable		If the GUI element stretches its size vertically to other elements
//		horizontally_squashable		If the GUI element can be squashed (by maximal with of some parent element) horizontally This is mainly meant to be used for scroll-pane The default value is false
//		vertically_squashable		If the GUI element can be squashed (by maximal height of some parent element) vertically This is mainly meant to be used for scroll-pane The default (parent) value for scroll pane is true, false otherwise
//		horizontal_spacing			Horizontal space between individual cells.
//		vertical_spacing			Vertical space between individual cells.

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
