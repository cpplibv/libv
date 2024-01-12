// Project: libv.lua, File: test/libv/lua/test_object_parser.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <sol/state.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/parse/color.hpp>
// std
#include <string_view>
// pro
#include <libv/lua/lua.hpp>
#include <libv/lua/object_parser.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

struct ReportLog {
	inline void lua_type_mismatch(const std::string_view name, const sol::type found, const sol::type expected) {
		(void) name;
		(void) found;
		(void) expected;
	}
	inline void string_parse_failed(const std::string_view name, const std::string_view value) {
		(void) name;
		(void) value;
	}
	inline auto reason() const {
		return "";
	}
};

struct Empty {
	bool operator==(const Empty&) const {
		return true;
	}
};

template <typename T>
struct Wrap {
	T value;

	template <typename K>
	Wrap(K&& value) : value(std::forward<K>(value)) {}
	bool operator==(const Wrap& rhs) const {
		return value == rhs.value;
	}
};

struct Sink {
	template <typename... T>
	Sink(T&&...) {}
	bool operator==(const Sink&) const {
		return true;
	}
};

using Color = libv::vec4f;

struct Planet {
	std::string name;
	Color color0;
	Color color1;
	Color color2;
	float radius;
	float mass;

	inline bool operator==(const Planet& other) const noexcept = default;

	friend inline std::ostream& operator<<(std::ostream& os, const Planet& rhs) {
		return os << '[' << rhs.name << ',' << rhs.color0 << ',' << rhs.color1 << ',' << rhs.color2 << ',' << rhs.radius << ',' << rhs.mass << ']';
	}
};

} // namespace

namespace ns = libv::lua;

TEST_CASE("object_parse simple types with void acts as existence bool query", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	CHECK(parse(R"(nil           )", ns::nil<void>()) == true);
	CHECK(parse(R"(true          )", ns::nil<void>()) == false);
	CHECK(parse(R"(1             )", ns::nil<void>()) == false);
	CHECK(parse(R"("text"        )", ns::nil<void>()) == false);
	CHECK(parse(R"(function() end)", ns::nil<void>()) == false);
	CHECK(parse(R"({}            )", ns::nil<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::nil<void>()) == false);

	CHECK(parse(R"(nil           )", ns::boolean<void>()) == false);
	CHECK(parse(R"(true          )", ns::boolean<void>()) == true);
	CHECK(parse(R"(1             )", ns::boolean<void>()) == false);
	CHECK(parse(R"("text"        )", ns::boolean<void>()) == false);
	CHECK(parse(R"(function() end)", ns::boolean<void>()) == false);
	CHECK(parse(R"({}            )", ns::boolean<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::boolean<void>()) == false);

	CHECK(parse(R"(nil           )", ns::number<void>()) == false);
	CHECK(parse(R"(true          )", ns::number<void>()) == false);
	CHECK(parse(R"(1             )", ns::number<void>()) == true);
	CHECK(parse(R"("text"        )", ns::number<void>()) == false);
	CHECK(parse(R"(function() end)", ns::number<void>()) == false);
	CHECK(parse(R"({}            )", ns::number<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::number<void>()) == false);

	CHECK(parse(R"(nil           )", ns::string<void>()) == false);
	CHECK(parse(R"(true          )", ns::string<void>()) == false);
	CHECK(parse(R"(1             )", ns::string<void>()) == false);
	CHECK(parse(R"("text"        )", ns::string<void>()) == true);
	CHECK(parse(R"(function() end)", ns::string<void>()) == false);
	CHECK(parse(R"({}            )", ns::string<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::string<void>()) == false);

	CHECK(parse(R"(nil           )", ns::function<void>()) == false);
	CHECK(parse(R"(true          )", ns::function<void>()) == false);
	CHECK(parse(R"(1             )", ns::function<void>()) == false);
	CHECK(parse(R"("text"        )", ns::function<void>()) == false);
	CHECK(parse(R"(function() end)", ns::function<void>()) == true);
	CHECK(parse(R"({}            )", ns::function<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::function<void>()) == false);

	CHECK(parse(R"(nil           )", ns::table<void>()) == false);
	CHECK(parse(R"(true          )", ns::table<void>()) == false);
	CHECK(parse(R"(1             )", ns::table<void>()) == false);
	CHECK(parse(R"("text"        )", ns::table<void>()) == false);
	CHECK(parse(R"(function() end)", ns::table<void>()) == false);
	CHECK(parse(R"({}            )", ns::table<void>()) == true);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::table<void>()) == false);

	CHECK(parse(R"(nil           )", ns::userdata<void>()) == false);
	CHECK(parse(R"(true          )", ns::userdata<void>()) == false);
	CHECK(parse(R"(1             )", ns::userdata<void>()) == false);
	CHECK(parse(R"("text"        )", ns::userdata<void>()) == false);
	CHECK(parse(R"(function() end)", ns::userdata<void>()) == false);
	CHECK(parse(R"({}            )", ns::userdata<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::userdata<void>()) == true);

	CHECK(parse(R"(nil           )", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"(true          )", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"(1             )", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"("text"        )", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"(function() end)", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"({}            )", ns::lightuserdata<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::lightuserdata<void>()) == false);
}

