// Project: libv.ui, File: src/libv/ui/component/panel_float.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/squish.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFloat : public ComponentAPI<Component, PanelFloat, class CorePanelFloat, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "float";
	static core_ptr create_core(std::string name);

public:
	void snap_to_edge(SnapToEdge value);
	[[nodiscard]] SnapToEdge snap_to_edge() const noexcept;

	void squish(Squish value);
	[[nodiscard]] Squish squish() const noexcept;

public:
	void add(Component component);
	void add_front(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
