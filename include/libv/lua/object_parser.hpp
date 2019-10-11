// File:   object_grabber.hpp Author: Vader Created on 27 April 2019, 02:20

#pragma once

// ext
#include <sol/object.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>
// libv
#include <libv/meta/lnv.hpp>
#include <libv/utility/concat.hpp>
// std
#include <string_view>
#include <tuple>
#include <type_traits>


// TODO P4: Design not that clean, some restructure could help, details unknown
// TODO P5: Generalize variable/matcher/expression concept to be interchangeable (if implemented correctly could allow or_ and any_of to be the same)
// TODO P5: Tables should handle members without a keys for constants and generators
// TODO P5: Ability to name pattern and better name concatenation, maybe stream

namespace libv {
namespace lua {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename CppT, typename LuaT, const char* LuaTypeName, sol::type LuaTypeEnum, bool CheckIsType, typename Reporter>
inline auto primitive_eval(const sol::object& var, Reporter&& reporter) {
	if constexpr (std::is_void_v<CppT>) {
		if (var.get_type() == LuaTypeEnum)
			return true;

		reporter.lua_type_mismatch(LuaTypeName, LuaTypeEnum, var.get_type());
		return false;
	} else {
		if (var.get_type() == LuaTypeEnum) {
			if constexpr (std::is_void_v<LuaT>)
				return std::optional<CppT>(std::in_place);
			else
				if constexpr (CheckIsType)
					if (var.is<LuaT>())
						return std::optional<CppT>(std::in_place, var.as<LuaT>());
					else
						return std::optional<CppT>(std::nullopt);
				else
					return std::optional<CppT>(std::in_place, var.as<LuaT>());
		}

		reporter.lua_type_mismatch(LuaTypeName, LuaTypeEnum, var.get_type());
		return std::optional<CppT>{std::nullopt};
	}
}

template <typename CppT, typename LuaT, const char* LuaTypeName, sol::type LuaTypeEnum, bool CheckIsType>
struct PrimitiveType {
	using CppType = CppT;
	using EvalType = std::conditional_t<std::is_void_v<CppType>, bool, std::optional<CppType>>;

	inline auto name() const noexcept {
		return LuaTypeName;
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		return primitive_eval<CppType, LuaT, LuaTypeName, LuaTypeEnum, CheckIsType>(var, reporter);
	}
};

// -------------------------------------------------------------------------------------------------

constexpr inline char LuaTypeNameNil          [] = "Nil";
constexpr inline char LuaTypeNameBoolean      [] = "Boolean";
constexpr inline char LuaTypeNameNumber       [] = "Number";
constexpr inline char LuaTypeNameString       [] = "String";
constexpr inline char LuaTypeNameFunction     [] = "Function";
constexpr inline char LuaTypeNameTable        [] = "Table";
constexpr inline char LuaTypeNameThread       [] = "Thread";
constexpr inline char LuaTypeNameUserdata     [] = "Userdata";
constexpr inline char LuaTypeNameLightuserdata[] = "Lightuserdata";

template <typename CppT = sol::nil_t>
struct Nil : PrimitiveType<CppT, sol::nil_t, LuaTypeNameNil, sol::type::nil, false> {};

template <typename CppT = bool>
struct Boolean : PrimitiveType<CppT, bool, LuaTypeNameBoolean, sol::type::boolean, false> {};

template <typename CppT = double>
struct Number : PrimitiveType<CppT, double, LuaTypeNameNumber, sol::type::number, false> {};

template <typename CppT = std::string_view>
struct String : PrimitiveType<CppT, std::string_view, LuaTypeNameString, sol::type::string, false> {};

template <typename CppT, typename LuaT = CppT>
struct Function : PrimitiveType<CppT, LuaT, LuaTypeNameFunction, sol::type::function, false> {};

template <typename CppT, typename LuaT = CppT>
struct Thread : PrimitiveType<CppT, LuaT, LuaTypeNameThread, sol::type::thread, false> {};

template <typename CppT>
struct Userdata : PrimitiveType<CppT, CppT, LuaTypeNameUserdata, sol::type::userdata, true> {};

template <typename CppT>
struct Lightuserdata : PrimitiveType<CppT, CppT, LuaTypeNameLightuserdata, sol::type::lightuserdata, true> {};

// -------------------------------------------------------------------------------------------------

template <typename Key, typename Matcher>
struct Member {
	Key key;
	Matcher matcher;
};

template <typename CppT, typename... MemberKeyMatchers>
struct Table {
	using CppType = CppT;
	using EvalType = std::conditional_t<std::is_void_v<CppType>, bool, std::optional<CppType>>;

	std::tuple<MemberKeyMatchers...> members;