TEST_CASE("object_parse simple types", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	CHECK(parse(R"(nil           )", ns::nil())     == std::optional<sol::nil_t>(sol::nil));
	CHECK(parse(R"(true          )", ns::boolean()) == std::optional<bool>(true));
	CHECK(parse(R"(1             )", ns::number())  == std::optional<double>(1.0));
	CHECK(parse(R"("text"        )", ns::string())  == std::optional<std::string_view>("text"));
	//	CHECK(parse(R"(function() end)", ns::function()) == ? ? ?);
	//	CHECK(parse(R"({}            )", ns::table<void>()) == false);
	CHECK(parse(R"(vec3f(1, 2, 3))", ns::userdata<libv::vec3f>()) == std::optional<libv::vec3f>(std::in_place, 1, 2, 3));
	//	CHECK(parse(R"(              )", ns::lightuserdata<void>()) == false);
}

TEST_CASE("object_parse table", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	SECTION("void table without pattern") {
		auto pattern = ns::table<void>();
		CHECK(parse(R"({1, 2})", pattern) == true);
	}

	SECTION("void table with pattern") {
		auto pattern = ns::table<void>(
				ns::member(1, ns::number<int>()),
				ns::member(2, ns::number<int>())
		);
		CHECK(parse(R"({1}      )", pattern) == false);
		CHECK(parse(R"({1, 2}   )", pattern) == true);
		CHECK(parse(R"({1, 2, 3})", pattern) == true);
	}

	SECTION("empty pattern") {
		auto pattern = ns::table<Empty>();
		CHECK(parse(R"({}      )", pattern) == std::optional<Empty>(std::in_place));
		CHECK(parse(R"({10}    )", pattern) == std::optional<Empty>(std::in_place));
		CHECK(parse(R"({10, 20})", pattern) == std::optional<Empty>(std::in_place));
	}

	SECTION("simple int key pair pattern") {
		auto pattern = ns::table<std::pair<int, int>>(
				ns::member(1, ns::number<int>()),
				ns::member(2, ns::number<int>())
		);
		CHECK(parse(R"({10, 20})", pattern) == std::optional<std::pair<int, int>>(std::in_place, 10, 20));
	}

	SECTION("simple text key pair pattern") {
		auto pattern = ns::table<std::pair<int, int>>(
				ns::member("key1", ns::number<int>()),
				ns::member("key2", ns::number<int>())
		);
		CHECK(parse(R"({["key1"] = 10, ["key2"] = 20})", pattern) == std::optional<std::pair<int, int>>(std::in_place, 10, 20));
	}

	SECTION("simple hybrid key pair pattern") {
		auto pattern = ns::table<std::pair<int, int>>(
				ns::member(1, ns::number<int>()),
				ns::member("key1", ns::number<int>())
		);
		CHECK(parse(R"({10, ["key1"] = 20})", pattern) == std::optional<std::pair<int, int>>(std::in_place, 10, 20));
	}

	SECTION("missing key pattern") {
		auto pattern = ns::table<void>(
				ns::member(1, ns::number<int>()),
				ns::member("missing", ns::number<int>())
		);
		CHECK(parse(R"({10, ["key1"] = 20})", pattern) == false);
	}
}

