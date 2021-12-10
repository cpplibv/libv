// Project: libv, File: app/space/view/scene_mp_status.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <map>
// pro
#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class SceneMPStatus {
private:
	libv::ui::PanelLine bar;
	std::map<UserID, libv::ui::Component> entries;

	libv::Nexus& nexus;

public:
	static libv::ui::Component create(libv::Nexus& nexus);
	explicit SceneMPStatus(libv::ui::PanelLine bar, libv::Nexus& nexus);
	~SceneMPStatus();

private:
	void register_nexus();
	void unregister_nexus();
};

// -------------------------------------------------------------------------------------------------

} // namespace space