	inline auto name() const noexcept {
		return LuaTypeNameTable;
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		const auto table = primitive_eval<sol::table, sol::table, LuaTypeNameTable, sol::type::table, false>(var, reporter);

		if constexpr (std::is_void_v<CppType>) {
			return table && std::apply([&table, &reporter](auto&... m) {
				return (m.matcher.eval(table->template get<sol::object>(m.key), reporter) && ...);
			}, members);

		} else {
			if (not table)
				// No need to report if its not a table (its already reported by primitive_eval)
				return EvalType(std::nullopt);

			return std::apply([&table, &reporter](const auto&... m) {
				return [](auto&&... member_results) {
					if ((member_results && ...)) {
//						return EvalType(std::in_place, member_results.value()...);
						return EvalType(CppType{member_results.value()...});
					} else {
						// TODO P5: report missing / failed members
						return EvalType(std::nullopt);
					}
				}(m.matcher.eval(table->template get<sol::object>(m.key), reporter)...);
			}, members);
		}
	}
};

// -------------------------------------------------------------------------------------------------

template <typename CppT, typename Matcher>
struct MatcherAs : Matcher {
	using CppType = CppT;
	using EvalType = std::conditional_t<std::is_void_v<CppType>, bool, std::optional<CppType>>;

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		auto result = Matcher::eval(var, reporter);

		if constexpr (std::is_void_v<CppT>) {
			return result;
		} else {
			std::optional<CppT> resultCpp(std::nullopt);
			if (result) {
				if constexpr (std::is_enum_v<CppT>)
					resultCpp.emplace(CppT(result.value()));
				else
					resultCpp.emplace(result.value());
			}
			return resultCpp;
		}
	}
};

template <typename Matcher>
struct MatcherOr : Matcher {
	using CppType = typename Matcher::CppType;
	using EvalType = typename Matcher::EvalType;

	typename Matcher::CppType fallback;

	inline auto name() const noexcept {
		return libv::concat("[", Matcher::name(), "]");
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		auto result = Matcher::eval(var, reporter);
		return result ? result : std::optional<typename Matcher::CppType>(fallback);
	}
};

template <typename CppT, typename... Matchers>
struct MatcherOneOf {
	using CppType = CppT;
	using EvalType = std::conditional_t<std::is_void_v<CppType>, bool, std::optional<CppType>>;

	std::tuple<Matchers...> matchers;

	inline auto name() const noexcept {
		// TODO P5: Better name for any
		return libv::concat(Matchers::name()...);
	}

	template <typename Reporter>
	inline auto impl(const sol::object& var, Reporter&& reporter) const {
		(void) var;
		(void) reporter; // TODO P5: Report

		if constexpr (std::is_void_v<CppType>)
			return false;
		else
			return EvalType(std::nullopt);
	}

	template <typename Reporter, typename Head, typename... Tail>
	inline auto impl(const sol::object& var, Reporter&& reporter, const Head& head, const Tail&... tail) const {
		(void) reporter; // TODO P5: Report

		auto result = head.eval(var, reporter);
		if (result)
			return result;
		else
			return impl(var, reporter, tail...);
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		return std::apply([this, &var, &reporter](const auto&... m){
			return impl(var, reporter, m...);
		}, matchers);
	}
};

template <typename Matcher, typename F>
struct MatcherTransform : Matcher {
	using CppType = std::invoke_result_t<F, libv::meta::lnv_t<typename Matcher::CppType, bool>>;
	using EvalType = std::conditional_t<std::is_void_v<CppType>, bool, std::optional<CppType>>;

	F func;

	inline auto name() const noexcept {
		return libv::concat(Matcher::name(), '\'');
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		auto result = Matcher::eval(var, reporter);

		constexpr bool void_in = std::is_void_v<typename Matcher::CppType>;
		constexpr bool void_out = std::is_void_v<CppType>;

		if constexpr (void_in) {
			if constexpr (void_out) {
				func(result);
				return result;

			} else {
				return func(result);
			}

		} else {
			if constexpr (void_out) {
				if (result) {
					func(*result);
					return result.has_value();
				} else {
					func(*result);
					return false;
				}
			} else {
				if (result)
					return EvalType(func(*result));
				else
					return EvalType(std::nullopt);
			}
		}
	}
};

template <typename F>
struct MatcherParser : String<> {
	// Function return type expected to be an optional
	using CppType = std::invoke_result_t<F, const std::string_view>::value_type;
	using EvalType = std::optional<CppType>;

	F func;

	inline auto name() const noexcept {
		return libv::concat("Parsed ", String::name());
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		auto result = String<>::eval(var, reporter);

		if (result)
			return func(*result);
		else
			return EvalType{std::nullopt};
	}
};

template <typename CppT>
struct MatcherConstant {
	using CppType = CppT;
	using EvalType = std::optional<CppType>;

	CppType contant;

	inline auto name() const noexcept {
		return "Constant";
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		(void) var;
		(void) reporter;

		return EvalType(contant);
	}
};

template <typename F>
struct MatcherGenerate {
	using CppType = std::invoke_result_t<F>;
	using EvalType = std::optional<CppType>;

	F func;

	inline auto name() const noexcept {
		return "Generated";
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter&& reporter) const {
		(void) var;
		(void) reporter;

		return EvalType(func());
	}
};

} // namespace detail

// --- Primitive -----------------------------------------------------------------------------------

template <typename CppT = sol::nil_t>
constexpr inline auto nil() noexcept {
	return detail::Nil<CppT>();
}

