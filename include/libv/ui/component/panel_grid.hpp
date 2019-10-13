// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// std
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/component/basic_panel.hpp>
#include <libv/ui/layout/layout_grid.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelGrid : public BasicPanel<LayoutGrid> {
public:
	explicit PanelGrid(BaseComponent& parent) :
		BasicPanel<LayoutGrid>(parent, GenerateName, "grid") { }

	PanelGrid(BaseComponent& parent, std::string name) :
		BasicPanel<LayoutGrid>(parent, std::move(name)) { }

	PanelGrid(BaseComponent& parent, GenerateName_t, const std::string_view type) :
		BasicPanel<LayoutGrid>(parent, GenerateName, type) { }

	virtual ~PanelGrid() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
