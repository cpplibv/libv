// Project: libv.ui, File: src/libv/ui/property_system/property_access.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// std
#include <string_view>
#include <type_traits>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component_system/core_ptr.hpp>
#include <libv/ui/style/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename CoreOwner>
class PropertyAccess {
	CoreOwner& owner;
	libv::observer_ptr<Style> style;

public:
	explicit constexpr LIBV_FORCE_INLINE PropertyAccess(CoreOwner& owner) :
		owner(owner),
		style(libv::make_observer_ptr(owner.style().get())) {
	}

private:
	template <typename Access>
	using access_value_type_t = typename std::remove_cvref_t<decltype(std::declval<const Access&>()(std::declval<CoreOwner&>()))>::value_type;
	template <typename Get>
	using get_value_type_t = std::remove_cvref_t<decltype(std::declval<const Get&>()(std::declval<CoreOwner&>()))>;

	// using handler_t = decltype(std::declval<CoreOwner>().handler());
	//
	// template <typename Access>
	// static constexpr bool is_access_v = false;
	// template <typename Access>
	// static constexpr bool is_init_funciton_v = false;
	// template <typename Access>
	// static constexpr bool is_set_v = false;
	// template <typename Access>
	// static constexpr bool is_get_v = false;

public:
	template <typename C, typename T>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE auto handler_setter(void(C::*MemberFunc)(T)) noexcept {
		return [MemberFunc](CoreOwner& c, auto v) {
			(C{&c}.*MemberFunc)(std::move(v));
		};
	}
	template <typename C, typename T>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE auto handler_getter(T(C::*MemberFunc)() const) noexcept {
		return [MemberFunc](CoreOwner& c) {
			return (C{&c}.*MemberFunc)();
		};
	}
	// template <typename C, typename T>
	// [[nodiscard]] static constexpr LIBV_FORCE_INLINE auto core_property(T(C::*Member)() const) noexcept {
	// 	return [Member](CoreOwner& c) -> auto& {
	// 		return (C{&c}.*Member)();
	// 	};
	// }

public:
	template <typename Access>
	void property(Access&& access, access_value_type_t<Access> init, std::string_view group, std::string_view name, std::string_view description) {
		// static_assert(Access yields PropertyA);
		// static_assert(Init can be assigned to property);

		(void) group;
		(void) description;

		using value_type = access_value_type_t<Access>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(owner.style_state(), name);
			if (value_opt) {
				AccessProperty::value(owner, property, *value_opt);
				return;
			}
		}

		AccessProperty::value(owner, property, std::move(init));
	}

	template <typename Access, typename Init>
			requires (std::is_invocable_v<Init, ContextUI&>)
	void property(Access&& access, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
		// static_assert(Access yields PropertyA);
		// static_assert(Init invoke with ContextUI& result can be assigned to property);
		static_assert(std::is_convertible_v<std::invoke_result_t<Init, ContextUI&>, access_value_type_t<Access>>, "Init function object has incorrect return type");

		(void) group;
		(void) description;

		using value_type = access_value_type_t<Access>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(owner.style_state(), name);
			if (value_opt) {
				AccessProperty::value(owner, property, *value_opt);
				return;
			}
		}

		auto value = init(owner.context());
		AccessProperty::value(owner, property, std::move(value));
	}

	template <typename Access, typename Set, typename Get, typename Init>
	void indirect(Access&& access, Set&& set, Get&& get, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
		// static_assert(Access yields Property<void|T>);
		// static_assert(Set is callable with invoke result of Get + driver);
		// static_assert(Get yields a non void);
		// static_assert(Init can be passed to Set);

		(void) group;
		(void) description;

		using value_type = get_value_type_t<Get>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(owner.style_state(), name);
			if (value_opt) {
				if (*value_opt != get(owner)) {
					set(owner, *value_opt);
					AccessProperty::changed(owner, property);
				}
				return;
			}
		}

		if (init != get(owner)) {
			set(owner, std::move(init));
			AccessProperty::changed(owner, property);
		}
	}

	template <typename Access, typename Set, typename Get, typename Init>
			requires (std::is_invocable_v<Init, ContextUI&>)
	void indirect(Access&& access, Set&& set, Get&& get, Init&& init, std::string_view group, std::string_view name, std::string_view description) {
		// static_assert(Access yields Property<void|T>);
		// static_assert(Set is callable with invoke result of Get + driver);
		// static_assert(Get yields a non void);
		// static_assert(Init invoke with ContextUI& result can be passed to Set);

		(void) group;
		(void) description;

		using value_type = get_value_type_t<Get>;
		auto& property = access(owner);

		if (AccessProperty::driver(property) != PropertyDriver::style)
			return;

		if (style != nullptr) {
			const value_type* value_opt = style->get_optional<value_type>(owner.style_state(), name);
			if (value_opt) {
				if (*value_opt != get(owner)) {
					set(owner, *value_opt);
					AccessProperty::changed(owner, property);
				}
				return;
			}
		}

		auto value = init(owner.context());
		if (value != get(owner)) {
			set(owner, std::move(value));
			AccessProperty::changed(owner, property);
		}
	}

	template <typename Set, typename Get>
	void synthesize(Set&& set, Get&& get, std::string_view group, std::string_view name, std::string_view description) {
		// static_assert(Set is callable with invoke result of Get);
		// static_assert(Get yields a non void);

		(void) set;
		(void) get;
		(void) group;
		(void) name;
		(void) description;
	}
};

// -------------------------------------------------------------------------------------------------

void assign_nested_style(CoreComponent& component, std::string_view nesting_name);

// -------------------------------------------------------------------------------------------------

class StyleAccess {
public:
	enum class AccessModeSelf {
		style,
		noop,
	};
	enum class AccessModeChild {
		all,
		dirty,
		none,
	};

private:
	AccessModeSelf mode_self;
	AccessModeChild mode_child;
	std::string_view nesting_name; // For non-nested access nesting_name is empty
	StyleAccess* nesting_parent;

public:
	explicit LIBV_FORCE_INLINE StyleAccess(
			AccessModeSelf mode_self,
			AccessModeChild mode_child,
			std::string_view nesting_name = "",
			StyleAccess* nesting_parent = nullptr) :
		mode_self(mode_self),
		mode_child(mode_child),
		nesting_name(nesting_name),
		nesting_parent(nesting_parent) {}

public:
	template <typename Self>
	inline void self(Self& self) {

		if constexpr (requires { typename Self::base_type; } )
			// Base call has to be unconditional as bases could have children on nested components
			self.Self::base_type::doStyle(*this);
		else
			static_assert(std::is_same_v<Self, CoreComponent>, "Every core type must have a base_type type alias");

		if (mode_self == AccessModeSelf::noop)
			return;

		PropertyAccess<Self> pa{self};
		Self::access_properties(pa);
	}

	template <typename Nested>
	inline void nested(Nested& nested, std::string_view nesting_name) {
		switch (mode_child) {
		case AccessModeChild::none:
			break;

		case AccessModeChild::dirty:
			assign_nested_style(ref_core(nested), nesting_name);
			AccessRoot::styleScan(ref_core(nested));
			break;

		case AccessModeChild::all:
			assign_nested_style(ref_core(nested), nesting_name);
			AccessRoot::styleScanAll(ref_core(nested));
			break;
		}
	}

	inline void child(Component& child) {
		switch (mode_child) {
		case AccessModeChild::none:
			break;

		case AccessModeChild::dirty:
			AccessRoot::styleScan(ref_core(child));
			break;

		case AccessModeChild::all:
			AccessRoot::styleScanAll(ref_core(child));
			break;
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
