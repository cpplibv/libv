// Project: libv.ui, File: src/libv/ui/property_system/property_visit.hpp

#pragma once

// pro
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/margin.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/orientation2.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/scroll_mode.hpp>
#include <libv/ui/property/shader_font.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/shader_quad.hpp>
#include <libv/ui/property/size.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/spacing.hpp>
#include <libv/ui/property/squish.hpp>
#include <libv/ui/property/texture_2D.hpp>
#include <libv/ui/property_system/property.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename F>
void visitProperty(const PropertyDynamic& property, F&& func) {
	if (const auto* p = std::any_cast<bool>(&property)) return func(*p);
	if (const auto* p = std::any_cast<float>(&property)) return func(*p);
	// if (const auto* p = std::any_cast<Style_view>(&property)) return func(*p);

	if (const auto* p = std::any_cast<AlignHorizontal>(&property)) return func(*p);
	if (const auto* p = std::any_cast<AlignVertical>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Anchor>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Background>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Color>(&property)) return func(*p);
	if (const auto* p = std::any_cast<ColumnCount>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Font2D_view>(&property)) return func(*p);
	if (const auto* p = std::any_cast<FontSize>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Margin>(&property)) return func(*p); // Should be a duplicate type
	if (const auto* p = std::any_cast<Orientation>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Orientation2>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Padding>(&property)) return func(*p); // Should be a duplicate type
	if (const auto* p = std::any_cast<ScrollMode>(&property)) return func(*p);
	if (const auto* p = std::any_cast<ShaderFont_view>(&property)) return func(*p);
	if (const auto* p = std::any_cast<ShaderImage_view>(&property)) return func(*p);
	if (const auto* p = std::any_cast<ShaderQuad_view>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Size>(&property)) return func(*p);
	if (const auto* p = std::any_cast<SnapToEdge>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Spacing>(&property)) return func(*p); // Should be a duplicate type
	if (const auto* p = std::any_cast<Spacing2>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Squish>(&property)) return func(*p);
	if (const auto* p = std::any_cast<Texture2D_view>(&property)) return func(*p);

	if (const auto* p = std::any_cast<std::string>(&property)) return func(*p); // For debug only

	// Not supported property type: Pass in as std::any
	return func(property);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
