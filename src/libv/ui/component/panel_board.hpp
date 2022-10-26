// Project: libv.ui, File: src/libv/ui/component/panel_board.hpp

#pragma once

// pro
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelBoard : public ComponentAPI<Component, PanelBoard, class CorePanelBoard, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "board";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

public:
	void add(Component component, libv::vec2f position, libv::vec2f size);
	void remove(Component& component);
	void remove(std::string_view component_name);
	void clear();

//public:
//	void board_position(libv::vec2f value);
//	[[nodiscard]] libv::vec2f board_position() const noexcept;
//
//	void board_size(libv::vec2f value);
//	[[nodiscard]] libv::vec2f board_size() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
