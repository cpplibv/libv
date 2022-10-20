// Project: libv, File: app/star/game/scene/scene_settings.hpp

#pragma once

// libv
#include <libv/ui/attach_state.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/scene_container.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <exception>
#include <memory>
#include <string>
#include <string_view>


namespace star {

// -------------------------------------------------------------------------------------------------

template <typename T>
inline T& requireBean(libv::Nexus& nexus, const std::string& sceneName, const std::string& beanName) {
	const auto bean = nexus.object_view_get<T>();
	if (!bean)
		throw std::runtime_error(sceneName + " scene creation failed: Missing bean: " + beanName);

	return *bean;
}

inline void switchParentScene(std::string_view sceneContainerID, const libv::ui::Component& component, libv::ui::Component scene) {
	auto it = component.parent();
	while (it = it.parent()) {
		auto sc = it.cast<libv::ui::SceneContainer>();
		if (sc && sc.identifier() == sceneContainerID)
			sc.assign(scene);
	}
}

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AssembleSceneProxy {
	std::unique_ptr<T> t;

	template <typename... Args>
	libv::ui::Component operator()(Args&&... args) {
		auto c = t->create_scene(std::forward<Args>(args)...);
		libv::ui::attach_state<std::unique_ptr<T>>(c)(std::move(t));
		return c;
	}
};

/// Requires a libv::ui::Component create_scene(...) member function
/// @usage: assembleScene<SceneType>(SceneCtorArgs...)(CreateSceneArgs...);
template <typename T, typename... Args>
[[nodiscard]] AssembleSceneProxy<T> assembleScene(Args&&... args) {
	return AssembleSceneProxy<T>(std::make_unique<T>(std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

//libv::ui::Component createMainLayoutScene(...) {
//libv::ui::Component createMainOverlay(...) {
//	libv::ui::PanelAnchor layers{"layers"};
//
////	{
////		main scene...
////
////		layers.add(std::move(__main_scene__));
////
////	} {
////		libv::ui::Label lbl("version_lbl");
////		lbl.style("main-menu.version-lbl");
////		lbl.text("Version: 0.0.0");
////		layers.add(std::move(lbl));
////	}
//
//	return layers;
//}
//
//// -------------------------------------------------------------------------------------------------
//
//struct SceneHandler;
//
//struct SceneBase {
//	SceneHandler* parent;
//
//	virtual libv::ui::Component create_scene() = 0;
//};
//
//struct SceneHandler : Component {
//	SceneHandler* parent = nullptr;
//
//	std::unique_ptr<SceneBase> activeScene;
//
//	void switch_scene(?);
//};
//
//template <typename T>
//struct Scene : SceneBase {
//};
//
//struct MainMenu : Scene<MainMenu> {
//	virtual libv::ui::Component create_scene() override {
//		auto btn = libv::ui::Button();
//		btn.event().submit.connect([this] {
//			scene.switch("settings", settings);
//		});
//		return btn;
//	}
//};
//
//struct Settings : Scene<Settings> {
//	virtual libv::ui::Component create_scene() override {
//		auto btn = libv::ui::Button();
//		btn.event().submit.connect([scene](libv::ui::Button& source) {
//			scene.switch("main_menu");
//		});
//		return btn;
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace star
