// Project: libv, File: app/space/sim/building_resources.hpp

#pragma once

// libv
#include <libv/serial/serial.hpp>
#include <libv/utility/to_underlying.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

// TODO P4: Locale for BuildingResource
// TODO P5: Different type for each resource. Multi type array? (tuple with nice interface?)
enum class BuildingResource : uint32_t {
	Energy,
	Fertility,
	Hospital,
	Housing,
//	Police,
//	Firefighter,
//	Education,
//	Pollution,
	Morale,
	ProductionBuilding,
	ProductionShip,
	ProductionTank,
	Science,
	Spy,
	Upkeep,
	Workforce, _LAST = BuildingResource::Workforce,
};

using BuildingResourceValueType = int32_t;

class BuildingResources {
	static constexpr auto resourcesCount = libv::to_value(BuildingResource::_LAST) + 1;

	BuildingResourceValueType values[resourcesCount];

public:
	BuildingResources() {
		clear();
	}
	inline void clear() {
		for (auto& value : values) {
			value = 0;
		}
	}
	inline auto& operator[](BuildingResource i) {
		return values[libv::to_value(i)];
	}
	inline auto operator[](BuildingResource i) const {
		return values[libv::to_value(i)];
	}
	inline BuildingResources operator+(const BuildingResources& rhs) const {
		BuildingResources result;
		return result += rhs;
	}
	inline BuildingResources& operator+=(const BuildingResources& rhs) {
		for (size_t i = 0; i < resourcesCount; ++i) {
			values[i] += rhs.values[i];
		}
		return *this;
	}

public:
	template <typename Archive> void serialize(Archive& ar, const unsigned int) const {
		for (auto value : values) {
			ar & LIBV_NVP(value);
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
