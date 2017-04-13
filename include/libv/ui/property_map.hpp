// File: property_map.hpp Author: Vader Created on 2016. július 4., 10:36

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/container/multi_type_map.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Container = std::map>
class BasicPropertyMap {
	BasicMultiTypeMap<Container> map;
public:
	template <typename T>
	struct Key : BasicMultiTypeMap<Container>::template Key<T> {
	private:
		std::string name_;
		T defaultValue_;
		public:
		Key(std::string name, T defaultValue) :
			BasicMultiTypeMap<Container>::template Key<T>(),
			name_(name),
			defaultValue_(defaultValue) { }

		const std::string& name() const {
			return name_;
		}

		const T& defaultValue() const {
			return defaultValue_;
		}
	};

	// SetterProxy ---------------------------------------------------------------------------------
public:

	class SetterProxy {
		friend class BasicPropertyMap<Container>;
		BasicPropertyMap<Container>& object;
		SetterProxy(BasicPropertyMap<Container>& object) : object(object) { }
	public:
		template <typename T, typename... Args, typename = decltype(T(std::declval<Args>()...))>
		inline SetterProxy& operator()(const Key<T>& key, Args&&... args) {
			object.set(key, std::forward<Args>(args)...);
			return *this;
		}
	};

	// Setters -------------------------------------------------------------------------------------
	template <typename T, typename... Args, typename = decltype(T(std::declval<Args>()...))>
	inline void set(const Key<T>& key, Args&&... args) {
		auto it = map.find(key);
		if (it.is_initialized())
			it.value() = T(std::forward<Args>(args)...);
		else
			map.emplace(key, std::forward<Args>(args)...);
	}
	inline SetterProxy set() {
		return SetterProxy(*this);
	}

	// Getters -------------------------------------------------------------------------------------
	template <typename T> decltype(auto) get(const Key<T>& key) {
		return map.find(key);
	}
	template <typename T> decltype(auto) get(const Key<T>& key) const {
		return map.find(key);
	}
	template <typename T> const T& getOrUse(const Key<T>& key, const T& def) const {
		auto it = map.find(key);
		if (it.is_initialized())
			return it.value();
		else
			return def;
	}
	template <typename T> const T& getOrDefault(const Key<T>& key) const {
		auto it = map.find(key);
		if (it.is_initialized())
			return it.value();
		else
			return key.defaultValue();
	}

	// Modifiers -----------------------------------------------------------------------------------
	template <typename T> void remove(const Key<T>& key) {
		map.erase(key);
	}
};

using PropertyMap = BasicPropertyMap<>;
template <typename T>
using PropertyAddress = PropertyMap::Key<T>;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv