// Project: libv.re, File: sandbox/re/scene_alpha.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus.hpp>

// #include <iostream>
// #include <libv/arg/arg.hpp>
// #include <libv/ctrl/controls.hpp>
// #include <libv/frame/frame.hpp>
// #include <libv/frame/icon_set.hpp>
// #include <libv/log/log.hpp>
// #include <libv/math/exp_moving_avg.hpp>
// #include <libv/res/resource_path.hpp>
// #include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle_system.hpp>
// #include <libv/rev/engine/canvas.hpp>
// #include <libv/rev/engine/engine.hpp>
// #include <libv/rev/engine/render_pass.hpp>
// #include <libv/rev/material/material_editor_grid.hpp>
// #include <libv/rev/material_adv/material_editor_grid.hpp>
// #include <libv/rev/newapi/scene.hpp>
// #include <libv/rev/post_processing.hpp>
// #include <libv/rev/render_target.hpp>
// #include <libv/rev/resource/resource_manager.hpp>
// #include <libv/rev/resource/shader.hpp>
// #include <libv/rev/settings.hpp>
// #include <libv/sun/camera.hpp>
// #include <libv/sun/ui/overlay_shader_error.hpp>
// #include <libv/ui/component/canvas.hpp>
// #include <libv/ui/component/label.hpp>
// #include <libv/ui/component/panel_anchor.hpp>
// #include <libv/ui/component/scene_container.hpp>
// #include <libv/ui/component_system/canvas.hpp>
// #include <libv/ui/component_system/create_scene.hpp>
// #include <libv/ui/context/context_state.hpp>
// #include <libv/ui/context/context_ui.hpp>
// #include <libv/ui/settings.hpp>
// #include <libv/ui/ui.hpp>
// #include <libv/utility/nexus.hpp>
// #include <libv/utility/timer.hpp>
// #include <wish/resource_mapping.hpp>


// -------------------------------------------------------------------------------------------------

struct SandboxState {
	// Simulation
	bool paused = false;
	bool unpauseNextFrame = false;
	bool slowMode = false;
	int slowModeCounter = 0;
	libv::time_point timeRealStart = libv::clock::now();
	libv::time_point timeRealFrame = timeRealStart;
	libv::time_duration_d timeSim{0};

	// Visuals
	libv::re::Scene_ptr scene;
	libv::re::Camera_ptr camera;
	libv::re::Canvas_ptr canvas;

	libv::re::MaterialTest0_ptr matTest0;
	libv::re::Object_ptr sphereMoving0;
	libv::re::Object_ptr sphereMoving1;
	libv::re::Light_ptr movingLight0;

	libv::re::Node_ptr movingModel;

	// State
	bool rotate = false;
	int state = 0;

	// EditorGrid editorGrid{rev};

	// libv::re::ParticleEffect fountain;
	// std::vector<libv::re::ParticleEffect> fountains;
	// libv::re::ParticleEffect particleFountain{scene, particleLibrary.fountain};

	// ParticleEffect cameraDust;
	//
	// struct Ship {
	// 	ShipType type;
	// 	ParticleEffect* engineBloom = nullptr;
	// };

	// std::vector<Ship> ships;

	libv::Nexus nexus;

public:
	explicit SandboxState(libv::Nexus& nexus);
	~SandboxState();
	void async_update(libv::time_point frameTime, libv::time_duration deltaTime);
	void create(libv::GL& gl);
	void destroy(libv::GL& gl);
	void render(libv::GL& gl, libv::vec2f canvasPosition, libv::vec2f canvasSize);
	// void render(libv::re::Engine& rev);
	// void render(libv::re::RenderPass& rev);
};

// -------------------------------------------------------------------------------------------------
