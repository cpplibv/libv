// Project: libv, File: src/space/universe/force.hpp

// hpp
#include <space/universe/faction.hpp>
// libv
#include <libv/color/space.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Faction::Faction(FactionID id, std::string name) :
		id(id),
		name(std::move(name)) {

	if (+id < 13) {
		const auto idf = static_cast<float>(+id) - 1.f;
		colorPrimary = libv::color::hsv_to_rgb({idf / 12.f, 0.75f, 1.f, 1.f});
		colorSecondary = libv::color::hsv_to_rgb({idf / 12.f + 0.5f / 12.f, 0.6f, 1.f, 1.f});

	} else {
		auto rng = libv::xoroshiro128(+id + 500000);
		auto dist = libv::make_uniform_distribution_inclusive(0.f, 1.f);
		libv::vec4f color0hsva;
		color0hsva.x = dist(rng);
		color0hsva.y = dist(rng) * 0.6f + 0.2f;
		color0hsva.z = dist(rng) * 0.4f + 0.6f;
		color0hsva.w = 1.f;
		libv::vec4f color1hsva;
		color1hsva.x = dist(rng);
		color1hsva.y = dist(rng) * 0.6f + 0.2f;
		color1hsva.z = dist(rng) * 0.4f + 0.6f;
		color1hsva.w = 1.f;
		colorPrimary = libv::color::hsv_to_rgb(color0hsva);
		colorSecondary = libv::color::hsv_to_rgb(color1hsva);
	}
}

Faction::Faction(FactionID id, std::string name, libv::vec4f colorPrimary, libv::vec4f colorSecondary) :
		id(id), name(std::move(name)), colorPrimary(colorPrimary), colorSecondary(colorSecondary) {}

void Faction::kill() {
	// noop
}

void Faction::update(sim_duration delta_time) {
	(void) delta_time;
}

bool Faction::canAttack(const Faction& other) const {
	// TODO P4: Use relation matrix to determine canAttack
	return id != other.id;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
