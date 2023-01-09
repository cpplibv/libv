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
	Component currentScene;
};

// =================================================================================================

core_ptr SceneContainer::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

core_ptr SceneContainer::create_core(std::string name, std::string identifier) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->identifier = std::move(identifier);
	return p;
}

core_ptr SceneContainer::create_core(std::string name, std::string identifier, Component scene) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->identifier = std::move(identifier);
	p->currentScene = std::move(scene);
	if (p->currentScene)
		p->add_front(p->currentScene);
	return p;
}

bool SceneContainer::castable(core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void SceneContainer::assign(Component scene) {
	if (self().currentScene)
		self().remove(self().currentScene);
	self().currentScene = std::move(scene);
	if (self().currentScene)
//		self().add(self().currentScene);
		self().add_front(self().currentScene);
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
