// Project: libv.ecs, File: src/libv/ecs/storage.hpp

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
	// TODO P5: libv.ecs Optimization: split storage into 2 container, (implement a column sequential container...)
	//			libv::segmented_row_set<EntityID, T> storage;
	//			libv::segmented_cs_map<EntityID, T> storage; // cs = column sequential

public:
	inline auto size() const noexcept {
		return storage.size();
	}

	inline T& operator[](EntityID id) noexcept {
		return storage[id];
	}

	inline const T& operator[](EntityID id) const noexcept {
		return storage[id];
	}

	template <typename K, typename = std::enable_if_t<std::is_same<T, K>::value>>
	inline T& insert(EntityID id, K&& value) noexcept {
		return storage.emplace(id, std::forward<K>(value)).first->second;
	}

	template <typename... Args>
	inline T& emplace(EntityID owner, Args&&... args) noexcept {
		return storage.emplace(std::piecewise_construct,
				std::forward_as_tuple(owner),
				std::forward_as_tuple(std::forward<Args>(args)...)).first->second;
	}

	template <typename F>
	inline void foreach(F&& f) noexcept {
		for (auto& comp : storage)
			f(comp.second);
	}

	template <typename Component>
	inline decltype(auto) creeper() noexcept {
		if constexpr(Component::is_optional_v) {
			return creeper_optional();
		} else {
			return creeper_simple();
		}
	}

	template <typename Component>
	inline decltype(auto) get(EntityID id) noexcept {
		if constexpr(Component::is_optional_v) {
			return get_optional(id);
		} else {
			return get_simple(id);
		}
	}

private:
	inline auto creeper_simple() noexcept {
		// - The requested IDs are strictly monotonically increasing
		// - The requested ID will always be present in the container
		// - The most likely match will be the element right after the current one

		// TODO P4: storage.begin() - 1 could fail with assert if storage is empty, bypassed with empty check, refactor to avoid the need of -1 start
		//		return [iter = storage.begin() - 1, end = storage.end()] (auto next) mutable -> T& {
		return [iter = (storage.empty() ? storage.begin() : storage.begin() - 1), end = storage.end()] (auto next) mutable -> T& {
			std::size_t jump = 1;
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

	inline auto creeper_optional() noexcept {
		// - The requested IDs are strictly monotonically increasing
		// - The most likely match will be the element right after the current one

		// TODO P4: storage.begin() - 1 could fail with assert if storage is empty, bypassed with empty check, refactor to avoid the need of -1 start
		//		return [iter = storage.begin() - 1, end = storage.end()] (auto next) mutable -> T* {
		return [iter = (storage.empty() ? storage.begin() : storage.begin() - 1), end = storage.end()] (auto next) mutable -> T* {
			std::size_t jump = 1;
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

	inline auto& get_simple(EntityID id) noexcept {
		return storage.find(id)->second;
	}

	inline auto* get_optional(EntityID id) noexcept {
		const auto it = storage.find(id);
		return it != storage.end() ? &it->second : nullptr;
	}

};

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv
