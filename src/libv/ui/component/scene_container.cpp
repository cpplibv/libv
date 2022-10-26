// Project: libv.ui, File: src/libv/ui/component/scene_container.cpp

// hpp
#include <libv/ui/component/scene_container.hpp>
// libv
#include <libv/ui/component/panel_anchor_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreSceneContainer : CorePanelAnchor {
	using base_type = CorePanelAnchor;
	using base_type::base_type;

public:
	std::string identifier;
	libv::ui::Component primaryScene;
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
