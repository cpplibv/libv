// Project: libv.ecs, File: src/libv/ecs/system.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
#include <range/v3/view/subrange.hpp>
// std
#include <algorithm>
#include <utility>
// pro
#include <libv/ecs/common.hpp>
#include <libv/ecs/component.hpp>
#include <libv/ecs/entity.hpp>
#include <libv/ecs/storage.hpp>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

class System {
private:
	boost::container::flat_set<Entity> entities;
	// TODO P5: libv.ecs: place the hard coded numbers somewhere (64 component)
	std::array<std::unique_ptr<ComponentStorageBase>, 64> storages;
	EntityID nextID = 0;

private:
	template <typename T>
	ComponentStorage<T>& storage(ComponentSystemID id) {
		auto& storage_up = storages[id];
		if (storage_up == nullptr)
			storage_up = std::make_unique<ComponentStorage<T>>();
		return static_cast<ComponentStorage<T>&>(*storage_up);
	}

public: // create ----------------------------------------------------------------------------------
	template <typename... Cs, typename F>
	EntityID create(EntityID parentID, F&& f) {
		auto id = (parentID << 32) + (++nextID & 0xFFFFFFFF);
		entities.emplace(id, make_bitset<Cs::ID...>());
		std::forward<F>(f)(storage<typename Cs::type>(Cs::ID)[id]...);
		return id;
	}

	template <typename... Cs, typename F>
	inline EntityID create(F&& f) {
		return create<Cs...>(EntityID{0}, std::forward<F>(f));
	}

	template <typename... Cs>
	inline EntityID create(EntityID parentID) {
		return create<Cs...>(parentID, [](Cs&...){});
	}

	template <typename... Cs>
	inline EntityID create() {
		return create<Cs...>([](Cs&...){});
	}

	template <typename... Cs>
	inline EntityID create_insert(EntityID parentID, Cs&&... cs) {
		auto id = (parentID << 32) + (++nextID & 0xFFFFFFFF);
		entities.emplace(id, make_bitset<Cs::ID...>());
		(storage<typename Cs::type>(Cs::ID).insert(id, std::forward<Cs>(cs)), ...);
		return id;
	}

	template <typename Head, typename... Cs,
			typename = std::enable_if_t<!std::is_same_v<std::decay_t<Head>, EntityID>>>
	inline EntityID create_insert(Head&& head, Cs&&... cs) {
		return create_insert(EntityID{0}, std::forward<Head>(head), std::forward<Cs>(cs)...);
	}

public: // component -------------------------------------------------------------------------------
	template <typename Cs, typename... Args>
	Cs& emplaceComponent(EntityID owner, Args&&... args) {
		const auto entityit = entities.find(owner);
		if (entityit != entities.end())
			entityit->components.set(Cs::ID);

		return storage<Cs>(Cs::ID).emplace(owner, std::forward<Args>(args)...);
	}

	template <typename C>
	C& insertComponent(EntityID owner, C&& component) {
		const auto entityit = entities.find(owner);
		if (entityit != entities.end())
			entityit->components.set(C::ID);

		return storage<typename C::type>(C::ID).insert(owner, std::forward<C>(component));
	}

public: // foreach ---------------------------------------------------------------------------------
	template <typename... Cs, typename F>
	void foreach(F&& f) {
		const auto requiredBits = make_required_bitset<Cs...>();

		[&](auto&&... creepers) {
			for (const auto& entity : entities)
				if ((entity.components & requiredBits) == requiredBits)
					f(creepers(entity.id)...);
		}(storage<typename Cs::type>(Cs::ID).template creeper<Cs>()...);
	}

	template <typename... Cs, typename F>
	void foreach_entity(F&& f) {
		const auto requiredBits = make_required_bitset<Cs...>();

		[&](auto&&... creepers) {
			for (const auto& entity : entities)
				if ((entity.components & requiredBits) == requiredBits)
					f(entity, creepers(entity.id)...);
		}(storage<typename Cs::type>(Cs::ID).template creeper<Cs>()...);
	}

	template <typename... Cs, typename F>
	void foreach_children(EntityID parentID, F&& f) {
		const auto requiredBits = make_required_bitset<Cs...>();

		auto children_itpair = std::equal_range(entities.begin(), entities.end(), parentID & 0xFFFFFFFF, ChildOfCmp());
		auto children = ranges::make_subrange(children_itpair.first, children_itpair.second);
		// Note: Remove children_itpair after range concept and boost flat_set can play nicely, in favour of:
		// auto children = ranges::equal_range(entities, parentID & 0xFFFFFFFF, ChildOfCmp());

		[&](auto&&... creepers) {
			for (const auto& entity : children)
				if ((entity.components & requiredBits) == requiredBits)
					f(entity, creepers(entity.id)...);
		}(storage<typename Cs::type>(Cs::ID).template creeper<Cs>()...);
	}

	template <typename Cs, typename F>
	void foreach_component(F&& f) {
		storage<typename Cs::type>(Cs::ID).foreach(std::forward<F>(f));
	}

public: // get -------------------------------------------------------------------------------------
	template <typename... Cs, typename F>
	void get(EntityID id, F&& f) {
		const auto entityit = entities.find(id);
		const auto requiredBits = make_required_bitset<Cs...>();

		if (entityit != entities.end())
			if ((entityit->components & requiredBits) == requiredBits)
				std::forward<F>(f)(storage<typename Cs::type>(Cs::ID).template get<Cs>(id)...);
	}

public: // -----------------------------------------------------------------------------------------
	std::size_t entityCount() {
		return entities.size();
	}

	template <typename C>
	std::size_t componentCount() {
		return storage<typename C::type>(C::ID).size();
	}

	void clear() {
		entities.clear();
		for (auto& storage : storages)
			storage.reset();
		nextID = 0;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv
