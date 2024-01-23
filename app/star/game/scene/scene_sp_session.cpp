// Project: libv, File: app/star/game/scene/scene_sp_session.cpp

#include <star/game/scene/scene_sp_session.hpp>

#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/scene_container.hpp>
#include <libv/ui/component/toggle_button.hpp>
#include <libv/ui/component_system/bean.hpp>
#include <libv/ui/component_system/switch_scene.hpp>

#include <cmath>

#include <star/game/control/requests.hpp>
#include <star/game/control/time_controller.hpp>
#include <star/game/scene/scenes.hpp>
#include <star/game/ui/controlled_scene.hpp>
#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

SceneSPSession::SceneSPSession(libv::Nexus& nexus) :
	timeController(std::make_shared<TimeController>(nexus)) {
}

void SceneSPSession::controls(ControllerContext& ctx) {
	ctx.attached<TimeController>(timeController.get());
}

void SceneSPSession::update(libv::time_point time, libv::time_duration_d delta) {
	timeController->update(time, delta);
	universe.update(timeController->timeSim, timeController->deltaSim);
}

libv::ui::Component SceneSPSession::createScene(libv::Nexus& nexus) {
	auto layers = libv::ui::PanelAnchor::n("layers");

	gsc = layers.add_na<libv::ui::SceneContainer>("sc-spsession", "game-scene");
	// gsc.assign(createSceneSurface(nexus, universe.surfaceA, timeController));
	// gsc.assign(createSceneSurface(nexus, universe.surfaceB, timeController));
	gsc.assign(createSceneStarmap(nexus, universe.starmap, timeController));

	layers.add(createInfoBar(nexus));
	layers.add(createMenu(nexus));

	return layers;
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::ui::Component SceneSPSession::createInfoBar(libv::Nexus& nexus) {
	auto panel = libv::ui::PanelLine::ns("info-bar", "game.info-bar.panel");
	const void* slot = panel.ptr();
	panel.event().detach.connect_system([nexus, slot] mutable {
		nexus.disconnect_slot_all(slot);
	});

	const auto timeEvent = [&]<typename Req, typename Cmd>(libv::ui::ToggleButton& btn, Req req, Cmd cmd) {
		btn.event().submit.connect([nexus, this, req](const libv::ui::EventSubmit& event) {
			timeController->request(req);
			event.reject_submit();
		});
		nexus.connect_channel_and_call<Cmd>(timeController.get(), slot, [btn, cmd](const Cmd& event) mutable {
			btn.select_silent(event == cmd);
		}, timeController->currentCommandTimeSpeed());
	};

	{
		auto leftLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		{
			auto lbl0 = leftLine.add_sa<libv::ui::Label>("game.info-bar.label", "Left");

			auto btnPause = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.pause", "[II]", "II");
			auto btnSpeed1 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[1]", "1");
			auto btnSpeed2 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[2]", "2");
			auto btnSpeed3 = leftLine.add_sa<libv::ui::ToggleButton>("game.info-bar.time.speed", "[4]", "4");
			timeEvent(btnPause, RequestTimeTogglePause{}, CommandTimeSpeed{0});
			timeEvent(btnSpeed1, RequestTimeSpeed{1}, CommandTimeSpeed{1});
			timeEvent(btnSpeed2, RequestTimeSpeed{2}, CommandTimeSpeed{2});
			timeEvent(btnSpeed3, RequestTimeSpeed{3}, CommandTimeSpeed{3});
		}
		auto gap = panel.add_s<libv::ui::Gap>("game.info-bar.gap");
		auto rightLine = panel.add_s<libv::ui::PanelLine>("game.info-bar.group");
		{
			auto lbl0 = rightLine.add_sa<libv::ui::Label>("game.info-bar.label", "Right");
		}
	}

	return panel;
}

libv::ui::Component SceneSPSession::createMenu(libv::Nexus& nexus) {
	auto line = libv::ui::PanelLine::ns("menu-line", "game.menu.line");

	auto btnStarmap = line.add_sa<libv::ui::Button>("game.menu.button", "Starmap");
	btnStarmap.event().submit.connect([this, nexus]() mutable {
		gsc.assign(createSceneStarmap(nexus, universe.starmap, timeController));
	});
	auto btnSurfaceA = line.add_sa<libv::ui::Button>("game.menu.button", "Surface A");
	btnSurfaceA.event().submit.connect([this, nexus]() mutable {
		gsc.assign(createSceneSurface(nexus, universe.surfaceA, timeController));
	});
	auto btnSurfaceB = line.add_sa<libv::ui::Button>("game.menu.button", "Surface B");
	btnSurfaceB.event().submit.connect([this, nexus]() mutable {
		gsc.assign(createSceneSurface(nexus, universe.surfaceB, timeController));
	});
	auto btnExit = line.add_sa<libv::ui::Button>("game.menu.button", "Exit to Main Menu");
	btnExit.event().submit.connect([nexus](libv::ui::Button& source) mutable {
		libv::ui::switchParentScene("main", source, createSceneMainMenu(nexus));
	});

	return line;
}

// =================================================================================================

libv::ui::Component createSceneSPSession(libv::Nexus& nexus) {
	auto& controls = libv::ui::requireBean<libv::ctrl::Controls>(nexus, "Starmap", "Controls");
	return star::createControlledScene<SceneSPSession>(controls, nexus)(nexus);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
