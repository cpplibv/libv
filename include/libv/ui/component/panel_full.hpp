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
	explicit PanelFull(ContextUI& context) : // Root only constructor
		BasicPanel<LayoutFull>(context) { }

	explicit PanelFull(BaseComponent& parent) :
		BasicPanel<LayoutFull>(parent, GenerateName, "full") { }

	PanelFull(BaseComponent& parent, std::string name) :
		BasicPanel<LayoutFull>(parent, std::move(name)) { }

	PanelFull(BaseComponent& parent, GenerateName_t, const std::string_view type) :
		BasicPanel<LayoutFull>(parent, GenerateName, type) { }

	virtual ~PanelFull() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
