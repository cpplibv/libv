// Project: libv, File: app/space/view/scene_menu_bar.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
//#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/panel_anchor.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
//#include <memory>
//#include <optional>
// pro
#include <space/view/canvas.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct SceneMenuBar {
	libv::Nexus2& nexus;
	User& user;

	//	enum class State {
	//		idle,
	//		client,
	//		server,
	//	};
	//	State active_state = State::idle;
	bool client_active = false;
	bool server_active = false;

//private:
	libv::ui::Label lbl_state{"mp-state"};
	libv::ui::Button btn_host{"mp-host"};
	libv::ui::Button btn_join{"mp-join"};

//	libv::ui::Component root;

public:
	static libv::ui::Component create(libv::Nexus2& nexus, User& user);
	~SceneMenuBar();
	libv::ui::Component init(libv::ui::PanelLine& mp_bar);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