template <typename CppT = bool>
constexpr inline auto boolean() noexcept {
	return detail::Boolean<CppT>();
}

template <typename CppT = double>
constexpr inline auto number() noexcept {
	return detail::Number<CppT>();
}

template <typename CppT = std::string_view>
constexpr inline auto string() noexcept {
	return detail::String<CppT>();
}

template <typename CppT, typename LuaT = CppT>
constexpr inline auto function() noexcept {
	return detail::Function<CppT, LuaT>();
}

template <typename CppT, typename LuaT = CppT>
constexpr inline auto thread() noexcept {
	return detail::Thread<CppT, LuaT>();
}

template <typename CppT>
constexpr inline auto userdata() noexcept {
	return detail::Userdata<CppT>();
}

template <typename CppT>
constexpr inline auto lightuserdata() noexcept {
	return detail::Lightuserdata<CppT>();
}

// --- Table ---------------------------------------------------------------------------------------

template <typename Key, typename Matcher>
constexpr inline auto member(Key&& key, Matcher&& matcher) noexcept {
	return detail::Member<std::decay_t<Key>, std::decay_t<Matcher>>{std::forward<Key>(key), std::forward<Matcher>(matcher)};
}

template <typename CppT, typename... Members>
constexpr inline auto table(Members&&... members) noexcept {
	return detail::Table<CppT, std::decay_t<Members>...>{{std::forward<Members>(members)...}};
}

// --- Container -----------------------------------------------------------------------------------

template <template <typename...> typename Map, typename Matcher>
constexpr inline auto map(Matcher&& matcher) noexcept {
	// Map<std::string, typename Matcher::CppType>
	return;
}

template <template <typename...> typename Vector, typename Member>
constexpr inline auto vector(Member&& member) noexcept {
	// Vector<typename Matcher::CppType>
	return;
}

// --- Optional / Variant --------------------------------------------------------------------------

template <typename Matcher>
constexpr inline auto optional(Matcher&& matcher) noexcept {
	// std::optional<typename Matcher::CppType>
	return;
}

template <typename... Matchers>
constexpr inline auto variant(Matchers&&... matchers) noexcept {
	// std::variant<typename Matchers::CppType...>
	return;
}

// --- Value ---------------------------------------------------------------------------------------

template <typename T>
constexpr inline auto constant(T&& func) noexcept {
	return detail::MatcherConstant<std::remove_cvref_t<T>>{std::forward<T>(func)};
}

template <typename F>
constexpr inline auto generate(F&& func) noexcept {
	return detail::MatcherGenerate<std::remove_cvref_t<F>>{std::forward<F>(func)};
}

// --- Adaptors ------------------------------------------------------------------------------------

template <typename CppT, typename Matcher>
constexpr inline auto as(Matcher&& matcher) noexcept {
	if constexpr (std::is_same_v<CppT, typename Matcher::CppType>)
		return std::forward<Matcher>(matcher);
	else
		return detail::MatcherAs<CppT, std::remove_cvref_t<Matcher>>{std::forward<Matcher>(matcher)};
}

template <typename Matcher>
constexpr inline auto or_(Matcher&& matcher, typename Matcher::CppType fallback) noexcept {
	return detail::MatcherOr<std::remove_cvref_t<Matcher>>{
			std::forward<Matcher>(matcher),
			std::move(fallback)
	};
}

template <typename HeadMatcher, typename... Matchers>
constexpr inline auto one_of(HeadMatcher&& head, Matchers&&... matchers) noexcept {
	static_assert((std::is_same_v<typename HeadMatcher::CppType, typename Matchers::CppType> && ...),
			"Matchers of one_of has to have the same result type");

	return detail::MatcherOneOf<typename HeadMatcher::CppType, std::remove_cvref_t<HeadMatcher>, std::remove_cvref_t<Matchers>...>{
		{std::forward<HeadMatcher>(head), std::forward<Matchers>(matchers)...}
	};
}

template <typename Matcher, typename F>
constexpr inline auto transform(Matcher&& matcher, F&& func) noexcept {
	return detail::MatcherTransform<std::remove_cvref_t<Matcher>, std::remove_cvref_t<F>>{
			{std::forward<Matcher>(matcher)},
			std::forward<F>(func)
	};
}

template <typename F>
constexpr inline auto string_parse(F&& func) noexcept {
	return detail::MatcherParser<std::remove_cvref_t<F>>{{}, std::forward<F>(func)};
}

template <typename F>
constexpr inline auto string_accept(F&& func) noexcept {
	return transform(string(), std::forward<F>(func));
}

// -------------------------------------------------------------------------------------------------

struct ReportNoop {
	inline void lua_type_mismatch(const std::string_view name, const sol::type found, const sol::type expected) noexcept {
		(void) name;
		(void) found;
		(void) expected;
	}
	inline void string_parse_failed(const std::string_view name, const std::string_view value) noexcept {
		(void) name;
		(void) value;
	}
	inline auto reason() const {
		return "";
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Parser, typename Reporter = ReportNoop>
inline auto parse(const sol::object& var, Parser&& parser, Reporter&& reporter = Reporter{}) {
	return parser.eval(var, reporter);
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
