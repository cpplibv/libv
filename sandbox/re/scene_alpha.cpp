// Project: libv.re, File: sandbox/re/scene_alpha.cpp

#include "scene_alpha.hpp"
#include "log.hpp"

#include <libv/ctrl/controls.hpp>
#include <libv/math/constants.hpp>
#include <libv/math/quat.hpp>
#include <libv/re/core/engine.hpp>
#include <libv/re/core/scene.hpp>
#include <libv/re/material/material_solid.hpp>
#include <libv/re/material/material_solid_model.hpp>
#include <libv/re/material/material_test0.hpp>
#include <libv/re/mesh/mesh_cube.hpp>
#include <libv/re/mesh/mesh_model_static.hpp>
#include <libv/re/mesh/mesh_plane.hpp>
#include <libv/re/mesh/mesh_sphere.hpp>
#include <libv/re/node/camera.hpp>
#include <libv/re/node/editor_grid.hpp>
#include <libv/re/node/lens_flare.hpp>
#include <libv/re/node/light/light.hpp>
#include <libv/re/node/model_static.hpp>
#include <libv/re/node/object.hpp>
#include <libv/re/node/outline.hpp>
#include <libv/re/node/skybox.hpp>
#include <libv/re/r.hpp>
#include <libv/re/resource/material_loader.hpp>
#include <libv/re/resource/material_scanner.hpp>
#include <libv/re/resource/model_loader.hpp>
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_loader.hpp>


// Testing only:
#include <libv/res/resource_path.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/vm4/load.hpp>
#include <libv/vm4/model.hpp>


// #include <libv/arg/arg.hpp>
// #include <libv/ctrl/controls.hpp>
// #include <libv/frame/frame.hpp>
// #include <libv/frame/icon_set.hpp>
// #include <libv/log/log.hpp>
// #include <libv/math/exp_moving_avg.hpp>
// #include <libv/res/resource_path.hpp>
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
// #include <libv/utility/chrono.hpp>
// #include <libv/utility/nexus.hpp>
// #include <libv/utility/timer.hpp>
// #include <iostream>
// #include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle_system.hpp>
// #include <libv/rev/post_processing.hpp>
// #include <libv/rev/render_target.hpp>
// #include <libv/rev/resource/resource_manager.hpp>
// #include <libv/rev/resource/shader.hpp>
// #include <libv/rev/effect/particle.hpp>
// #include <libv/rev/engine/canvas.hpp>
// #include <libv/rev/engine/engine.hpp>
// #include <libv/rev/engine/render_pass.hpp>
// #include <libv/rev/material/material_editor_grid.hpp>
// #include <libv/rev/material_adv/material_editor_grid.hpp>
// #include <libv/rev/settings.hpp>
// #include <libv/sun/ui/overlay_shader_error.hpp>
// #include <libv/sun/camera.hpp>
//
// #include <wish/resource_mapping.hpp>


// -------------------------------------------------------------------------------------------------

