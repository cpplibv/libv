// Project: libv.ui, File: src/libv/ui/component/check_box.hpp

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/background.hpp>
//#include <libv/ui/property/align.hpp>
//#include <libv/ui/property/color.hpp>
//#include <libv/ui/property/font_2D.hpp>
//#include <libv/ui/property/font_size.hpp>
//#include <libv/ui/property/shader_image.hpp>
//#include <libv/ui/property/texture_2D.hpp>


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

class CheckBox : public ComponentAPI<Component, CheckBox, class CoreCheckBox, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "check-box";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

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
