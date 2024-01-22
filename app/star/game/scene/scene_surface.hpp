// Project: libv, File: app/star/game/scene/scene_surface.cpp

#pragma once

#include <libv/math/ray.hpp>
#include <libv/re/fwd.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/utility/screen_picker.hpp>

#include <vector>

#include <star/game/fwd.hpp>
#include <star/game/control/surface_controller.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class CanvasSurface : public libv::ui::CanvasBaseRE {
public:
	// Render
	libv::re::Scene_ptr scene;
	libv::re::Camera_ptr camera;
	libv::screen_picker<float> screenPicker;

	// State
	Surface_sp surface;
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
	explicit CanvasSurface(libv::Nexus& nexus, Surface_sp surface_, TimeController_sp timeController_);
	~CanvasSurface();

private:
	virtual void render(libv::GL& gl) override;
};

// -------------------------------------------------------------------------------------------------

class SceneSurface {
	Surface_sp surface;

	TimeController_sp timeController;
	SurfaceController surfaceController;

	libv::ui::CanvasAdaptorT<CanvasSurface> canvas{nullptr};

public:
	explicit SceneSurface(libv::Nexus& nexus, Surface_sp surface_, TimeController_sp timeController_);

public:
	void controls(ControllerContext& ctx);
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