SandboxState::SandboxState(libv::Nexus& nexus) :
		scene(libv::re::Scene::create("Alpha")),
		camera(libv::re::Camera::create()),
		nexus(nexus) {

	camera->tmpCameraPlayer.look_at({-3.f, -6.f, 4.0f}, {0, 0, 0});
	camera->tmpCameraPlayer.far(50.0f); // With inf projection far is only used for linearized visualization (and camera controls)
	camera->tmpCameraPlayer.near(0.5f);

	// TODO P2: Proper camera control management
	// TODO P2: Proper control management
	// CameraPerspective camera(pos2, fov, aspectRatio, near, far);
	// ControllerOrbit controller(camera, canvas);
	{
		auto& controls = *nexus.object_view_get<libv::ctrl::Controls>();

		// Simulation controls:
		controls.feature_action<SandboxState>("sim.pause", [](libv::ctrl::arg_action, SandboxState& state) {
			state.paused = !state.paused;
		});
		controls.bind("sim.pause", "space");
		controls.feature_binary<SandboxState>("sim.debug.slow-mode", [](libv::ctrl::arg_binary event, SandboxState& state) {
			state.slowMode = event.value;
			state.slowModeCounter = 0;
		});
		controls.bind("sim.debug.slow-mode", "B");
		controls.feature_action<SandboxState>("sim.debug.next-frame", [](libv::ctrl::arg_action, SandboxState& state) {
			state.unpauseNextFrame = true;
		});
		controls.bind("sim.debug.next-frame", "N");

		// Alpha controls:
		controls.feature_action<SandboxState>("alpha.auto-camera", [](libv::ctrl::arg_action, SandboxState& state) {
			state.rotate = !state.rotate;
		});
		controls.bind("alpha.auto-camera", "C");

		// Scene controls:
		controls.feature_action<SandboxState>("scene.freeze-view-frustum", [](libv::ctrl::arg_action, SandboxState& state) {
			state.scene->freezeViewFrustum(!state.scene->freezeViewFrustum());
		});
		controls.bind("scene.freeze-view-frustum", "V");

		const auto addDebugViewCtrl = [&](std::string name, std::string feature, int index, libv::re::DebugCaptureMode captureMode, auto defaultSequence) {
			controls.feature_action<SandboxState>("scene.debug." + feature, [name, index, captureMode](libv::ctrl::arg_action, SandboxState& state) {
				state.scene->debugPass(name, index, captureMode);
			});
			auto ec = controls.bind("scene.debug." + feature, defaultSequence);
			log_sandbox.error_if(ec != libv::ctrl::binding_outcome::success, "Invalid sequence \"{}\"", defaultSequence);
		};
		addDebugViewCtrl(""                      , ""                        , 0, libv::re::DebugCaptureMode::normal, "`");
		addDebugViewCtrl("depth-prime"           , "depth-prime"             , 0, libv::re::DebugCaptureMode::depth, "1");
		addDebugViewCtrl("depth-prime"           , "depth-prime-linear"      , 0, libv::re::DebugCaptureMode::depth_linear, "2");
		addDebugViewCtrl("depth-prime"           , "depth-prime-distance"    , 0, libv::re::DebugCaptureMode::depth_distance, "3");
		addDebugViewCtrl("opaque"                , "opaque"                  , 0, libv::re::DebugCaptureMode::normal, "4");
		addDebugViewCtrl("opaque-stencil"        , "opaque-stencil"          , 0, libv::re::DebugCaptureMode::stencil, "5");
		addDebugViewCtrl("background"            , "background"              , 0, libv::re::DebugCaptureMode::normal, "6");
		addDebugViewCtrl("transparent"           , "transparent"             , 0, libv::re::DebugCaptureMode::normal, "7");
		addDebugViewCtrl("post-process"          , "post-process"            , 0, libv::re::DebugCaptureMode::normal, "8");
		addDebugViewCtrl("post-process-prefilter", "post-process-prefilter"  , 0, libv::re::DebugCaptureMode::normal, "Ctrl + `");
		addDebugViewCtrl("post-process-down"     , "post-process-down-0"     , 0, libv::re::DebugCaptureMode::normal, "Ctrl + 1");
		addDebugViewCtrl("post-process-down"     , "post-process-down-1"     , 1, libv::re::DebugCaptureMode::normal, "Ctrl + 2");
		addDebugViewCtrl("post-process-down"     , "post-process-down-2"     , 2, libv::re::DebugCaptureMode::normal, "Ctrl + 3");
		addDebugViewCtrl("post-process-down"     , "post-process-down-3"     , 3, libv::re::DebugCaptureMode::normal, "Ctrl + 4");
		addDebugViewCtrl("post-process-down"     , "post-process-down-4"     , 4, libv::re::DebugCaptureMode::normal, "Ctrl + 5");
		addDebugViewCtrl("post-process-down"     , "post-process-down-5"     , 5, libv::re::DebugCaptureMode::normal, "Ctrl + 6");
		addDebugViewCtrl("post-process-down"     , "post-process-down-6"     , 6, libv::re::DebugCaptureMode::normal, "Ctrl + 7");
		addDebugViewCtrl("post-process-down"     , "post-process-down-7"     , 7, libv::re::DebugCaptureMode::normal, "Ctrl + 8");
		addDebugViewCtrl("post-process-down"     , "post-process-down-8"     , 8, libv::re::DebugCaptureMode::normal, "Ctrl + 9");
		addDebugViewCtrl("post-process-up"       , "post-process-up-0"       , 0, libv::re::DebugCaptureMode::normal, "Shift + `");
		addDebugViewCtrl("post-process-up"       , "post-process-up-1"       , 1, libv::re::DebugCaptureMode::normal, "Shift + 1");
		addDebugViewCtrl("post-process-up"       , "post-process-up-2"       , 2, libv::re::DebugCaptureMode::normal, "Shift + 2");
		addDebugViewCtrl("post-process-up"       , "post-process-up-3"       , 3, libv::re::DebugCaptureMode::normal, "Shift + 3");
		addDebugViewCtrl("post-process-up"       , "post-process-up-4"       , 4, libv::re::DebugCaptureMode::normal, "Shift + 4");
		addDebugViewCtrl("post-process-up"       , "post-process-up-5"       , 5, libv::re::DebugCaptureMode::normal, "Shift + 5");
		addDebugViewCtrl("post-process-up"       , "post-process-up-6"       , 6, libv::re::DebugCaptureMode::normal, "Shift + 6");
		addDebugViewCtrl("post-process-up"       , "post-process-up-7"       , 7, libv::re::DebugCaptureMode::normal, "Shift + 7");
		addDebugViewCtrl("post-process-up"       , "post-process-up-8"       , 8, libv::re::DebugCaptureMode::normal, "Shift + 8");
		addDebugViewCtrl("post-process-up"       , "post-process-up-9"       , 9, libv::re::DebugCaptureMode::normal, "Shift + 9");
		addDebugViewCtrl("flare"                 , "flare"                   , 0, libv::re::DebugCaptureMode::normal, "9");
		addDebugViewCtrl("outline"               , "outline"                 , 0, libv::re::DebugCaptureMode::normal, "0");
		addDebugViewCtrl("hud"                   , "hud"                     , 0, libv::re::DebugCaptureMode::normal, "-");
		addDebugViewCtrl("outline-jfa-mask"      , "outline-jfa-mask"        , 0, libv::re::DebugCaptureMode::normal, "F1");
		addDebugViewCtrl("outline-jfa-init"      , "outline-jfa-init"        , 0, libv::re::DebugCaptureMode::jfa, "F2");
		addDebugViewCtrl("outline-jfa-step"      , "outline-jfa-step-0"      , 0, libv::re::DebugCaptureMode::jfa, "F3");
		addDebugViewCtrl("outline-jfa-step"      , "outline-jfa-step-1"      , 1, libv::re::DebugCaptureMode::jfa, "F4");
		addDebugViewCtrl("outline-jfa-step"      , "outline-jfa-step-2"      , 2, libv::re::DebugCaptureMode::jfa, "F5");
		addDebugViewCtrl("outline-jfa-step"      , "outline-jfa-step-3"      , 3, libv::re::DebugCaptureMode::jfa, "F6");
		addDebugViewCtrl("outline-jfa-step"      , "outline-jfa-step-4"      , 4, libv::re::DebugCaptureMode::jfa, "F7");
		// addDebugViewCtrl("outline-jfa-step" 0..n ?);
		addDebugViewCtrl("outline-jfa-outline"   ,"outline-jfa-outline"      , 0, libv::re::DebugCaptureMode::normal, "F8");
		addDebugViewCtrl("trace-debug"           , "trace-debug"             , 0, libv::re::DebugCaptureMode::normal, "=");

		controls.context_enter<libv::sun::BaseCameraOrbit>(&camera->tmpCameraPlayer);
		controls.context_enter<SandboxState>(this);
	}

	matTest0 = libv::re::MaterialTest0::create();

	auto sphere = libv::re::Object::create(
			libv::vec3f{0, 0, 0},
			matTest0,
			libv::re::MeshSphere::create(36)
	);
	sphere->transform.scale = libv::vec3f(2.f);
	scene->add(std::move(sphere));

	auto meshSphere = libv::re::MeshSphere::create(36);
	sphereMoving0 = libv::re::Object::create(
			libv::vec3f{0, 0, 0},
			matTest0,
			meshSphere
	);
	scene->add(sphereMoving0);
	sphereMoving1 = libv::re::Object::create(
			libv::vec3f{0, 0, 0},
			libv::re::MaterialTest0::create(), // Different material
			meshSphere
	);
	scene->add(sphereMoving1);
	sphereMoving1->material->state.polygonMode(libv::gl::PolygonMode::Line);
	sphereMoving1->material->state.stencilTest(true);
	sphereMoving1->material->state.stencilWrite(0xFF);
	sphereMoving1->material->state.stencilFunction(libv::gl::TestFunction::Always);
	sphereMoving1->material->state.stencilFunctionReference(0xFF);
	sphereMoving1->material->state.stencilFunctionMask(0xFF);
	sphereMoving1->material->state.stencilOpStencilFail(libv::gl::StencilAction::Keep);
	sphereMoving1->material->state.stencilOpDepthFail(libv::gl::StencilAction::Keep);
	sphereMoving1->material->state.stencilOpStencilAndDepthPass(libv::gl::StencilAction::Replace);

	scene->add(libv::re::Skybox::create(
			libv::r.texture.load_async("texture/sky/debug_x_front_uv_cube_srgb.dds")));

	auto meshSmallSphere = libv::re::MeshSphere::create(12);
	auto materialSmallSphere = libv::re::MaterialTest0::create();
	for (int x = -5; x <= 5; ++x) {
		const auto xf = static_cast<float>(x);
		for (int y = -5; y <= 5; ++y) {
			const auto yf = static_cast<float>(y);

			auto ball = libv::re::Object::create(
					libv::vec3f{xf, yf, -1.f},
					materialSmallSphere,
					meshSmallSphere
			);
			ball->transform.scale = libv::vec3f{0.5f};
			scene->add(ball);
		}
	}

	{
		auto materialStenciled = libv::re::MaterialTest0::create();
		materialStenciled->state.stencilTest(true);
		materialStenciled->state.stencilWrite(0xFF);
		materialStenciled->state.stencilFunction(libv::gl::TestFunction::Always);
		materialStenciled->state.stencilFunctionReference(0x80);
		materialStenciled->state.stencilFunctionMask(0xFF);
		materialStenciled->state.stencilOpStencilFail(libv::gl::StencilAction::Keep);
		materialStenciled->state.stencilOpDepthFail(libv::gl::StencilAction::Keep);
		materialStenciled->state.stencilOpStencilAndDepthPass(libv::gl::StencilAction::Replace);

		auto stenciledBall = libv::re::Object::create(
				libv::vec3f{0, -2.5f, 0.f},
				materialStenciled,
				meshSphere
		);
		stenciledBall->transform.scale = libv::vec3f{1.0f};
		scene->add(stenciledBall);
	}

	{
		auto ballDistant = libv::re::Object::create(
				libv::vec3f{-15.f, 0.f, 0.f},
				matTest0,
				meshSphere
		);
		ballDistant->transform.rotation.rotate(libv::tau_f * .25f, libv::vec3f(0, 1, 0));
		scene->add(ballDistant);
	}
	{
		auto ball = libv::re::Object::create(
				libv::vec3f{-3, -2.5f, 0.f},
				matTest0,
				meshSphere
		);
		ball->transform.scale = libv::vec3f{1.0f};
		auto ballOutline = libv::re::Outline::create(ball, libv::vec4f{1.f, 0.0f, 0.0f, 1.0f});
		scene->add(ball);
		scene->add(ballOutline);
	}
	{
		auto ball = libv::re::Object::create(
				libv::vec3f{3, -2.5f, 0.f},
				libv::re::MaterialTest0::create(),
				libv::re::MeshSphere::create(2)
		);
		ball->transform.scale = libv::vec3f{3.0f};
		// ball->transform.scale = libv::vec3f{1.0f};
		ball->material->state.cull(false);
		ball->material->state.polygonMode(libv::gl::PolygonMode::Line);
		auto ballOutline = libv::re::Outline::create(ball, libv::vec4f{1.f, 0.95f, 0.1f, 0.85f});
		ballOutline->materials.object->state.cull(false);
		ballOutline->materials.object->state.polygonMode(libv::gl::PolygonMode::Line);
		ballOutline->materials.model->state = ballOutline->materials.object->state;
		scene->add(ball);
		scene->add(ballOutline);
	}

	{
		auto l = libv::re::LightPoint::create();
		l->transform.position = libv::vec3f{6, 2, 3};
		l->color = libv::vec3f{1.0f, 0.4f, 0.4f};
		scene->add(l);
	}
	{
		auto l = libv::re::LightPoint::create();
		l->transform.position = libv::vec3f{2, 8, 3};
		l->color = libv::vec3f{0.8f, 0.8f, 1.0f};
		scene->add(l);
	}
	{
		movingLight0 = libv::re::LightPoint::create();
		movingLight0->transform.position = libv::vec3f{2, 6, 3};
		movingLight0->color = libv::vec3f{0.4f, 1.0f, 0.4f};
		scene->add(movingLight0);
	}
	{
		auto circle_01 = libv::r.texture.load_async("texture/particle/circle_01.png");
		auto circle_05 = libv::r.texture.load_async("texture/particle/circle_05.png");
		auto star_01 = libv::r.texture.load_async("texture/particle/star_01.png");
		circle_01->swizzle("111r");
		circle_05->swizzle("111r");
		star_01->swizzle("111r");

		auto lensFlare = libv::re::LensFlare::create(movingLight0, std::chrono::milliseconds(250), 0.01f);
		lensFlare->add(  star_01, 0.0f, 500.f, -1.f, libv::vec4f{1}, 1.f);
		lensFlare->add(circle_01, 0.6f, 100.f);
		lensFlare->add(  star_01, 1.2f, 120.f, -1.f, libv::vec4f{0.5f, 1.f, 1.f, 0.5f});
		lensFlare->add(circle_05, 1.5f, 180.f, -1.f, libv::vec4f{0.5f, 1.f, 1.f, 0.5f});
		scene->add(lensFlare);
	}

	// Sun
	// /*Warm*/ auto sun = libv::re::LightDirectional::create(libv::vec3f{-1, -1, -3.f}.normalize(), libv::vec3f{0.9f, 0.8f, 0.6f});
	// /*Down*/ auto sun = libv::re::LightDirectional::create(libv::vec3f{-1, -50, -3.f}.normalize(), libv::vec3f{0.9f, 0.8f, 0.6f});
	/*White*/ auto sun = libv::re::LightDirectional::create(libv::vec3f{-1, -1, -3.f}.normalize(), libv::vec3f{1.f});
	scene->add(sun);

	{
		auto circle_01 = libv::r.texture.load_async("texture/particle/circle_01.png");
		auto circle_05 = libv::r.texture.load_async("texture/particle/circle_05.png");
		auto star_01 = libv::r.texture.load_async("texture/particle/star_01.png");
		auto star_03 = libv::r.texture.load_async("texture/particle/star_03.png");
		auto star_06 = libv::r.texture.load_async("texture/particle/star_06.png");
		circle_01->swizzle("111r");
		circle_05->swizzle("111r");
		star_01->swizzle("111r");
		star_03->swizzle("111r");
		star_06->swizzle("111r");

		auto lensFlare = libv::re::LensFlare::create(sun, std::chrono::milliseconds(250), 1.00f);
		lensFlare->add(  star_01, 0.0f, 10.f, -1.f, libv::vec4f{1}, 1.f);
		lensFlare->add(  star_01, 0.0f, 500.f, -1.f, libv::vec4f{1}, 1.f);
		lensFlare->add(  star_06, 0.4f, 120.f, -1.f, libv::vec4f::one(), 0.f, libv::deg_to_rad(10.f), false);
		lensFlare->add(circle_01, 0.5f, 100.f, 300.f);
		lensFlare->add(circle_01, 0.6f, 100.f);
		lensFlare->add(  star_01, 0.7f, 120.f, -1.f, libv::vec4f{0.5f, 1.f, 1.f, 1.f});
		lensFlare->add(circle_05, 0.9f, 180.f, -1.f, libv::vec4f{0.5f, 1.f, 1.f, 1.f});
		lensFlare->add(  star_06, 1.0f, 120.f);
		lensFlare->add(  star_06, 1.2f, 80.f, -1.f, libv::vec4f::one(), 0.f, libv::deg_to_rad(45.f), true);
		lensFlare->add(  star_06, 1.4f, 80.f, -1.f, libv::vec4f::one(), 0.f, libv::deg_to_rad(0.f), true);
		lensFlare->add(  star_03, 1.7f, 320.f, 1200.f);
		lensFlare->add(  star_03, 2.0f, 320.f, -1.f, libv::vec4f{0.5f, 0.5f, 0.5f, 1}, 0.5f);
		scene->add(lensFlare);
	}

	{ // Ground plane
		auto m = libv::re::MaterialTest0::create();
		m->state.cull(false);
		m->mode = 1;
		// m->texture0 = libv::r.texture.load_async("texture/hexagon_metal_0001_diffuse.dds");
		m->texture0 = libv::re::unitBaseColor;
		m->texture0 = libv::r.texture.load_async("texture/test_pattern_metal_a_2k_base_srgb.dds");
		m->texture1 = libv::re::unitNormal;
		m->texture1 = libv::r.texture.load_async("texture/test_pattern_metal_a_2k_normal.dds");
		m->texture2 = libv::re::unitRMAO;
		m->texture2 = libv::r.texture.load_async("texture/test_pattern_metal_a_2k_rmao.dds");
		auto plane = libv::re::Object::create({0, 0, -1.5f}, m, libv::re::MeshPlane::create(libv::vec2i(32, 32), libv::vec2f(3, 3)));
		plane->transform.scale = libv::vec3f(16, 16, 1);
		scene->add(plane);

		auto cube = libv::re::Object::create({10, -10, -0.55f}, m, libv::re::MeshCube::create());
		cube->transform.scale *= 2.f;
		scene->add(cube);
	}

	auto ls0 = libv::re::LightSpot::create(libv::vec3f{12, -15, -1}, libv::vec3f{0, 5, -7}, 0.6f, 0.6f, {1.0f,0.7f,0.7f}, 4.f);
	scene->add(ls0);
	scene->add(libv::re::LightSpot::create(libv::vec3f{ 8, -15, -1}, libv::vec3f{0, 5, -7}, 0.8f, 0.6f, {0.7f,1.0f,0.7f}, 4.f));
	scene->add(libv::re::LightSpot::create(libv::vec3f{ 4, -15, -1}, libv::vec3f{0, 5, -7}, 0.8f, 0.6f, {0.7f,0.7f,1.0f}, 4.f));
	scene->add(libv::re::LightPoint::create(libv::vec3f{ 0, -15, -1}, {1.0f, 1.0f, 1.0f}, 2.f));

	// Model tests:
	auto model = libv::r.model.load_async("model/test_node_02_med.game.vm4");
	scene->add(libv::re::ModelStatic::create(libv::vec3f{0, 10, 0}, model));
	scene->add(libv::re::ModelStatic::create(libv::vec3f{10, 10, 0}, model));
	auto model2 = libv::r.model.load_async("model/test_instance_node_med.game.vm4");
	auto model2Node = libv::re::ModelStatic::create(libv::vec3f{-10, 10, 0}, model2);
	auto model2Outline = libv::re::Outline::create(model2Node, libv::vec4f{0.38f, 1.0f, 0.38f, 0.85f});
	scene->add(model2Node);
	scene->add(model2Outline);
	auto model3 = libv::r.model.load_async("model/fighter_01_eltanin.vm4");
	movingModel = libv::re::ModelStatic::create(libv::vec3f{0, 20, 0}, model3);
	scene->add(movingModel);

	// Material override tests:
	scene->add(libv::re::Outline::create(libv::re::ModelStatic::create(libv::vec3f{0, 30, 0}, model3), libv::vec4f{1.f, 0.95f, 0.1f, 0.85f}));
	scene->add(libv::re::ModelStatic::create(libv::vec3f{0, 40, 0}, libv::r.model.load_async("model/fighter_01_eltanin.vm4", libv::re::MaterialSolidModel::create({0.4f, 1.f, 0.4f, 1}), libv::re::VariantID{100})));
	scene->add(libv::re::ModelStatic::create(libv::vec3f{0, 50, 0}, libv::r.model.load_async("model/fighter_01_eltanin.vm4", [](const libv::re::MaterialScanner& scanner) { return libv::re::MaterialSolidModel::create(scanner); }, libv::re::VariantID{101})));
	scene->add(libv::re::ModelStatic::create(libv::vec3f{0, 60, 0}, libv::r.model.load_async("model/fighter_01_eltanin.vm4", [](const libv::vm4::Material& vm4Material) { return libv::r.material.load(vm4Material); }, libv::re::VariantID{102})));

	// Material multi-material:
	scene->add(libv::re::ModelStatic::create(libv::vec3f{-10, 30, 0}, libv::r.model.load_async("model/test_multi_material.vm4", [](const libv::re::MaterialScanner& scanner) { return libv::re::MaterialSolidModel::create(scanner); }, libv::re::VariantID{100})));

	// Bloom
	scene->add(libv::re::Object::create(libv::vec3f{-40, 0, 0}, libv::re::MaterialSolid::create({10, 10, 10, 1}), libv::re::MeshCube::create()));
	scene->add(libv::re::Object::create(libv::vec3f{-40, 10, 0}, libv::re::MaterialSolid::create({20, 10, 10, 1}), libv::re::MeshCube::create()));
	scene->add(libv::re::Object::create(libv::vec3f{-40, 20, 0}, libv::re::MaterialSolid::create({30, 10, 10, 1}), libv::re::MeshCube::create()));
	scene->add(libv::re::Object::create(libv::vec3f{-40, 30, 0}, libv::re::MaterialSolid::create({40, 10, 10, 1}), libv::re::MeshCube::create()));

	// Grid
	scene->add(libv::re::EditorGrid::create());
}