// --- Value ---------------------------------------------------------------------------------------

TEST_CASE("adaptor constant", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	CHECK(parse(R"(nil)", ns::constant(2)) == std::optional<int>(2));
	CHECK(parse(R"(nil)", ns::constant<std::string>("text")) == std::optional<std::string>("text"));
}

TEST_CASE("adaptor generate", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	int i0 = 0;
	const auto f0 = [&]{ return i0++; };

	CHECK(parse(R"(nil)", ns::generate(f0)) == std::optional<int>(0));
	CHECK(parse(R"(nil)", ns::generate(f0)) == std::optional<int>(1));
	CHECK(parse(R"(nil)", ns::generate(f0)) == std::optional<int>(2));

	std::string i1 = "";
	const auto f1 = [&]{ return i1 += "x"; };

	CHECK(parse(R"(nil)", ns::generate(f1)) == std::optional<std::string>("x"));
	CHECK(parse(R"(nil)", ns::generate(f1)) == std::optional<std::string>("xx"));
	CHECK(parse(R"(nil)", ns::generate(f1)) == std::optional<std::string>("xxx"));
}

// --- Adaptors ------------------------------------------------------------------------------------

TEST_CASE("adaptor as", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	CHECK(parse(R"(2.0   )", ns::as<Sink>(ns::number())) == std::optional<Sink>(std::in_place));
	CHECK(parse(R"("text")", ns::as<Sink>(ns::number())) == std::optional<Sink>(std::nullopt));
	CHECK(parse(R"(2.0   )", ns::as<Sink>(ns::number())) == std::optional<Sink>(std::in_place));
	CHECK(parse(R"("text")", ns::as<Sink>(ns::number())) == std::optional<Sink>(std::nullopt));
}

TEST_CASE("adaptor or", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	CHECK(parse(R"(2.0   )", ns::or_(ns::number(), 1))   == std::optional<double>(2.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::number(), 1))   == std::optional<double>(1.0));
	CHECK(parse(R"("text")", ns::or_(ns::number(), 1))   == std::optional<double>(1.0));
	CHECK(parse(R"(2.0   )", ns::or_(ns::number(), 1.0)) == std::optional<double>(2.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::number(), 1.0)) == std::optional<double>(1.0));
	CHECK(parse(R"("text")", ns::or_(ns::number(), 1.0)) == std::optional<double>(1.0));
}

