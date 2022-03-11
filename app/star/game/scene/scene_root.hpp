// Project: libv, File: app/star/game/scene/scene_root.hpp

#pragma once

// libv
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/panel_anchor.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct SwitchPrimaryScene {
	libv::ui::Component scene;
};

// -------------------------------------------------------------------------------------------------

class CoreSceneRoot;

// NOTE: Root scene will take care of the overlays, the version stamp, and it will provide the root component for every scene
class SceneRoot : public libv::ui::ComponentAPI<libv::ui::PanelAnchor, SceneRoot, CoreSceneRoot, libv::ui::EventHostGeneral> {
public:
	static constexpr std::string_view component_type = "root-scene";
	static libv::ui::core_ptr create_core(std::string name);

public:
	explicit inline SceneRoot(libv::ui::core_ptr ptr) noexcept;
	explicit SceneRoot(std::string name);
	explicit SceneRoot(libv::ui::GenerateName_t = {});

public:
	void setPrimaryScene(libv::ui::Component scene);
	void addOverlayScene(libv::ui::Component scene);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