SandboxState::~SandboxState() {
	// TODO P2: Controls guard objects for safety
	auto& controls = *nexus.object_view_get<libv::ctrl::Controls>();
	controls.context_leave<libv::sun::BaseCameraOrbit>();
	controls.context_leave<SandboxState>();
}

void SandboxState::async_update(libv::time_point timeFrame, libv::time_duration timeDelta) {
	this->timeRealFrame = timeFrame;

	if ((!paused && !slowMode) || unpauseNextFrame || (slowMode && (++slowModeCounter % 10) == 0)) {
		unpauseNextFrame = false;
		timeSim += timeDelta;

		const auto timeEffect = static_cast<float>(timeSim.count());
		sphereMoving0->transform.position.x = 4.f + 2.f + 2.0f * std::sin(timeEffect * libv::tau_f * 0.1f);
		sphereMoving0->transform.rotation.rotate(static_cast<float>(timeDelta.count()) * 0.1f * libv::tau_f, libv::vec3f(0, 0, 1));

		sphereMoving0->transform.scale = libv::vec3f(2.f);

		sphereMoving1->transform.position.x = -4.f;
		// sphereMoving1->transform.position.z = -std::sin(timeEffect);
		sphereMoving1->transform.scale = libv::vec3f(2.f, 2.f, 1.f + std::sin(timeEffect));

		movingLight0->transform.position.x = 10.0f * std::sin(timeEffect * libv::tau_f * -0.2f);
		movingLight0->transform.position.y = 10.0f * std::cos(timeEffect * libv::tau_f * -0.2f);
		movingLight0->transform.position.z = -1.f;

		movingModel->transform.rotation = libv::quatf::create_rotate(0.5f * std::cos(timeEffect * libv::tau_f * -0.075f), libv::vec3f{0, 1, 0});
	}

	if (rotate)
		camera->tmpCameraPlayer.orbit_yaw(static_cast<float>(timeDelta.count()) * libv::tau * 0.01f);
	// // camera->tmpCameraPlayer.orbit_yaw(static_cast<float>(timeDelta.count()) * libv::tau * 0.05f);
	// // camera->tmpCameraPlayer.orbit_pitch(static_cast<float>(timeDelta.count()) * libv::tau * 0.05f);

	// =================================================================================================
	// // Define test
	// auto s = libv::re::r.shader.load("re/test/material_test0.vs", "re/test/material_test0.fs");
	// static int i = 0;
	// static int j = 60;
	// static bool test = false;
	// if (j++ == 60*10) {
	// 	j = 0;
	// 	s->undefine("TEST");
	// }
	// if (i++ == 60*5) {
	// 	i = 0;
	// 	test = !test;
	// 	if (test)
	// 		s->define("TEST 0");
	// 	else
	// 		s->define("TEST 1");
	// }
	// =================================================================================================

	// !!! Should be set differently, also eye must be mutex guarded, 1 frame diff is fine, its only used for sort
	//		but best would be a delayed evaluation, or rather the camera update be part of the async update process?
	// state.eye = camera.eye();
}

void SandboxState::create(libv::GL& gl) {
	(void) gl;
}

void SandboxState::destroy(libv::GL& gl){
	(void) gl;
}

void SandboxState::render(libv::GL& gl, libv::vec2i canvasPosition, libv::vec2i canvasSize, libv::vec2f localMousePosition) {
	{
		const auto timeEffect = timeRealFrame - timeRealStart;
		const auto timeEffectLooping = static_cast<float>(std::fmod((timeEffect).count() + 1800.0, 3600.0) - 1800.0);
		matTest0->uniform_dynamic = timeEffectLooping;
	}

	scene->render(
			gl,
			*camera,
			canvasPosition,
			canvasSize,
			localMousePosition,
			timeSim.count(),
			(timeRealFrame - timeRealStart).count());
}

// -------------------------------------------------------------------------------------------------

// // void render(libv::re::Engine& rev) {
// // void render(libv::re::RenderPass& rev) {
// // 	fountain.render(rev);
// 	// fountain.render(rev(layer::effect));
// 	// fountain.render(rev.layer(layer::effect));
// 	// rev.render(fountain);
// 	// rev.render(fountain, layer::effect);
// 	// fountain.sort(glr.eye());
// 	rev.rendererParticle.render(glr, rev.uniformStream, fountain);

// update
	// if (enableSimulationUpdate)
	// 	fountain.update(timeDelta);
	// 	// fountain.update(timeDelta, eye);
	// // else
	// // 	fountain.sort(eye);
	// //
	// // if constexpr (!isServer)
	// // 	ships[0].engineBloom.update();

// effect definitions, effect library, load from lua
// // for (int i = 0; i < 10; ++i) {
// // 	auto& fountain = fountains.emplace_back();
// // 	fountain.addModule(std::make_unique<libv::re::ParticleModuleEmitter>(true, libv::vec3f{0}, libv::vec3f{0,
// 0, 40.f}, 50.f));
// // 	fountain.addModule(std::make_unique<libv::re::ParticleModuleGravity>());
// // 	fountain.addModule(std::make_unique<libv::re::ParticleModuleTurbulence>(1.f));
// // }
// fountain.addModule(std::make_unique<libv::re::ParticleModuleEmitter>(true, libv::vec3f{0}, libv::vec3f{0,
// 0, 40.f}, 50.f)); fountain.addModule(std::make_unique<libv::re::ParticleModuleGravity>());
// fountain.addModule(std::make_unique<libv::re::ParticleModuleTurbulence>(1.f));

