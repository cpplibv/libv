// Project: libv, File: app/star/game/scene/scene_root.hpp

#pragma once

// libv
#include <libv/ui/component/panel_anchor.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreSceneContainer;

class SceneContainer : public libv::ui::ComponentAPI<libv::ui::PanelAnchor, SceneContainer, CoreSceneContainer, libv::ui::EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "scene-ctnr";
	[[nodiscard]] static libv::ui::core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(libv::ui::core_ptr) noexcept;

public:
	void assign(libv::ui::Component scene);

	void identifier(std::string id) noexcept;
	[[nodiscard]] const std::string& identifier() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
