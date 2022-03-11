// Project: libv, File: app/star/game/scene/_scene_manager.hpp

#pragma once

// libv
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/panel_anchor.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

//star: scene navigation:
//	hierarchical handlers
//		+ hierarchical navigation handlers
//		+ in events soft requests to change scene
//		+ requests are handled in each level
//		- how to pass additional data into a new (sub) scene
//
//	template <typename T>
//	struct Scene {
//		SceneHandler* parent;
//
//		libv::ui::Component create_scene() = 0;
//	};
//
//	struct SceneHandler {
//		SceneHandler* parent = nullptr;
//
//		void switch_scene(???);
//	};

struct SceneHandler;

struct SceneBase {
	SceneHandler* parent;

	virtual libv::ui::Component create_scene() = 0;
};

struct SceneHandler : Component {
	SceneHandler* parent = nullptr;

	std::unique_ptr<SceneBase> activeScene;

	void switch_scene(???);
};

template <typename T>
struct Scene : SceneBase {
//	SceneHandler* parent;
//	libv::ui::Component create_scene() = 0;
};

struct MainMenu : Scene<MainMenu> {
	virtual libv::ui::Component create_scene() override {
		auto btn = libv::ui::Button();
		btn.event().submit.connect([this] {
			scene.switch("settings", settings);
		});
		return btn;
	}
};

struct Settings : Scene<Settings> {
	virtual libv::ui::Component create_scene() override {
		auto btn = libv::ui::Button();
		btn.event().submit.connect([scene](libv::ui::Button& source) {
			scene.switch("main_menu");
		});
		return btn;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace star
