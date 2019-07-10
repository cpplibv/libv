// File: layout_line.hpp Author: Vader Created on 2017. augusztus 26., 23:33

#pragma once

// std
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/component/basic_panel.hpp>
#include <libv/ui/layout/layout_float.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFloat : public BasicPanel<LayoutFloat> {
public:
	PanelFloat() :
		BasicPanel<LayoutFloat>(UnnamedTag{}, "float") { }

	PanelFloat(std::string name) :
		BasicPanel<LayoutFloat>(std::move(name)) { }

	PanelFloat(UnnamedTag, const std::string_view type) :
		BasicPanel<LayoutFloat>(UnnamedTag{}, type) { }

	virtual ~PanelFloat() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
