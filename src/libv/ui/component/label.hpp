// Project: libv.ui, File: src/libv/ui/component/label.hpp

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Label : public ComponentAPI<Component, Label, class CoreLabel, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "label";
	static core_ptr create_core(std::string name);

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

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
