// File: label.hpp - Created on 2017.10.22. 07:12 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Label : public ComponenetHandler<class CoreLabel, EventHostGeneral<Label>> {
public:
	explicit Label(std::string name);
	explicit Label(GenerateName_t = {}, const std::string_view type = "label");
	explicit Label(core_ptr core) noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void font(Font2D_view value);
	[[nodiscard]] const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	[[nodiscard]] FontSize font_size() const noexcept;

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
