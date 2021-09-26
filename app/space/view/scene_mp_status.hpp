// Project: libv, File: app/space/view/scene_mp_bar.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
//#include <libv/ui/component/fwd.hpp>
//#include <libv/ui/component/button.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/panel_float.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
//#include <memory>
//#include <optional>
// pro
//#include <space/canvas.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SceneMPStatus {
	libv::Nexus& nexus;
	Lobby& lobby;
//	Player& player;

//private:
//	libv::ui::Label lbl_state{"mp-state"};
//	libv::ui::Button btn_host{"mp-host"};
//	libv::ui::Button btn_join{"mp-join"};

//	libv::ui::Component root;

public:
//	static libv::ui::Component create(libv::Nexus& nexus, Player& player);
//	~SceneMPStatus();
//	libv::ui::Component init(libv::ui::PanelLine& mp_bar);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
