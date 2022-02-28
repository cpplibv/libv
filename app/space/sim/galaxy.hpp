// Project: libv, File: app/space/sim/galaxy.hpp

#pragma once

// libv
#include <libv/utility/entity/entity_ptr.hpp>
// std
#include <memory>
#include <vector>
// pro
//#include <space/fwd.hpp>
#include <space/sim/engine/chrono.hpp>
#include <space/sim/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

/// Galaxy == Star Map
class Galaxy {
public:
	std::shared_ptr<SimulationContext> ctx;

	std::vector<libv::primary_entity_ptr<Planet>> planets;
	std::vector<libv::primary_entity_ptr<Fleet>> fleets;

public:
	template <typename Archive> void serialize(Archive& ar);

public:
	explicit Galaxy(const std::shared_ptr<SimulationContext>& ctx);
//	Galaxy(const Galaxy&);
//	Galaxy& operator=(const Galaxy&) &;
	Galaxy(Galaxy&&) noexcept;
	Galaxy& operator=(Galaxy&&) & noexcept;
	~Galaxy();

public:
	void kill();
	void update(sim_duration delta_time);

public:
	void debug();
};

// -------------------------------------------------------------------------------------------------

} // namespace space
