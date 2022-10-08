// Project: libv, File: app/star/game/scene/scene_root.cpp

// hpp
#include <libv/ui/component/scene_container.hpp>
// libv
#include <libv/ui/component/panel_anchor_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreSceneContainer : public libv::ui::CorePanelAnchor {
public:
	friend SceneContainer;
	[[nodiscard]] inline auto handler() { return SceneContainer{this}; }

private:
	std::string identifier;
	libv::ui::Component primaryScene;

public:
	using CorePanelAnchor::CorePanelAnchor;
};

// =================================================================================================

libv::ui::core_ptr SceneContainer::create_core(std::string name) {
	return libv::ui::create_core_ptr<CoreType>(std::move(name));
}

bool SceneContainer::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void SceneContainer::assign(libv::ui::Component scene) {
	if (self().primaryScene)
		self().remove(self().primaryScene);
	self().primaryScene = std::move(scene);
	if (self().primaryScene)
//		self().add(self().primaryScene);
		self().add_front(self().primaryScene);
}

void SceneContainer::identifier(std::string id) noexcept {
	self().identifier = std::move(id);
}

const std::string& SceneContainer::identifier() const noexcept {
	return self().identifier;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
