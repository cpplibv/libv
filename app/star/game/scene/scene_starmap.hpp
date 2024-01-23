// Project: libv, File: app/star/game/scene/scene_starmap.hpp

#pragma once

#include <libv/math/ray.hpp>
#include <libv/re/fwd.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/utility/screen_picker.hpp>

#include <vector>

#include <star/game/fwd.hpp>
#include <star/game/control/starmap_controller.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class CanvasStarmap : public libv::ui::CanvasBaseRE {
public:
	// Render
	libv::re::Scene_ptr scene;
	libv::re::Camera_ptr camera;
	libv::screen_picker<float> screenPicker;

	// State
	Starmap_sp starmap;
	TimeController_sp timeController;
	bool rotate = false;

	// Canvas interaction
	libv::ray3f mouseRay;

	// RE Objects
	libv::re::MeshSphere_ptr ballMesh;
	libv::re::MaterialSolid_ptr ballMaterial;

	// RE Nodes
	libv::re::Node_ptr editorGrid;
	libv::re::Skybox_ptr skybox;
	std::vector<libv::re::Object_ptr> balls;
	libv::re::Object_ptr movingBall;
	libv::re::Object_ptr pickingBall;

public:
	explicit CanvasStarmap(libv::Nexus& nexus, Starmap_sp starmap, TimeController_sp timeController);
	~CanvasStarmap();

private:
	virtual void render(libv::GL& gl) override;
};

// -------------------------------------------------------------------------------------------------

class SceneStarmap {
	Starmap_sp starmap;

	TimeController_sp timeController;
	StarmapController starmapController;

	libv::ui::CanvasAdaptorT<CanvasStarmap> canvas{nullptr};

public:
	explicit SceneStarmap(libv::Nexus& nexus, Starmap_sp starmap, TimeController_sp timeController);

public:
	void controls(ControllerContext& ctx);
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
