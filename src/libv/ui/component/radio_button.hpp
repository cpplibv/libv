// Project: libv.ui, File: src/libv/ui/component/radio_button.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class RadioGroup;

class RadioButton : public ComponentHandler<class CoreRadioButton, EventHostSubmittable<RadioButton>> {
public:
	explicit RadioButton(std::string name);
	explicit RadioButton(GenerateName_t = {}, const std::string_view type = "radio-button");
	explicit RadioButton(core_ptr core) noexcept;

public:
	void group(RadioGroup group);
	[[nodiscard]] const RadioGroup& group() const noexcept;

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

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;
};

// -------------------------------------------------------------------------------------------------

class RadioGroup {
	Component owner;
	RadioButton selection;

	//BasicEventProxy<RadioGroup, EventSubmit> change;
	std::vector<RadioButton> group;

public:
	void add(RadioButton radioButton);
	void remove(const RadioButton& radioButton);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
