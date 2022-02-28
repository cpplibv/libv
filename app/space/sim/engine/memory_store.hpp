// Project: libv, File: app/space/sim/engine/memory_store.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/utility/entity/entity_ptr.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct MemoryStore {
public:
	libv::entity_store<Faction> faction;
	libv::entity_store<Planet> planet;
	libv::entity_store<Fleet> fleet;

public:
	MemoryStore();
	MemoryStore(const MemoryStore&) = delete;
	MemoryStore& operator=(const MemoryStore&) & = delete;
	MemoryStore(MemoryStore&&) noexcept;
	MemoryStore& operator=(MemoryStore&&) & noexcept;
	~MemoryStore();

public:
	template <typename T>
	void create_to(libv::entity_ptr<T>& result) {
		if constexpr (std::is_same_v<T, Faction>)
			result = faction.create();

		else if constexpr (std::is_same_v<T, Planet>)
			result = planet.create();

		else if constexpr (std::is_same_v<T, Fleet>)
			result = fleet.create();

		else
			T::missing_branch();
	}

private:
	void checkLeak() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
