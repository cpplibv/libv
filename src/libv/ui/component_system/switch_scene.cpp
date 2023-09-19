// Project: libv.ui, File: src/libv/ui/component_system/switch_scene.cpp

// hpp
#include <libv/ui/component_system/switch_scene.hpp>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/scene_container.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void switchParentScene(std::string_view sceneContainerID, const libv::ui::Component& component, libv::ui::Component newScene) {
	auto it = component.parent();
	while ((it = it.parent())) {
		auto sc = it.cast<libv::ui::SceneContainer>();
		if (sc && sc.identifier() == sceneContainerID)
			sc.assign(newScene);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
