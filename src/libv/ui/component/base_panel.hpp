// Project: libv.ui, File: src/libv/ui/component/base_panel.hpp

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BasePanel : public ComponentAPI<Component, BasePanel, class CoreBasePanel, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "base-panel";
	static core_ptr create_core(std::string name);

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

public:
	void add(Component component);
	void add_front(Component component);
	void remove(Component& component);
	void remove(std::string_view component_name);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
