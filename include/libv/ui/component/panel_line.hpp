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
	PanelLine() :
		BasicPanel<LayoutLine>(UnnamedTag{}, "line") { }

	PanelLine(std::string name) :
		BasicPanel<LayoutLine>(std::move(name)) { }

	PanelLine(UnnamedTag, const std::string_view type) :
		BasicPanel<LayoutLine>(UnnamedTag{}, type) { }

	virtual ~PanelLine() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
