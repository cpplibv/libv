// Project: libv.ui, File: src/libv/ui/property_access_context.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
// std
#include <string_view>
#include <type_traits>
// pro
//#include <libv/ui/core_component.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;

class CoreComponent;

//struct BasePropertyInfo {
//	std::string group;
//	std::string name;
//	std::string description;
//
//	virtual std::shared_ptr<CoreComponent> makeEditor() = 0;
//	virtual std::string toString() = 0;
//	virtual ~BasePropertyInfo() = default;
//};
//
//template <typename T, typename Set, typename Get>
//struct PropertyInfo : BasePropertyInfo {
////	owner Property
//	Set set;
//	Get get;
////	setter
////	getter
////	fallback
////	init
//
//	virtual std::shared_ptr<CoreComponent> makeEditor() override { }
//	virtual std::string toString() override { }
//};

//struct ComponentPropertyDescription {
//	std::vector<std::unique_ptr<BasePropertyInfo>> properties;
//};

// -------------------------------------------------------------------------------------------------

/// Style setter
template <typename Owner>
struct PropertyAccessContext {
	Owner& owner;
	CoreComponent& component;
	libv::observer_ptr<Style> style;
	ContextUI& context;

private:
	template <typename Access>
	using access_value_type_t = typename std::remove_cvref_t<decltype(std::declval<const Access&>()(std::declval<Owner&>()))>::value_type;
	template <typename Get>
	using get_value_type_t = std::remove_cvref_t<decltype(std::declval<const Get&>()(std::declval<Owner&>()))>;

//	template <typename Access>
//	static constexpr bool is_access_v = false;
//	template <typename Access>
//	static constexpr bool is_init_funciton_v = false;
//	template <typename Access>
//	static constexpr bool is_set_v = false;
//	template <typename Access>
//	static constexpr bool is_get_v = false;

public:
	template <typename Access>
	void property(Access&& access, access_value_type_t<Access> init, std::string_view group, std::string_view name, std::string_view description) {
//		static_assert(Access yields PropertyA);
//		static_assert(Init can be assigned to property);

		(void) group;
		(void) description;

		using value_type = access_value_type_t<Access>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(name);
			if (value_opt) {
				if (*value_opt != property())
					AccessProperty::value(component, property, *value_opt);
				return;
			}
		}

		if (init != property())
			AccessProperty::value(component, property, std::move(init));
	}

	template <typename Access, typename Init>
			WISH_REQUIRES(std::is_invocable_v<Init, ContextUI&>)
	void property(Access&& access, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
//		static_assert(Access yields PropertyA);
//		static_assert(Init invoke with ContextUI& result can be assigned to property);
		static_assert(std::is_convertible_v<std::invoke_result_t<Init, ContextUI&>, access_value_type_t<Access>>, "Init function object has incorrect return type");

		(void) group;
		(void) description;

		using value_type = access_value_type_t<Access>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(name);
			if (value_opt) {
				if (*value_opt != property())
					AccessProperty::value(component, property, *value_opt);
				return;
			}
		}

		auto value = init(context);
		if (value != property())
			AccessProperty::value(component, property, std::move(value));
	}

	template <typename Access, typename Set, typename Get, typename Init>
	void indirect(Access&& access, Set&& set, Get&& get, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
//		static_assert(Access yields Property<void>);
//		static_assert(Set is callable with invoke result of Get + driver);
//		static_assert(Get yields a non void);
//		static_assert(Init can be passed to Set);

		(void) group;
		(void) description;

		using value_type = get_value_type_t<Get>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(name);
			if (value_opt) {
				if (*value_opt != get(owner))
					set(owner, *value_opt);
				return;
			}
		}

		if (init != get(owner))
			set(owner, std::move(init));
	}

	template <typename Access, typename Set, typename Get, typename Init>
			WISH_REQUIRES(std::is_invocable_v<Init, ContextUI&>)
	void indirect(Access&& access, Set&& set, Get&& get, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
//		static_assert(Access yields Property<void>);
//		static_assert(Set is callable with invoke result of Get + driver);
//		static_assert(Get yields a non void);
//		static_assert(Init invoke with ContextUI& result can be passed to Set);

		(void) group;
		(void) description;

		using value_type = get_value_type_t<Get>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(name);
			if (value_opt) {
				if (*value_opt != get(owner))
					set(owner, *value_opt);
				return;
			}
		}

		auto value = init(context);
		if (value != get(owner))
			set(owner, std::move(value));
	}

	template <typename Set, typename Get>
	void synthetize(Set&& set, Get&& get, std::string_view group, std::string_view name, std::string_view description) {
//		static_assert(Set is callable with invoke result of Get);
//		static_assert(Get yields a non void);

		(void) set;
		(void) get;
		(void) group;
		(void) name;
		(void) description;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
