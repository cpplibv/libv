// Project: libv, File: app/space/view/scene_mp_status.cpp

// hpp
#include <space/view/scene_mp_status.hpp>
// libv
#include <libv/ui/attach_state.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <space/network/lobby.hpp>
//#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

libv::ui::Component SceneMPStatus::create(libv::Nexus& nexus) {
	libv::ui::PanelLine bar("mp-status");
	bar.style("space.hud-bar.lobby.panel");
	libv::ui::attach_state<SceneMPStatus>(bar)(bar, nexus);
	return bar;
}

SceneMPStatus::SceneMPStatus(libv::ui::PanelLine bar, libv::Nexus& nexus) :
		bar(std::move(bar)),
		nexus(nexus) {
	{
		libv::ui::Label title;
		title.style("space.hud-bar.lobby.title");
		title.text("Lobby:");
		this->bar.add(std::move(title));
	}

	register_nexus();
}

SceneMPStatus::~SceneMPStatus() {
	unregister_nexus();
}

void SceneMPStatus::register_nexus() {
	nexus.connect_global<Lobby::OnClientJoin>(this, [this](const Lobby::OnClientJoin& event) {
		libv::ui::Label label;
		label.style("space.hud-bar.lobby.name");
		label.text(event.client->name);
		entries.emplace(event.client->userID, label);
		bar.add(std::move(label));
	});

	nexus.connect_global<Lobby::OnClientLeave>(this, [this](const Lobby::OnClientLeave& event) {
		auto it = entries.find(event.client->userID);
		if (it == entries.end()) {
			// <<< P5: Log error
			assert(false && "Internal consistency error");
			return;
		}

		it->second.markRemove();
		entries.erase(it);
	});

//	nexus.connect<Lobby::OnClose>(this, [this](const Lobby::OnClose&) {
//		// Noop?
//	});
}

void SceneMPStatus::unregister_nexus() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
