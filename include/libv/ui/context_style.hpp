// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
// std
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;

class ContextStyle {
public:
	libv::observer_ptr<Style> style;
	const Style& fallback;
	BaseComponent& component;

	// set one by name
	// get one by name
	// set all
	// iterate/get all
	// iterate description all
	// set one by "address"

private:
//	template <typename T, typename F>
//	void description(const std::string_view name, const T& property, Flag_t invalidate, const F& fallback, const std::string_view description) {
//		(void) invalidate;
//		(void) fallback;
//
//		// outside_function(name, fmt::format(outside_format, AccessProperty::value(property)), description);
//
//		(void) name;
//		(void) property;
//		(void) description;
//	}

public:
	template <typename Property>
			WISH_REQUIRES(C_Property<Property> || C_PropertySG<Property>)
	inline void operator()(Property& property, const std::string_view description) {
		(void) description;
		using value_type = typename Property::value_type;

		if (AccessProperty::manual(property))
			return;

		value_type value = [this]{
			if (style != nullptr) {
				const value_type* value_opt = style->get_optional<value_type>(Property::name);
				if (value_opt)
					return *value_opt;
			}

			return fallback.get_or_throw<value_type>(Property::name); // This throw could even be a log and terminate
		}();

		if constexpr (C_Property<Property>) {
			const bool change = value != AccessProperty::value(property);

			if (!change)
				return;

			AccessProperty::value(property, std::move(value));
			component.flagAuto(Property::invalidate);
		} else {
			const bool change = value != AccessProperty::value(component, property);

			if (!change)
				return;

			AccessProperty::value(component, property, std::move(value));
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
