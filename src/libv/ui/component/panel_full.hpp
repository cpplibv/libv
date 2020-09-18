// Project: libv.ui, File: src/libv/ui/component/panel_full.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFull : public ComponentHandler<class CorePanelFull, EventHostGeneral<PanelFull>> {
public:
	explicit PanelFull(std::string name);
	explicit PanelFull(GenerateName_t = {}, const std::string_view type = "full");
	explicit PanelFull(core_ptr core) noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