TEST_CASE("adaptor one_of", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	auto pattern = ns::one_of(
		ns::table<double>(ns::member("a", ns::number())),
		ns::table<double>(ns::member("b", ns::number())),
		ns::table<double>(ns::member("c", ns::number()))
	);

	CHECK(parse(R"({a = 10                })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({b = 10                })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({c = 10                })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({a = 10, b = 20        })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({a = 10, c = 20        })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({b = 10, c = 20        })", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({a = 10, b = 20, c = 30})", pattern) == std::optional<double>(10.0));
	CHECK(parse(R"({                      })", pattern) == std::optional<double>(std::nullopt));
	CHECK(parse(R"({d = 30                })", pattern) == std::optional<double>(std::nullopt));
}

TEST_CASE("adaptor transform", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	auto call_count = 0;
	auto fd = [&call_count](const auto& n) { call_count++; return n + n; };
	auto fs = [&call_count](const auto& n) { call_count++; return libv::concat(n, n); };
	auto fb = [&call_count](const auto& n) { call_count++; return !n; };
	auto fv = [&call_count](const auto&) { call_count++; };

	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::number(), fd))), std::optional<double>>);
	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::string(), fs))), std::optional<std::string>>);
	CHECK(parse(R"(nil)", ns::transform(ns::number(), fd)) == std::optional<double>(std::nullopt));
	CHECK(call_count == 0); call_count = 0;
	CHECK(parse(R"(5  )", ns::transform(ns::number(), fd)) == std::optional<double>(std::in_place, 10.0));
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"("x")", ns::transform(ns::string(), fs)) == std::optional<std::string>("xx"));
	CHECK(call_count == 1); call_count = 0;

	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::number<void>(), fb))), bool>);
	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::string<void>(), fb))), bool>);
	CHECK(parse(R"(nil)", ns::transform(ns::number<void>(), fb)) == true);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"(5  )", ns::transform(ns::number<void>(), fb)) == false);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"("x")", ns::transform(ns::string<void>(), fb)) == false);
	CHECK(call_count == 1); call_count = 0;

	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::number<void>(), fv))), bool>);
	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::string<void>(), fv))), bool>);
	CHECK(parse(R"(nil)", ns::transform(ns::number<void>(), fv)) == false);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"(5  )", ns::transform(ns::number<void>(), fv)) == true);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"("x")", ns::transform(ns::string<void>(), fv)) == true);
	CHECK(call_count == 1); call_count = 0;

	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::number(), fv))), bool>);
	static_assert(std::is_same_v<decltype(parse("", ns::transform(ns::string(), fv))), bool>);
	CHECK(parse(R"(nil)", ns::transform(ns::number(), fv)) == false);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"(5  )", ns::transform(ns::number(), fv)) == true);
	CHECK(call_count == 1); call_count = 0;
	CHECK(parse(R"("x")", ns::transform(ns::string(), fv)) == true);
	CHECK(call_count == 1); call_count = 0;
}

TEST_CASE("adaptor string_parse", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	const auto f = [](const auto& str) {
		if (str.empty())
			return std::optional<Wrap<std::string>>(std::nullopt);
		else
			return std::optional<Wrap<std::string>>(std::in_place, str);
	};

	CHECK(parse(R"(nil)", ns::string_parse(f)) == std::optional<Wrap<std::string>>(std::nullopt));
	CHECK(parse(R"("" )", ns::string_parse(f)) == std::optional<Wrap<std::string>>(std::nullopt));
	CHECK(parse(R"("x")", ns::string_parse(f)) == std::optional<Wrap<std::string>>(std::in_place, "x"));
}

// --- Nested Adaptors -----------------------------------------------------------------------------

TEST_CASE("adaptor or nested with adaptor as", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	using CD = Wrap<double>;

	CHECK(parse(R"(2.0   )", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(2.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(1.0));
	CHECK(parse(R"("text")", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(1.0));
	CHECK(parse(R"(2.0   )", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(2.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(1.0));
	CHECK(parse(R"("text")", ns::or_(ns::as<CD>(ns::number()), CD(1))) == std::optional<CD>(1.0));
	CHECK(parse(R"(2.0   )", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(2.0));
	CHECK(parse(R"(nil   )", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(1.0));
	CHECK(parse(R"("text")", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(1.0));
	CHECK(parse(R"(2.0   )", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(2.0));
	CHECK(parse(R"(nil   )", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(1.0));
	CHECK(parse(R"("text")", ns::as<CD>(ns::or_(ns::number(), 1)))     == std::optional<CD>(1.0));
}

TEST_CASE("adaptor or nested with adaptor transform", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	const auto f = [](const auto& x) { return x * x; };

	CHECK(parse(R"(2.0   )", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(4.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(3.0));
	CHECK(parse(R"("text")", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(3.0));
	CHECK(parse(R"(2.0   )", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(4.0));
	CHECK(parse(R"(nil   )", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(3.0));
	CHECK(parse(R"("text")", ns::or_(ns::transform(ns::number(), f), 3.0)) == std::optional<double>(3.0));
	CHECK(parse(R"(2.0   )", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(4.0));
	CHECK(parse(R"(nil   )", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(9.0));
	CHECK(parse(R"("text")", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(9.0));
	CHECK(parse(R"(2.0   )", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(4.0));
	CHECK(parse(R"(nil   )", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(9.0));
	CHECK(parse(R"("text")", ns::transform(ns::or_(ns::number(), 3.0), f)) == std::optional<double>(9.0));
}

// --- Concepts ------------------------------------------------------------------------------------

//template <typename MatcherT>
//concept bool Matcher = requires(const MatcherT matcher, sol::object var, ReportNoop reporter) {
//		typename MatcherT::CppType;
//		typename MatcherT::EvalType;
//		{ matcher.name() } -> String;
//		{ matcher.eval(var, reporter) } -> typename MatcherT::CppType;
//		{ matcher.eval(var, reporter) } -> std::optional<typename MatcherT::CppType>;
//};
//
//TEST_CASE("Every matcher has to conform to the Matcher concept", "[libv.lua.op]") {
//
//	CHECK((requires Matcher<decltype(ns::number())>));
//}

// --- Complex -------------------------------------------------------------------------------------

TEST_CASE("complex pattern: color and planet", "[libv.lua.op]") {
	auto state = ns::create_state(ns::lib::libv_vec);
	const auto parse = [&](const std::string& script, const auto& parser) {
		return ns::parse(state.script("return " + script).get<sol::object>(), parser);
	};

	const auto pattern_color = [] {
		return ns::one_of(
				ns::string_parse(&libv::parse::parse_color_optional),
				ns::as<Color>(ns::userdata<libv::vec4f>()),
				ns::transform(ns::userdata<libv::vec3f>(), [](const libv::vec3f& vec) { return Color(vec, 1.f); }),
				ns::table<Color>(
					ns::member("r", ns::number<float>()),
					ns::member("g", ns::number<float>()),
					ns::member("b", ns::number<float>()),
					ns::member("a", ns::or_(ns::number<float>(), 1.f))),
				ns::table<Color>(
					ns::member("x", ns::number<float>()),
					ns::member("y", ns::number<float>()),
					ns::member("z", ns::number<float>()),
					ns::member("w", ns::or_(ns::number<float>(), 1.f))),
				ns::table<Color>(
					ns::member(1, ns::number<float>()),
					ns::member(2, ns::number<float>()),
					ns::member(3, ns::number<float>()),
					ns::member(4, ns::or_(ns::number<float>(), 1.f)))
		);
	};

	int i = 0;
	const auto next_name = [&i] {
		return libv::concat("planet-", i++);
	};

	const auto pattern_planet = [&pattern_color, &next_name] {
		return ns::table<Planet>(
				ns::member("name", ns::one_of(ns::string<std::string>(), ns::generate(next_name))),
				ns::member("color0", pattern_color()),
				ns::member("color1", pattern_color()),
				ns::member("color2", pattern_color()),
				ns::member("radius", ns::or_(ns::number<float>(), 10.f)),
				ns::member("mass", ns::number<float>())
		);
	};

	{
		const auto script = R"qq({
			name = "text-name",
			color0 = {1, 0, 0},
			color1 = "red",
			color2 = "hsv(120°, 100%, 100%, 50%)",
			mass = 20,
		})qq";
		const auto expected = Planet{"text-name", {1, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 0.5}, 10, 20};
		CHECK(parse(script, pattern_planet()) == std::optional<Planet>(expected));
	}
	{
		const auto script = R"qq({
			color0 = vec4f(1, 0, 0, 1),
			color1 = vec3f(1, 1, 0),
			color2 = {0, 1, 0, 0.5},
			radius = 5,
			mass = 10,
		})qq";
		const auto expected = Planet{"planet-0", {1, 0, 0, 1}, {1, 1, 0, 1}, {0, 1, 0, 0.5}, 5, 10};
		CHECK(parse(script, pattern_planet()) == std::optional<Planet>(expected));
	}
}

// --- Reporters -----------------------------------------------------------------------------------

// TODO P4: Reporters
