// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/observer_ptr.hpp>
//#include <libv/utility/observer_ref.hpp>
#include <libv/utility/intrusive_ptr.hpp>
// std
#include <stdexcept>
#include <string>
#include <string_view>
// pro
#include <libv/ui/property.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Style : public libv::intrusive_base_unsafe<Style> {
private:
	std::vector<libv::intrusive_ptr<Style>> parents;
//	std::unordered_set<libv::observer_ref<ComponentBase>> users;
//	boost::container::flat_map<std::string, PropertyDynamic, std::less<>> properties;
//	boost::container::flat_set<PropertyDynamic, libv::variant_index> properties;
	boost::container::flat_map<std::string, PropertyDynamic, std::less<>> properties;

public:
	const std::string style_name;

public:
	explicit Style(std::string style_name) : style_name(std::move(style_name)) { }

public:
	void inherit(const libv::intrusive_ptr<Style>& base) {
		// TODO P5: detect cyclic dependency attempts
		parents.emplace_back(base);
	}

public:
	void set(std::string property, PropertyDynamic value) {
		properties.emplace(std::move(property), std::move(value));
	}

	template <typename T>
	inline void set(std::string property, T&& value) {
		set(std::move(property), PropertyDynamic{std::forward<T>(value)});
	}

public:
	template <typename F>
	inline void foreach(F&& func) {
		for (const auto& [key, value] : properties)
			func(key, value);
	}

private:
	[[nodiscard]] libv::observer_ptr<const PropertyDynamic> get_optional_variant(const std::string_view property) const {
		libv::observer_ptr<const PropertyDynamic> result;

		const auto it = properties.find(property);
		if (it != properties.end()) {
			result = libv::make_observer(it->second);
		} else {
			for (const auto& parent : parents) {
				result = parent->get_optional_variant(property);
				if (result)
					break;
			}
		}

		return result;
	}

public:
	template <typename T = void>
	[[nodiscard]] auto get_optional(const std::string_view property) const {
		if constexpr (std::is_same_v<T, void>) {
			return get_optional_variant(property);
		} else {
			auto result = get_optional(property);
			return result && !std::holds_alternative<T>(*result) ? nullptr : result;
		}
	}

	[[nodiscard]] const PropertyDynamic& get_or_throw(const std::string_view property) const {
		const auto result = get_optional(property);
		if (!result)
			throw std::invalid_argument(libv::concat("Requested property \"", property, "\" is not found"));
		return *result;
	}

	template <typename T>
	[[nodiscard]] const T& get_or_throw(const std::string_view property) const {
		auto result = get_optional(property);

		if (!result)
			throw std::invalid_argument(libv::concat("Requested property \"", property, "\" is not found"));

		if (!std::holds_alternative<T>(*result))
			throw std::invalid_argument(libv::concat("Requested property \"", property, "\" has different type"));

		return *result;
	}
};



// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
