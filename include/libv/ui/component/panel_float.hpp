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
	explicit PanelFloat(BaseComponent& parent) :
		BasicPanel<LayoutFloat>(parent, GenerateName, "float") { }

	PanelFloat(BaseComponent& parent, std::string name) :
		BasicPanel<LayoutFloat>(parent, std::move(name)) { }

	PanelFloat(BaseComponent& parent, GenerateName_t, const std::string_view type) :
		BasicPanel<LayoutFloat>(parent, GenerateName, type) { }

	virtual ~PanelFloat() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
