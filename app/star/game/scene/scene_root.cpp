// Project: libv, File: app/star/game/scene/scene_root.cpp

// hpp
#include <star/game/scene/scene_root.hpp>
// libv
#include <libv/ui/component/panel_anchor_core.hpp>
// pro
#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class CoreSceneRoot : public libv::ui::CorePanelAnchor {
public:
	friend SceneRoot;
	[[nodiscard]] inline auto handler() { return SceneRoot{this}; }

private:
	libv::ui::Component primaryScene;

public:
	using CorePanelAnchor::CorePanelAnchor;
};

// =================================================================================================

libv::ui::core_ptr SceneRoot::create_core(std::string name) {
	return libv::ui::create_core_ptr<CoreSceneRoot>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

SceneRoot::SceneRoot(libv::ui::core_ptr ptr) noexcept :
	BaseAPI(std::move(ptr)) {
}

SceneRoot::SceneRoot(std::string name) :
	SceneRoot(create_core(std::move(name))) {

	event().global.connect<SwitchPrimaryScene>(*this, [s = &self()](const SwitchPrimaryScene& event) {
		s->handler().setPrimaryScene(event.scene);
	});
}

SceneRoot::SceneRoot(libv::ui::GenerateName_t) :
	SceneRoot(libv::ui::generate_component_name(component_type, nextID++)) {
}

void SceneRoot::setPrimaryScene(libv::ui::Component scene) {
	if (self().primaryScene)
		self().remove(self().primaryScene);
	self().primaryScene = std::move(scene);
	if (self().primaryScene)
//		self().add(self().primaryScene);
		self().add_front(self().primaryScene);
}

void SceneRoot::addOverlayScene(libv::ui::Component scene) {
	self().add(std::move(scene));
}

// =================================================================================================

//libv::ui::Component createSceneRoot(libv::Nexus& nexus, std::shared_ptr<ClientRoot>& settings) {
//libv::ui::Component createSceneRoot(libv::Nexus& nexus) {
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

// -------------------------------------------------------------------------------------------------

} // namespace star
