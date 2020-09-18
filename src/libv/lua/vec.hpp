// Project: libv.lua, File: src/libv/lua/vec.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <fmt/format.h>
#include <sol/state.hpp>
#include <sol/usertype.hpp>
// libv
#include <libv/math/vec.hpp>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace lua {

template <typename V, typename T, typename R>
void registerLuaVecCommon(R& type) {
	type.set(sol::meta_function::addition, sol::overload(
			sol::resolve<T(const T&, const T&)>(::libv::operator+),
			sol::resolve<T(const V&, const T&)>(::libv::operator+),
			sol::resolve<T(const T&, const V&)>(::libv::operator+)
	));
	type.set(sol::meta_function::subtraction, sol::overload(
			sol::resolve<T(const T&, const T&)>(::libv::operator-),
			sol::resolve<T(const V&, const T&)>(::libv::operator-),
			sol::resolve<T(const T&, const V&)>(::libv::operator-)
	));
	type.set(sol::meta_function::multiplication, sol::overload(
			sol::resolve<T(const T&, const T&)>(::libv::operator*),
			sol::resolve<T(const V&, const T&)>(::libv::operator*),
			sol::resolve<T(const T&, const V&)>(::libv::operator*)
	));
	type.set(sol::meta_function::division, sol::overload(
			sol::resolve<T(const T&, const T&)>(::libv::operator/),
			sol::resolve<T(const V&, const T&)>(::libv::operator/),
			sol::resolve<T(const T&, const V&)>(::libv::operator/)
	));
	type.set("length", &T::length);
}

template <typename T>
void registerLuaVec2(sol::state& lua, const std::string& name) {
	using V = typename T::value_type;

	auto type = lua.new_usertype<T>(
			name,
			sol::call_constructor, sol::constructors<sol::types<V, V>>{}
	);

	type.set("x", &T::x);
	type.set("y", &T::y);

	type.set(sol::meta_function::to_string, [](const T& vec) {
			return fmt::format("{} {}", vec.x, vec.y);
	});

	registerLuaVecCommon<V, T>(type);
}

template <typename T>
void registerLuaVec3(sol::state& lua, const std::string& name) {
	using V = typename T::value_type;

	auto type = lua.new_usertype<T>(
			name,
			sol::call_constructor, sol::constructors<sol::types<V, V, V>>{}
	);

	type.set("x", &T::x);
	type.set("y", &T::y);
	type.set("z", &T::z);

	type.set(sol::meta_function::to_string, [](const T& vec) {
			return fmt::format("{} {} {}", vec.x, vec.y, vec.z);
	});

	registerLuaVecCommon<V, T>(type);
}

template <typename T>
void registerLuaVec4(sol::state& lua, const std::string& name) {
	using V = typename T::value_type;

	auto type = lua.new_usertype<T>(
			name,
			sol::call_constructor, sol::constructors<sol::types<V, V, V, V>>{}
	);

	type.set("x", &T::x);
	type.set("y", &T::y);
	type.set("z", &T::z);
	type.set("w", &T::w);

	type.set(sol::meta_function::to_string, [](const T& vec) {
			return fmt::format("{} {} {} {}", vec.x, vec.y, vec.z, vec.w);
	});

	registerLuaVecCommon<V, T>(type);
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace iris
