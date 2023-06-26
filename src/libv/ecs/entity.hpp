// Project: libv.ecs, File: src/libv/ecs/entity.hpp

#pragma once

// std
#include <vector>
// pro
#include <libv/ecs/common.hpp>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

struct Entity {
	EntityID id;
	EntityComponents components;

	// IDEA: Rotating id: no real id! Instead of Entity store only the EntityComponents
	//		For references use watchers
	//		-->> this is basically a pure EntityComponents index (index as dense meta data)
	//		and uses the position as "EntityID". Deletion and addition is based on rotating the last and
	//		updating every component store to do the same.
	//		NOTE: Huge drawback i could not find a solution to parent and hierarchy handling yet
	//		I had a brief idea to rotate every child if there is not enough space, need some work
	//		maybe an additional parent indexing

	// TODO P4: currently implicit ctor for boost::flat_set::find
	inline Entity(EntityID id) : id(id) { }

	explicit inline Entity(EntityID id, EntityComponents components) :
		id(id), components(components) { }

	inline bool operator<(const Entity& rhs) const {
		return id < rhs.id;
	}
	inline bool operator==(EntityID rhs) const {
		return id == rhs;
	}
};

struct ChildOfCmp {
	static inline bool operator()(const Entity& lhs, const EntityID& rhs) {
		return (lhs.id >> 32) < rhs;
	}
	static inline bool operator()(const EntityID& lhs, const Entity& rhs) {
		return lhs < (rhs.id >> 32);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv
