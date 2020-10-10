// Project: libv.lua, File: src/libv/lua/object_parser.hpp, Author: Császár Mátyás [Vader]

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


// IDEA: Alternative imperative design idea #1: ---
//
//static inline libv::vec4f verify_color(sol::object object) {
//	bool success = false;
//	libv::vec4f result;
//
//	const auto type = object.get_type();
//
//	if (type == sol::type::string) {
//		auto r = libv::parse::parse_color_optional(object.as<std::string_view>());
//		if (!r)
//			error();
//		result = *r;
//
//		success = success || r;
//	}
//
//	if (type == sol::type::table) {
//		const auto table = sol::table(object);
//
//		success = success ||
//				try_number(result.x, table["x"]) &&
//						try_number(result.y, table["y"]) &&
//						try_number(result.z, table["z"]) &&
//						try_number(result.w, table["w"]);
//
//		success = success ||
//				try_number(result.x, table["r"]) &&
//						try_number(result.y, table["g"]) &&
//						try_number(result.z, table["b"]) &&
//						try_number(result.w, table["a"]);
//
//		success = success ||
//				try_number(result.x, table[0]) &&
//						try_number(result.y, table[1]) &&
//						try_number(result.z, table[2]) &&
//						try_number(result.w, table[3]);
//	}
//}
//
//
//
// IDEA: Alternative imperative design idea #2: ---
//
//template <typename Magic>
//static inline auto verify_color(sol::object object, Magic& magic) {
//	auto result = magic.result<libv::vec4f>();
//
//	magic.candidate_string_parse("Color string", &libv::parse::parse_color_optional);
//
//	magic.candidate_usertype<libv::vec4f>("vec4f");
//
//	magic.candidate_usertype<libv::vec3f>("vec3f", [](const auto& c, auto& r) {
//		r.x = c.x;
//		r.y = c.y;
//		r.z = c.z;
//		r.w = 1.0f;
//      magic.reject("reason");
//	});
//
//	magic.candidate_table("XYZ Table", [](auto c, auto& r) {
//		c["x"].number(r.x);
//		c["y"].number(r.y);
//		c["z"].number(r.z);
//		c["w"].number_or(r.w, 1.0f);
//		c.no_unused();
//	});
//
//	magic.candidate_table("RGB Table", [](auto c, auto& r) {
//		c["r"].number(r.x);
//		c["g"].number(r.y);
//		c["b"].number(r.z);
//		c["a"].number_or(r.w, 1.0f);
//		c.no_unused();
//	});
//
//	magic.candidate_table("Array Table", [](auto c, auto& r) {
//		c[0].number(r.x);
//		c[1].number(r.y);
//		c[2].number(r.z);
//		c[3].number_or(r.w, 1.0f);
//		c.no_unused();
//	});
//
//  magic.ensure([](auto c, const auto& r) {
//      if (isnan(r.x) || isnan(r.y) || isnan(r.z) || isnan(r.w))
//          c.reject("Color values cannot be NaN");
//  });
//
//	return magic.end(verify_color, result);
//}


// IDEA: Reporter output idea #1: ---
//
// Lua object does not match [TYPE]
// Candidate [string] rejected: [reason]
// Candidate [userdata] rejected: [reason]
// Candidate table [table name] rejected: [reason]
//		[member0 name]:[member0 type]
//		[member1 name]:[member1 type]
//		[member2 name]:[member2 type]
//		[member3 name]:[member3 type]
//		[no unused]
// Candidate table [table name] rejected: [reason]
//		[member0 name]:[member0 type]
//		[member1 name]:[optional][member1 type]
// Candidate table [table name] rejected: [reason]
//		[member0 name]:[member0 type]
//		[member1 name]:[member1 type]
//		[member2 name]:[member2 type]
//		[member3 name]:[optional][member3 type]
//		[unused][member4 name]:[member4 type]
//
//
//
// IDEA: Reporter output idea #2: ---
//
//Lua object is not a valid "Color"
//	Candidate [Parsed String]
//		Rejected: String is not a valid color format
//	Candidate [Filtered String]
//		Rejected: Maximum string length is 15
//	Candidate [Userdata]
//		Rejected: Type mismatch, Lua object is a [String]
//	Candidate [RGB Table]
//		Rejected:
//		!   r:[Number] - Missing member
//		!   g:[Number] - Missing member
//		!   b:[Number] - Missing member
//			a:[Optional Number] - Fallback to 1.0
//	Candidate [XYZ Table]
//		Rejected:
//		!   x:[Number] - Type mismatch, Lua object is a [String]
//		!   y:[Number] - Type mismatch, Lua object is a [String]
//		!   z:[Number] - Missing member
//			w:[Optional Number]=0.2
//	Candidate [Array Table]
//		Rejected:
//			1:[Number]=1
//			2:[Number]=1
//			3:[Number]=1
//			4:[Optional Number]=1
//		!   5:[Unused]=1 - Unused member




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
					if (var.is<CppT>()) {
						return std::optional<CppT>(std::in_place, var.as<CppT>());
					} else {
//						reporter.user_type_mismatch(LuaTypeName, LuaTypeEnum, var.get_type());
						return std::optional<CppT>(std::nullopt);
					}
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
constexpr inline char LuaTypeNameLightUserdata[] = "LightUserdata";

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
struct LightUserdata : PrimitiveType<CppT, CppT, LuaTypeNameLightUserdata, sol::type::lightuserdata, true> {};

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
//						reporter.missing_member(...);
//						reporter.member_matcher_failed?(...);
//						reporter.rejected(...);
						return EvalType(std::nullopt);
					}
				}(m.matcher.eval(table->template get<sol::object>(m.key), reporter)...);
			}, members);
//			reporter.unused_member(...);
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
	return detail::LightUserdata<CppT>();
}

// --- Aliases -------------------------------------------------------------------------------------

constexpr inline auto float_ = detail::Number<float>();
constexpr inline auto double_ = detail::Number<float>();
constexpr inline auto int32_ = detail::Number<int32_t>();
constexpr inline auto uint32_ = detail::Number<uint32_t>();
constexpr inline auto int64_ = detail::Number<int64_t>();
constexpr inline auto uint64_ = detail::Number<uint64_t>();

constexpr inline auto string_view = detail::String<std::string_view>();

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

struct ReportString {
	std::ostringstream ss;

	inline void lua_type_mismatch(const std::string_view name, const sol::type found, const sol::type expected) noexcept {
//		ss << name << found << expected;
		(void) found;
		(void) expected;

		ss << name;
	}
	inline void string_parse_failed(const std::string_view name, const std::string_view value) noexcept {
		ss << name << value;
	}
	inline auto reason() const {
		return ss.str();
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
