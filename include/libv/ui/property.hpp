// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// Property evaluation is during attach time! Or its own time(?)

// * Properties:
// * Type of default behavior for property inheritance:
// * - Lock: forced inheritance (enable/disable)
// * - Normal: only effecting current (background, padding, show/hide)
// * - Overridable: inheritance until overridden (font, font_color)

//			//boost::container::flat_map<PropertyID, Property> properties;


// TODO P2: PropertyArchive, serialize_property
//	template <typename Archive, typename Descriptor>
//	void serialize_property(Archive& ar) {
//		ar & Descriptor(Property::Size, Mandatory, "Determines the maximum size of the text");
//		ar & Descriptor(Property::Boundary, Optional, "Determines the boundary policy");
//		ar & Descriptor(Property::Align, Default(LEFT), "Alignment of the text");
//		ar & Descriptor(Property::Orient, Default(RIGHT), "Orientation of the text");
//	}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================

//
//using FontProperty = std::variant<
//		FontEffect, FontFamily, FontSize, FontStyle, FontVariant, FontWeight
//		>;
//using StyleProperty = std::variant<
//		Background //, Foreground, Border
//		>;
//using LayoutProperty = std::variant<
//		Alignment //		Margin, Padding, Border
//		>;
//using Property = std::variant<
//		FontProperty, StyleProperty, LayoutProperty
//		>;

// =================================================================================================

//struct FontEffect {};
//struct FontFamily {};
//struct FontSize {}; //font-size/line-height
//struct FontStyle {}; //font-style - Specifies the font style. Default value is "normal". See font-style for possible values
//struct FontVariant {}; //font-variant - Specifies the font variant. Default value is "normal". See font-variant for possible values
//struct FontWeight {}; //font-weight - Specifies the font weight. Default value is "normal". See font-weight for possible values
//
//struct Text {};
//
//struct Program {};
//
//struct Alignment {};
////struct Orientation {};
//
////struct TextAlignment {};
//
////struct Anchor {};
////struct FontColor { vec4f value; };
////	anchor
////	background
////	font_color = #FFFFFFFF
////	layout = layout_float
////	onUpdate = function: 000000000094a570
////	zzz_children
