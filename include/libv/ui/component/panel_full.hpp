// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFull : public ComponenetHandler<class CorePanelFull, EventHostGeneral<PanelFull>> {
public:
	explicit PanelFull(std::string name);
	explicit PanelFull(GenerateName_t = {}, const std::string_view type = "full");
	explicit PanelFull(base_ptr core) noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
