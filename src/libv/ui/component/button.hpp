// Project: libv.ui, File: src/libv/ui/component/button.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Button : public ComponentAPI<Component, Button, class CoreButton, EventHostSubmittable> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "button";
	static core_ptr create_core(std::string name);

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void font(Font2D_view value);
	[[nodiscard]] const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	[[nodiscard]] FontSize font_size() const noexcept;

	void font_color(Color value);
	[[nodiscard]] const Color& font_color() const noexcept;

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
