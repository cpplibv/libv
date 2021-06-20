// Project: libv.ui, File: src/libv/ui/component/panel_status_log.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/chrono.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelStatusLine : public ComponentAPI<PanelLine, PanelStatusLine, class CorePanelStatusLine, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "status-line";
	static core_ptr create_core(std::string name);

public:
	using EntryID = uint64_t;

	void add(EntryID id, Component component, time_duration lifetime = {});
	void add(EntryID id, Component component, time_point time_of_death);
	void remove(EntryID id);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
