// Project: libv.ui, File: src/libv/ui/component/scene_container.hpp

#pragma once

// libv
#include <libv/ui/component/panel_anchor.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreSceneContainer;

class SceneContainer : public ComponentAPI<PanelAnchor, SceneContainer, CoreSceneContainer, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "scene-ctnr";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static core_ptr create_core(std::string name, std::string identifier);
	[[nodiscard]] static core_ptr create_core(std::string name, std::string identifier, Component scene);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void assign(Component scene);

	void identifier(std::string id) noexcept;
	[[nodiscard]] const std::string& identifier() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
