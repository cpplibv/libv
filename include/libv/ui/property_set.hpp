// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// std
#include <string_view>
#include <type_traits>
// pro
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_size.hpp>
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

//Property<bool> propertyEnable = (
//		"enable", Inheritable, true,
//		"Enable or disable input events");
//
//Property<libv::vec4f> propertyColor = (
//		"color", Inheritable, {1, 1, 1, 1},
//		"Enable or disable input events");
//
//Property<libv::vec4f> propertyPadding = (
//		"padding", Inheritable, {0, 0, 0, 0},
//		"Enable or disable input events");
//
//Property<libv::vec4f> propertyMargin = (
//		"margin", Inheritable, {0, 0, 0, 0},
//		"Enable or disable input events");
//
//Property<std::shared_ptr<Font2D>> propertyFont = (
//		"font", Inheritable, fallback_font,
//		"Enable or disable input events");
//
//Property<int32_t> propertyFontSize = (
//		"font-size", Inheritable, 14,
//		"Enable or disable input events");

// -------------------------------------------------------------------------------------------------

namespace PS {
struct Component {
	template <typename Archive>
	constexpr void properties(Archive& ar) {
		(void) ar;
	}
};

struct Label : Component {
	Color color;
	Font2D_view font;
	FontSize font_size;
	Anchor align;

	template <typename Archive>
	constexpr void properties(Archive& ar) {
		Component::properties(ar);

		ar(color, "color", "Font color", Color{1, 1, 1, 1});
		// <<< P4: default font should not be null
		ar(font, "font", "Font file path", nullptr);
		ar(font_size, "font_size", "Font size in pixel", FontSize{14});
		ar(align, "align", "Text vertical alignment", Anchor::Left);
//		ar(color2, "color2", "Font color2 description", libv::ui::Color{0, 0, 0, 1});
	}
};
} // namespace PS

using PropertySetLabel = PropertySet<PS::Label>;

// =================================================================================================

// Property evaluation is during attach time - Or its own time(?)

	// Heritage:
	//		Lock - forced inheritance >> Not a property but flags
	//		   enable/disable
	//		   show/hide
	//
	//		Normal - only effecting current
	//		   background
	//		   border
	//		   layout
	//		   margin
	//		   name
	//		   onEvent
	//		   padding
	//		   size
	//		   color
	//
	//		Inheritance - inheritance until overridden
	//		   font
	//		   font_size
	//		   font_shader
	//		   font_color
	//		   icon_enable
	//
	//		Additive - additive union
	//		   icon_set

	// Providing:
	//		Mandatory
	//		Optional
	//		Defaulted


//struct StyleComponent : Style {
//	Property<Size> size; // Style based size? Or per component instance based? Or style default, component soft override?
//	Property<bool> enabled;
//	Property<bool> show; // This would only disable render, but still account during layout, not sure
////	Property<Mode> mode; // Instead of show + enable a single mode? Render 0/1, Interactive 0/1, Layout 0/1
//	Property<libv::vec4f> padding;
//
//	template <typename Archive, typename Descriptor>
//	void serialize_property(Archive& ar) {
////		ar & Descriptor(font, "font", "The font to use in the label. The label can only contain text in single font.");
//	}
//};

//struct StyleCache {
//	std::unordered_map<std::string, std::weak_ptr<Style>> cache;
//	template <typename T>
//	T fetch(const std::string_view key) {
//		return cache[key];
//	}
//};
//
//struct StylePanel : Style {
////	layout = layout_float
//	Property<int32_t> layout;
//
//	template <typename Archive, typename Descriptor>
//	void serialize_property(Archive& ar) {
//		Style::template serialize_property<Archive, Descriptor>(ar);
//
//		ar & Descriptor(layout, "layout", "...");
//	}
//};

//struct StyleLabel : StyleComponent {
////	Property<Font2D> font;
////	Property<FontSize> font_size;
////	Property<ShaderFont> shader;
//////	Property<std::shared_ptr<Font2D>> font;
//////	Property<std::shared_ptr<Shader>> shader;
////	Property<Color> color;
////	//Property<Indent> indent;
//
//	template <typename Archive, typename Descriptor>
//	void serialize_property(Archive& ar) {
////		StyleComponent::template serialize_property<Archive, Descriptor>(ar);
//
////		ar & Descriptor(font, "font", "The font to use in the label. The label can only contain text in single font.");
////		ar & Descriptor(shader, "shader", "Font shader");
////		ar & Descriptor(color, "color", "Font color", Color{1, 1, 1, 1});
//
////		struct FontEffect {};
////		struct FontFamily {};
////		struct FontSize {}; //font-size/line-height
////		struct FontStyle {}; //font-style - Specifies the font style. Default value is "normal". See font-style for possible values
////		struct FontVariant {}; //font-variant - Specifies the font variant. Default value is "normal". See font-variant for possible values
////		struct FontWeight {}; //font-weight - Specifies the font weight. Default value is "normal". See font-weight for possible values
////
////		struct Alignment {};
////		struct Orientation {};
////		struct TextAlignment {};
////		struct Anchor {};
////		struct FontColor { vec4f value; };
////		struct Image {};
////		struct Background {};
////		struct Layout {};
////
////		Text					The text to display in the label. The text can contain newlines.
////		textvariable			Associates a Tkinter variable (usually a StringVar) with the label. If the variable is changed, the label text is updated. (textVariable/Variable)
////		wordWrap
////
////		align						Horizontal align of the inner content of the component
////		vertical_align				Vertical align of the inner content of the component
////		size_min
////		size_max
////		indent
////		margin
////
////		NoTextInteraction			No interaction with the text is possible.
////		LinksAccessibleByKeyboard	Links can be focused using tab and activated with enter.
////		LinksAccessibleByMouse		Links can be highlighted and activated with the mouse.
////		TabIndex / Focusable		If true, the widget accepts input focus. The default is false. (takeFocus/TakeFocus)
////		TextSelectableByKeyboard	Text can be selected with the cursor keys on the keyboard.
////		TextSelectableByMouse		Text can be selected with the mouse and copied to the clipboard using a context menu or standard keyboard shortcuts.
////		ar & Descriptor(Property::Size, Mandatory, "Determines the maximum size of the text");
////		ar & Descriptor(Property::Align, Default(LEFT), "Alignment of the text");
////		ar & Descriptor(Property::Orient, Default(RIGHT), "Orientation of the text");
////
////		cell_spacing				Space between the table cell contents and border.
////		horizontally_stretchable	If the GUI element stretches its size horizontally to other elements
////		vertically_stretchable		If the GUI element stretches its size vertically to other elements
////		horizontally_squashable		If the GUI element can be squashed (by maximal with of some parent element) horizontally This is mainly meant to be used for scroll-pane The default value is false
////		vertically_squashable		If the GUI element can be squashed (by maximal height of some parent element) vertically This is mainly meant to be used for scroll-pane The default (parent) value for scroll pane is true, false otherwise
////		horizontal_spacing			Horizontal space between individual cells.
////		vertical_spacing			Vertical space between individual cells.
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
