// Project: libv.ui, File: src/libv/ui/component/button.hpp

#pragma once

// pro
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ToggleButton : public ComponentAPI<Button, ToggleButton, class CoreToggleButton, EventHostSubmittable> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "toggle";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void text_on(std::string value);
	[[nodiscard]] const std::string& text_on() const noexcept;

	void text_off(std::string value);
	[[nodiscard]] const std::string& text_off() const noexcept;

public:
	void select(bool value) noexcept;
	[[nodiscard]] bool select() const noexcept;

	inline bool toggle() noexcept {
		bool value = !select();
		select(value);
		return value;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
