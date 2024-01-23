// Project: libv, File: app/star/game/control/surface_controller.cpp

#include <star/game/control/surface_controller.hpp>

#include <libv/ctrl/controls.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/math/plane.hpp>

#include <star/game/scene/scene_surface.hpp>
#include <star/game/sim/universe.hpp>

#include <libv/re/node/node.hpp>

// For testings skys only:
#include <libv/re/node/skybox.hpp>
#include <libv/re/resource/texture_loader.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

SurfaceController::SurfaceController(libv::Nexus& nexus) :
	nexus(nexus) {
}

void SurfaceController::init(Surface* surface, CanvasSurface* canvas) {
	this->surface = surface;
	this->canvas = canvas;
}

SurfaceController::~SurfaceController() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

void SurfaceController::request(RequestSurfacePlaceBallUnderCursor) {
	const auto plane = libv::planef::from_normal_point({0, 0, 1}, {0, 0, 0});

	const auto intersection = libv::intersect_ray_plane(canvas->mouseRay, plane);
	if (!intersection)
		return; // Player visibile error notifiction would go here (or a nexus event)

	surface->balls.emplace_back(*intersection, 0.5f);
}

void SurfaceController::setSky(int index) {
	std::vector<std::pair<std::string_view, libv::re::SkyboxType>> skys{
			{"texture/sky/debug_x_front_uv_cube_srgb.dds", libv::re::SkyboxType::cubemapXYZ},

			{"texture/sky/lago_disola_eqrt_srgb.dds", libv::re::SkyboxType::equirectangular},
			{"texture/sky/lago_disola_eqrt_hdr.dds", libv::re::SkyboxType::equirectangular},
			{"texture/sky/lago_disola_cube_srgb.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/lago_disola_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},

			{"texture/sky/ambience_morning_green_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/lago_disola_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/pizzo_pernice_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},

			{"texture/sky/drakensberg_solitary_mountain_puresky_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/kloppenheim_03_puresky_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/mud_road_puresky_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/qwantani_puresky_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/sunflowers_puresky_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY},

			{"texture/sky/tryo_nebula40_white_cube_srgb.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/tryo_nebula80_colorful_cube_srgb.dds", libv::re::SkyboxType::cubemapZXY},
			{"texture/sky/tryo_stars_sl_cube_srgb.dds", libv::re::SkyboxType::cubemapZXY},
	};

	const auto& sky = skys[index % skys.size()];
	canvas->skybox->textureSky(libv::r.texture.load(sky.first, libv::re::VariantID::Default, libv::re::LoadingMode::async, canvas->skybox->texture()), sky.second);
}

void SurfaceController::request(RequestSurfaceSkyNext) {
	setSky(++testSkyIndex);
}

void SurfaceController::request(RequestSurfaceSkyPrev) {
	setSky(--testSkyIndex);
}

void SurfaceController::request(RequestSurfaceToggleGrid) {
	canvas->editorGrid->visible(!canvas->editorGrid->visible());
}

// -------------------------------------------------------------------------------------------------

void SurfaceController::register_controls(libv::ctrl::Controls& controls, SurfaceControllerFeatures features) {
	using ControlState = SurfaceController;

	const auto act = [&](bool enable, std::string&& name, auto&& reqProto) {
		if (enable) {
			controls.feature_action<ControlState>(std::move(name), [req = std::move(reqProto)](libv::ctrl::arg_action, ControlState& state) {
				state.request(req);
			});
		}
	};

	act(features.placeBall, "surface.place-ball", RequestSurfacePlaceBallUnderCursor{});
	act(features.placeBall, "surface.sky.next", RequestSurfaceSkyNext{});
	act(features.placeBall, "surface.sky.prev", RequestSurfaceSkyPrev{});
	act(features.placeBall, "surface.debug.toggle-grid", RequestSurfaceToggleGrid{});
}

void SurfaceController::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("surface.place-ball", "M");
	controls.bind("surface.sky.next", "Page Up");
	controls.bind("surface.sky.prev", "Page Down");
	controls.bind("surface.debug.toggle-grid", "F5");
}

// -------------------------------------------------------------------------------------------------

} // namespace star
