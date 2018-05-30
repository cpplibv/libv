// File: ecs.hpp Author: Vader Created on 2017. március 15., 3:05

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/meta/type_traits.hpp>
// std
#include <tuple>
#include <type_traits>
#include <utility>
// pro
#include <libv/ecs/common.hpp>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

struct ComponentStorageBase {
	virtual ~ComponentStorageBase() = default;
};

template <typename T>
struct ComponentStorage final : public ComponentStorageBase {
private:
	boost::container::flat_map<EntityID, T> storage;

public:
	inline auto size() {
		return storage.size();
	}

	inline T& operator[](EntityID id) {
		return storage[id];
	}

	template <typename K, typename = std::enable_if_t<std::is_same<T, K>::value>>
	inline T& insert(EntityID id, K&& value) {
		return storage.emplace(id, std::forward<K>(value)).first->second;
	}

	template <typename... Args>
	inline T& emplace(EntityID owner, Args&&... args) {
		return storage.emplace(std::piecewise_construct,
				std::forward_as_tuple(owner),
				std::forward_as_tuple(std::forward<Args>(args)...)).first->second;
	}

	template <typename F>
	inline void foreach(F&& f) {
		for (auto& comp : storage)
			f(comp.second);
	}

	template <typename Component>
	inline decltype(auto) creeper() {
		if constexpr(Component::is_optional_v) {
			return creeper_optional();
		} else {
			return creeper_simple();
		}
	}

	template <typename Component>
	inline decltype(auto) get(EntityID id) {
		if constexpr(Component::is_optional_v) {
			return get_optional(id);
		} else {
			return get_simple(id);
		}
	}

private:
	auto creeper_simple() {
		// - The requested IDs are strictly monotonically increasing
		// - The requested ID will always be present in the container
		// - The most likely match will be the element right after the current one
		return [iter = storage.begin() - 1, end = storage.end()] (auto next) mutable -> T& {
			size_t jump = 1;
			++iter;
			while (iter->first != next) {
				auto guess = iter + jump;
				if (guess < end && guess->first <= next) {
					iter = guess;
					jump *= 2;
				} else {
					jump /= 2;
				}
			}
			return iter->second;
		};
	}

	auto creeper_optional() {
		// - The requested IDs are strictly monotonically increasing
		// - The most likely match will be the element right after the current one
		return [iter = storage.begin() - 1, end = storage.end()] (auto next) mutable -> T* {
			size_t jump = 1;
			while (true) {
				auto guess = iter + jump;

				if (guess < end && guess->first < next) {
					iter = guess;
					jump *= 2;
				} else if (guess < end && guess->first == next) {
					iter = guess;
					return &iter->second;
				} else {
					jump /= 2;
				}

				if (jump == 0)
					return nullptr;
			}
		};
	}

	auto& get_simple(EntityID id) {
		return storage.find(id)->second;
	}
	auto* get_optional(EntityID id) {
		const auto it = storage.find(id);
		return it != storage.end() ? &it->second : nullptr;
	}

};

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv
