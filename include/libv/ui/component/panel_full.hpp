// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// std
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/component/basic_panel.hpp>
#include <libv/ui/layout/layout_full.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFull : public BasicPanel<LayoutFull> {
public:
	PanelFull() :
		BasicPanel<LayoutFull>(UnnamedTag{}, "full") { }

	PanelFull(std::string name) :
		BasicPanel<LayoutFull>(std::move(name)) { }

	PanelFull(UnnamedTag, const std::string_view type) :
		BasicPanel<LayoutFull>(UnnamedTag{}, type) { }

	virtual ~PanelFull() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
