// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// std
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/component/basic_panel.hpp>
#include <libv/ui/layout/layout_line.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelLine : public BasicPanel<LayoutLine> {
public:
	explicit PanelLine(BaseComponent& parent) :
		BasicPanel<LayoutLine>(parent, GenerateName, "line") { }

	PanelLine(BaseComponent& parent, std::string name) :
		BasicPanel<LayoutLine>(parent, std::move(name)) { }

	PanelLine(BaseComponent& parent, GenerateName_t, const std::string_view type) :
		BasicPanel<LayoutLine>(parent, GenerateName, type) { }

	virtual ~PanelLine() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
