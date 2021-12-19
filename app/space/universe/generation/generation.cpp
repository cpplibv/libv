// Created by Vader on 2021.12.13..

// hpp
#include <space/universe/generation/generation.hpp>
// libv
#include <libv/math/constants.hpp>
#include <libv/utility/random/normal_distribution.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <space/universe/galaxy.hpp>
#include <space/universe/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Planet generatePlanet(PlanetID id, libv::vec3f position, libv::xoroshiro128& rng_planet) {
	Planet planet{id, position};

	planet.radius = libv::normal998_clamp(rng_planet, 0.3f, 0.2f);

	{
		auto dst = libv::make_uniform_distribution_inclusive(0.f, 1.f);

		const auto r0 = dst(rng_planet);
		const auto g0 = dst(rng_planet);
		const auto b0 = dst(rng_planet);
		const auto r1 = dst(rng_planet);
		const auto g1 = dst(rng_planet);
		const auto b1 = dst(rng_planet);

		planet.color0 = {r0, g0, b0, 1.0f};
		planet.color1 = {r1, g1, b1, 1.0f};
//		planet.mass = libv::make_uniform_distribution_inclusive(1000.f, 1000000000.f)(rng_planet);
	}

	return planet;
}

Galaxy generateGalaxy(GalaxyGenerationSettings settings) {
	Galaxy galaxy;
	auto galaxy_rng = libv::xoroshiro128(settings.seed);

	for (int i = 0; i < settings.numPlanet; ++i) {
		const auto a0 = libv::make_uniform_distribution_exclusive(0.f, libv::tau)(galaxy_rng);
		const auto r0 = std::sqrt(libv::make_uniform_distribution_inclusive(0.f, 1.f)(galaxy_rng)) * settings.galaxyRadius;
		const auto x0 = std::cos(a0) * r0;
		const auto y0 = std::sin(a0) * r0;
//		const auto z0 = libv::make_uniform_distribution_inclusive(-1.f, 1.f)(rng_planet);
		const auto z0 = libv::normal998_clamp(galaxy_rng, 0.f, settings.galaxyHeight);
		const auto position = libv::vec3f{x0, y0, z0};

		auto rng_planet = galaxy_rng.fork();
		galaxy.planets.push_back(generatePlanet(galaxy.nextPlanetID++, position, rng_planet));
		// <<< Assign Neutral faction in a better way
		galaxy.planets.back().faction = galaxy.faction("Neutral");
	}

	return galaxy;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
