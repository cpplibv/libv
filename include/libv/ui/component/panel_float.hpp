// File: layout_line.hpp Author: Vader Created on 2017. augusztus 26., 23:33

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/event_host.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/squish.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFloat : public ComponenetHandler<class CorePanelFloat, EventHostGeneral<PanelFloat>> {
public:
	explicit PanelFloat(std::string name);
	explicit PanelFloat(GenerateName_t = {}, const std::string_view type = "float");
	explicit PanelFloat(base_ptr core) noexcept;

public:
	void snap_to_edge(SnapToEdge value);
	[[nodiscard]] SnapToEdge snap_to_edge() const noexcept;

	void squish(Squish value);
	[[nodiscard]] Squish squish() const noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
