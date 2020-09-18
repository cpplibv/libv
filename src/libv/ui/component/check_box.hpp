// Project: libv.ui, File: src/libv/ui/component/check_box.hpp, Author: Császár Mátyás [Vader]

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
// =================================================================================================

enum class CheckBoxMode {
	on_off,
	on_off_mixed,
};

enum class CheckBoxValue {
	on,
	off,
	mixed,
};

// =================================================================================================

class CheckBox : public ComponentHandler<class CoreCheckBox, EventHostSubmitable<CheckBox>> {
public:
	explicit CheckBox(std::string name);
	explicit CheckBox(GenerateName_t = {}, const std::string_view type = "check-box");
	explicit CheckBox(core_ptr core) noexcept;

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;

public:
	void mode(CheckBoxMode value);
	[[nodiscard]] CheckBoxMode mode() const noexcept;

	void value(CheckBoxValue value);
	[[nodiscard]] CheckBoxValue value() const noexcept;

	inline void set_to_on() {
		value(CheckBoxValue::on);
	}

	inline void set_to_off() {
		value(CheckBoxValue::off);
	}

	inline void set_to_mixed() {
		value(CheckBoxValue::mixed);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
