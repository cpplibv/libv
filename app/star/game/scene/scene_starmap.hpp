// Project: libv, File: app/star/game/scene/scene_surface.cpp

#pragma once

#include <libv/math/ray.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/material/skybox_type.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/utility/screen_picker.hpp>

#include <string>
#include <vector>

#include <star/game/fwd.hpp>
#include <star/game/control/surface_controller.hpp>
#include <star/game/control/time_controller.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct Starmap {
	struct Sky {
		std::string texture;
		libv::re::SkyboxType type = libv::re::SkyboxType::cubemapZXY;
		float intensity = 1.f;
		float rotateZ = 0.f;
		float horizonOffset = 0.f;
	};

	struct Ball {
		libv::vec3f position;
		float size = 0.f;
	};

	Sky sky;
	Ball movingBall;
	Ball pickingBall;
	std::vector<Ball> balls;
	// Terrain, Buildings, Enviroment descriptors and/or simulation data

public:
	Surface();
	void update(double timeSim);
};

// -------------------------------------------------------------------------------------------------

class CanvasSurface : public libv::ui::CanvasBaseRE {
public:
	// Render
	libv::re::Scene_ptr scene;
	libv::re::Camera_ptr camera;
	libv::screen_picker<float> screenPicker;

	// State
	Surface& surface;
	TimeController& timeController;
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
	explicit CanvasSurface(libv::Nexus& nexus, Surface& surface, TimeController& timeController);
	~CanvasSurface();

private:
	virtual void render(libv::GL& gl) override;
};

// -------------------------------------------------------------------------------------------------

class SceneSurface {
	Surface surface;

	TimeController timeController;
	SurfaceController surfaceController;

	libv::ui::CanvasAdaptorT<CanvasSurface> canvas{nullptr};

public:
	explicit SceneSurface(libv::Nexus& nexus);

public:
	void controls(ControllerContext& ctx);
	void update(libv::time_point time, libv::time_duration_d delta);
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus);

private:
	[[nodiscard]] libv::ui::Component createInfoBar(libv::Nexus& nexus);
	[[nodiscard]] libv::ui::PanelLine createMenu(libv::Nexus& nexus);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
