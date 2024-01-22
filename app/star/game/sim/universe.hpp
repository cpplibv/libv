//

#pragma once

#include <memory>

#include <libv/math/vec.hpp>
#include <libv/re/material/skybox_type.hpp>
#include <libv/utility/chrono.hpp>

#include <star/game/fwd.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

using SimTime = libv::time_duration_f;

// -------------------------------------------------------------------------------------------------

struct Sky { // When assets and prefabs exists in the engine this chould be removed
	std::string texture;
	libv::re::SkyboxType type = libv::re::SkyboxType::cubemapZXY;
	float intensity = 1.f;
	float rotateZ = 0.f;
	float horizonOffset = 0.f;
};

// =================================================================================================

struct Universe {
	std::shared_ptr<Starmap> starmap;
	std::shared_ptr<Surface> surfaceA;
	std::shared_ptr<Surface> surfaceB;

public:
	Universe();
	void update(SimTime time, SimTime delta);
};

// -------------------------------------------------------------------------------------------------

struct Starmap {
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
	Starmap();
	void update(SimTime time, SimTime delta);
};

// -------------------------------------------------------------------------------------------------

struct Surface {
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
	void update(SimTime time, SimTime delta);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
