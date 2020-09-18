// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/orientation.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelLine : public ComponentHandler<class CorePanelLine, EventHostGeneral<PanelLine>> {
public:
	explicit PanelLine(std::string name);
	explicit PanelLine(GenerateName_t = {}, const std::string_view type = "line");
	explicit PanelLine(core_ptr core) noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void orientation(Orientation value);
	[[nodiscard]] Orientation orientation() const noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
