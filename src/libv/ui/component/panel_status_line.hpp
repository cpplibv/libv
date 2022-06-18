// Project: libv.ui, File: src/libv/ui/component/panel_status_line.hpp

#pragma once

// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventPanelStatusExpired : BaseEvent {
	uint64_t id;
};

template <typename ComponentT>
struct EventHostPanelStatus : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventPanelStatusExpired> expire{this->owner};
};

// -------------------------------------------------------------------------------------------------

class PanelStatusLine : public ComponentAPI<PanelLine, PanelStatusLine, class CorePanelStatusLine, EventHostPanelStatus> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "status-line";
	static core_ptr create_core(std::string name);

public:
	using EntryID = uint64_t;

	void add(EntryID id, Component component, time_duration lifetime);
	/// Zero time point means it is a permanent entry
	void add(EntryID id, Component component, time_point time_of_death = {});
	void remove(EntryID id);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
