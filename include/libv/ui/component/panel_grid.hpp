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
	PanelGrid() :
		BasicPanel<LayoutGrid>(UnnamedTag, "grid") { }

	explicit PanelGrid(std::string name) :
		BasicPanel<LayoutGrid>(std::move(name)) { }

	PanelGrid(UnnamedTag_t, const std::string_view type) :
		BasicPanel<LayoutGrid>(UnnamedTag, type) { }

	virtual ~PanelGrid() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
