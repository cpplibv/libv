// Project: libv.ui, File: src/libv/ui/component/_idea_enum_button.hpp

#pragma once

// pro
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/detail/component_api.hpp>
//#include <libv/ui/property/align.hpp>
//#include <libv/ui/property/color.hpp>
//#include <libv/ui/property/font_2D.hpp>
//#include <libv/ui/property/font_size.hpp>
//#include <libv/ui/property/shader_image.hpp>
//#include <libv/ui/property/texture_2D.hpp>


#include <libv/ui/component/button_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//template <typename Enum>
//struct InnerEnumButton {
//	Enum value;
//
//public:
//	void init(libv::ui::Button& button) {
//
//	}
//
//private:
//	using libv::ui::Button::text;
//
//public:
//	void value(std::string value);
//	[[nodiscard]] const std::string& value() const noexcept;
//};
//
//using EnumButton = libv::ui::ComponentStorage<libv::ui::Button, InnerEnumButton>

class BaseEnumButton : public ComponentAPI<Button, BaseEnumButton, class CoreEnumButton, EventHostSubmittable> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "button";
	static core_ptr create_core(std::string name);

private:
	using ComponentAPI::text;

public:
	void value(uint64_t value);
	[[nodiscard]] uint64_t value() const noexcept;
};

// -------------------------------------------------------------------------------------------------

template <typename Enum>
class EnumButton : BaseEnumButton {
public:
	using BaseEnumButton::BaseEnumButton;

private:
	using BaseEnumButton::value;

public:
	void value(Enum value);
	[[nodiscard]] Enum value() const noexcept;

	void next_value() noexcept;
	void prev_value() noexcept;
};

// -------------------------------------------------------------------------------------------------

template <typename Enum>
class CoreEnumButton : CoreButton {
	Enum value;

};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
