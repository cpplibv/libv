// Project: libv.lua, File: src/libv/lua/object_parser.hpp

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
// pro
#include <libv/lua/sol_type_to_string.hpp>


// =================================================================================================
//
// NOTE: Current state of the object parser:
//
//	It is working, functional and tested, but...
//
//	This seamed like a good idea, but the compile-time and the complexity detoured me from actually using it.
//	It is an actually functional and working implementation (apart from the reporters which are not implemented fully,
//	but their concept is proven)
//
//	The alternative imperative design ideas couldn't address my concerns, and they would be a different implementation
//	anyways, so for now I shelf this implementation
//
// =================================================================================================

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
// IDEA: Alternative imperative design idea #2 (multiple reentry with different template instance): ---
//      Not sure if this would be good, as side effects could repeat
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

// *** P5: Generalize variable/matcher/expression concept to be interchangeable (if implemented correctly could allow or_ and any_of to be the same)
// *** P5: Tables should handle members without a keys for constants and generators
// *** P5: Ability to name pattern and better name concatenation, maybe stream

namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename CppT, typename LuaT, const char* LuaTypeName, sol::type LuaTypeEnum, bool CheckIsType, typename Reporter>
inline auto primitive_eval(const sol::object& var, Reporter& reporter) {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
						// *** P5: report missing / failed members
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
		// *** P5: Better name for any
		return libv::concat(Matchers::name()...);
	}

	template <typename Reporter>
	inline auto impl(const sol::object& var, Reporter& reporter) const {
		(void) var;
		(void) reporter; // *** P5: Report

		if constexpr (std::is_void_v<CppType>)
			return false;
		else
			return EvalType(std::nullopt);
	}

	template <typename Reporter, typename Head, typename... Tail>
	inline auto impl(const sol::object& var, Reporter& reporter, const Head& head, const Tail&... tail) const {
		(void) reporter; // *** P5: Report

		auto result = head.eval(var, reporter);
		if (result)
			return result;
		else
			return impl(var, reporter, tail...);
	}

	template <typename Reporter>
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
		auto result = String<>::eval(var, reporter);

		if (!result)
			return EvalType{std::nullopt};

		auto parse_result = func(*result);
		if (parse_result)
			return parse_result;

		reporter.string_parse_failed(name(), *result);
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline auto eval(const sol::object& var, Reporter& reporter) const {
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
	inline void lua_type_mismatch(const std::string_view name, const sol::type expected, const sol::type found) noexcept {
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
//	int depth = 0;

	inline void lua_type_mismatch(const std::string_view name, const sol::type expected, const sol::type found) noexcept {
//		ss << name << found << expected;
		(void) found;
		(void) expected;

//		ss << "Type mismatch: " << name << ". Expected: " << lua_type_to_string(expected) << " Found: " << lua_type_to_string(found) << "\n";

		//	Candidate [Userdata]
		//		Rejected: Type mismatch, Expected a [Userdata] but got a [String]
		ss << "Candidate [" << name << "]\n";
		ss << "    Rejected: Type mismatch: Expected a [" << lua_type_to_string(expected) << "] but got a [" << lua_type_to_string(found) << "]\n";
	}
	inline void string_parse_failed(const std::string_view name, const std::string_view value) noexcept {
//		ss << name << value;

		//	Candidate [Parsed String]
		//		Rejected: String is not a valid color format
		ss << "Candidate [" << name << "]\n";
		ss << "    Rejected: Not a valid string: [" << value << "]\n";
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

// =================================================================================================
// Usage example:
// =================================================================================================
//
//
//namespace {
//
//// -------------------------------------------------------------------------------------------------
//
//inline const auto pattern_color() noexcept {
//	return libv::lua::one_of(
//			libv::lua::string_parse(&libv::parse::parse_color_optional),
//			libv::lua::as<Color>(libv::lua::userdata<libv::vec4f>()),
//			libv::lua::transform(libv::lua::userdata<libv::vec3f>(), [](const libv::vec3f& vec) { return Color(vec, 1.f); }),
//			libv::lua::table<Color>(
//					libv::lua::member("r", libv::lua::number<float>()),
//					libv::lua::member("g", libv::lua::number<float>()),
//					libv::lua::member("b", libv::lua::number<float>()),
//					libv::lua::member("a", libv::lua::or_(libv::lua::number<float>(), 1.f))),
//			libv::lua::table<Color>(
//					libv::lua::member("x", libv::lua::number<float>()),
//					libv::lua::member("y", libv::lua::number<float>()),
//					libv::lua::member("z", libv::lua::number<float>()),
//					libv::lua::member("w", libv::lua::or_(libv::lua::number<float>(), 1.f))),
//			libv::lua::table<Color>(
//					libv::lua::member(1, libv::lua::number<float>()),
//					libv::lua::member(2, libv::lua::number<float>()),
//					libv::lua::member(3, libv::lua::number<float>()),
//					libv::lua::member(4, libv::lua::or_(libv::lua::number<float>(), 1.f)))
//	);
//}
//
//template <typename F>
//inline const auto pattern_protocol(std::string protocol, F&& func) {
//	auto parse_func = [protocol = std::move(protocol)](auto str) {
//		if (not str.starts_with(protocol))
//			return std::optional<std::string_view>(std::nullopt);
//
//		str.remove_prefix(protocol.size());
//		return std::optional<std::string_view>(str);
//	};
//
//	return libv::lua::transform(libv::lua::string_parse(std::move(parse_func)), std::forward<F>(func));
//}
//
//inline const auto pattern_background(UI& ui) noexcept {
////	return libv::lua::string<std::string>();
//	return pattern_protocol("image ", [&ui](const auto path){ return ui.context().texture2D(path); });
////	return libv::lua::variant(
////			pattern_protocol("image ", [&ui](const auto path){ return ui.context().texture2D(path); }),
//////			pattern_protocol("color ", ),
//////			pattern_protocol("stretch ", ),
////			libv::lua::string_parse(&libv::parse::parse_color_optional)
////	);
//}
//
//void process_style_property(UI& ui, Style& style, const std::string_view key, const sol::object& value) {
//	const auto property = [&](const std::string_view name, const auto& pattern) {
//		if (name != key)
//			return false;
//
//		auto reporter = libv::lua::ReportString{};
//		const auto result = libv::lua::parse(value, pattern, reporter);
//		if (result) {
//			style.set(std::string(name), *result);
//		} else {
//			// TODO P4: Detailed error message with libv.lua.object_parser reporter API
//			log_ui.warn("Failed to parse style {} property {} value \"{}\". Property is set to fallback value. Reason:\n{}",
//					style.style_name, key, value.as<std::string_view>(), reporter.reason());
//		}
//
//		return true;
//	};
//
//	if (property("align", libv::lua::string_parse(&libv::ui::parse_align_horizontal_optional))) return;
//	if (property("align_vertical", libv::lua::string_parse(&libv::ui::parse_align_vertical_optional))) return;
//	if (property("background", pattern_background(ui))) return;
//	if (property("color", pattern_color())) return;
//	if (property("font", libv::lua::string_accept([&ui](const auto path){ return ui.context().font(path); }))) return;
//	if (property("font_color", pattern_color())) return;
//	if (property("font_outline", pattern_color())) return;
//	if (property("font_size", libv::lua::as<FontSize>(libv::lua::number<int16_t>()))) return;
//	if (property("size", libv::lua::string_parse(&libv::ui::parse_size_optional))) return;
////	if (property("font_shader", libv::lua::string())) return;
////	if (property("layout", libv::lua::string_parse(&libv::ui::parse_layout_optional))) return;
//
//	log_ui.warn("Ignoring unrecognized property style {} property {} value \"{}\".",
//			style.style_name, key, value.as<std::string_view>());
//}
//
//} // namespace
//
//// -------------------------------------------------------------------------------------------------
//
//Component script_file(UI& ui, lua::State& lua, const std::filesystem::path& file) {
//
//	auto ui_table = lua.create_table();
//	ui_table["bottom"] = "bottom";
//	ui_table["center"] = "center";
//	ui_table["justify"] = "justify";
//	ui_table["justifyall"] = "justifyall";
//	ui_table["left"] = "left";
//	ui_table["right"] = "right";
//	ui_table["top"] = "top";
//
//	ui_table["ORIENT_Orientation::up"] = Orientation::up;
//	ui_table["ORIENT_Orientation::right"] = Orientation::right;
//	ui_table["ORIENT_Orientation::left"] = Orientation::left;
//	ui_table["ORIENT_Orientation::down"] = Orientation::down;
//
//	std::unordered_map<std::string, libv::intrusive_ptr<Style>> styles;
//
//	ui_table.set_function("style", [&styles, &ui](const std::string_view style_name) {
//		auto& style_ip = styles[std::string(style_name)]; // TODO P5: C++20: std::string(std::string_view) temp string for hash lookup
//		if (!style_ip)
//			style_ip = libv::make_intrusive<Style>(std::string(style_name));
//
//		return [style_ip, &ui](const sol::table& table) {
//			for (const auto& [key, value] : table) {
//				if (key.get_type() != sol::type::string) {
//					log_ui.warn("Style's property key \"{}\" is expected to be a string. Style name: {}, key: {}, value: {}",
//							key.as<std::string_view>(), style_ip->style_name, key.as<std::string_view>(), value.as<std::string_view>());
//					continue;
//				}
//
//				const auto key_sv = key.as<std::string_view>();
//				process_style_property(ui, *style_ip, key_sv, value);
//			}
//		};
//	});
//
////	ui_table.set_function("component", [](const std::string_view component_name) {
////		return [component_name = std::string(component_name)](const sol::table& table) {
////			for (const auto& [key, value] : table) {
////				log_ui.info("{}/{} = {}", component_name, key.as<std::string_view>(), value.as<std::string_view>());
////			}
////		};
////	});
////
////	ui_table.set_function("color", [](const std::string_view color_str) -> sol::object {
////		auto color = libv::parse::parse_color_optional(color_str);
////		if (color)
////			return *color;
////		else
////			return sol::nil;
////	});
//
//	ui_table.set_function("component", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("image", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("stretch", [](const std::string_view) { return "Not implemented yet."; });
//	ui_table.set_function("layout_line", [](const std::string_view) { return "Not implemented yet."; });
//
//	lua["ui"] = ui_table;
//	const auto table = lua.script_file(file.string());
////	const auto table = lua.safe_script_file(file.string());
//
//	(void) ui;
//
//	// -------------------------------------------------------------------------------------------------
//
//	log_ui.trace("Parsed styles:");
//	for (const auto& [name, style] : styles)
//		style->foreach([&name](const auto& key, const auto& value) {
//			std::visit([&](const auto& var) {
//				if constexpr (std::is_enum_v<std::decay_t<decltype(var)>>)
//					log_ui.trace("{}/{} = {}", name, key, libv::to_value(var));
//				else
//					log_ui.trace("{}/{} = {}", name, key, var);
//			}, value);
//		});
//
////	return Component{nullptr};
//	assert(false && "Not implemented yet.");
//	Label tmp{"lua"};
//	tmp.text("Lua script component");
//	return tmp;
//}
// =================================================================================================
