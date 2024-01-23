// Project: libv, File: app/star/game/sim/universe.cpp

#include <star/game/sim/universe.hpp>

#include <libv/math/constants.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

Universe::Universe() {
	starmap = std::make_shared<Starmap>();
	surfaceA = std::make_shared<Surface>();
	surfaceB = std::make_shared<Surface>();
}

void Universe::update(SimTime time, SimTime delta) {
	starmap->update(time, delta);
	surfaceA->update(time, delta);
	surfaceB->update(time, delta);
}

// -------------------------------------------------------------------------------------------------

Starmap::Starmap() {
	sky = Sky("texture/sky/ambience_morning_green_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY, 1.f, libv::tau_f * 0.5f);
	movingBall = Ball(libv::vec3f{15, 0, 0}, 1.f);
	pickingBall = Ball(libv::vec3f{0, 0, 0}, 0.1f);
	balls.emplace_back(libv::vec3f{+10, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{-10, 0, 0}, 1.f);
}

void Starmap::update(SimTime time, SimTime delta) {
	(void) delta;
	movingBall.position.x = std::cos(time.count()) * 15.f;
	movingBall.position.y = std::sin(time.count()) * 15.f;
}

// -------------------------------------------------------------------------------------------------

Surface::Surface() {
	sky = Sky("texture/sky/ambience_morning_green_cube_hdr.dds", libv::re::SkyboxType::cubemapZXY, 1.f, libv::tau_f * 0.5f);
	movingBall = Ball(libv::vec3f{15, 0, 0}, 1.f);
	pickingBall = Ball(libv::vec3f{0, 0, 0}, 0.1f);
	balls.emplace_back(libv::vec3f{+10, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{+5, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{ 0, 0, 0}, 3.f);
	balls.emplace_back(libv::vec3f{-5, 0, 0}, 1.f);
	balls.emplace_back(libv::vec3f{-10, 0, 0}, 1.f);
}

void Surface::update(SimTime time, SimTime delta) {
	(void) delta;
	movingBall.position.x = std::cos(time.count()) * 15.f;
	movingBall.position.y = std::sin(time.count()) * 15.f;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
