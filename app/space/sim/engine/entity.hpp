// Project: libv, File: app/space/sim/engine/entity.hpp

#pragma once

// libv
#include <libv/utility/entity/entity_ptr_fwd.hpp>
// std
#include <atomic>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename CRTP, typename ID_t, ID_t InvalidID>
class Entity {
	friend libv::entity_access;

public:
	using ID = ID_t;
	using Base = Entity;

private:
	std::atomic_uint32_t ref_count = 0;
	bool dead_ = false;
	// TODO P3: dead_ could be stored as ref_count's LSB and increase/decrease ref_count by 2 instead of 1
	//			dead could and/or should be stored as a single bit inside ID
	//				=> invalidID:= 0, firstID:=1, isDead:= value < 0, id:=abs(value)
	//					-> requires definition of operator<=> for the enum type as abs has to be used
	//					 \ OR store a different type than the ID

public:
	ID id = InvalidID;
	// TODO P3: entity_ptr system could have an implicit ID based on memory block start index and the offset
	//			Added bonuses: 0 id could/will be invalid, auto id reuse
	//			! Not viable, serialization through network would fail, id ID's are not kept in sync

protected:
	constexpr inline Entity() noexcept = default;
	explicit constexpr inline Entity(ID id) noexcept : id(id) {}

public:
	constexpr inline void kill() noexcept {
		dead_ = true;
	}

	[[nodiscard]] constexpr inline bool dead() const noexcept {
		return dead_;
	}

//	libv::observer_ptr<Simulation> simulation() {
//		return libv::entity_store<CRTP>::context_from_pointer(static_cast<CRTP*>(this));
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
