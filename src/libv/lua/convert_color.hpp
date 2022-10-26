// Project: libv.lua, File: src/libv/lua/convert_color.hpp

#pragma once

// libv
#include <libv/lua/lua.hpp>
#include <libv/math/vec.hpp>
#include <libv/parse/color.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

inline std::optional<libv::vec4f> convert_color(const sol::object& object) {

	if (object.get_type() == sol::type::string) {
		return {libv::parse::parse_color_optional(object.as<std::string_view>())};

	} else if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec3f>())
			return libv::vec4f(object.as<libv::vec3f>(), 1.0f);

		else if (object.is<libv::vec4f>())
			return object.as<libv::vec4f>();

		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();
		{
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			auto n3 = table.get<sol::object>(3);
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>(4);
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}
		{
			auto n1 = table.get<sol::object>("r");
			auto n2 = table.get<sol::object>("g");
			auto n3 = table.get<sol::object>("b");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>("a");
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}
		{
			auto n1 = table.get<sol::object>("x");
			auto n2 = table.get<sol::object>("y");
			auto n3 = table.get<sol::object>("z");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number) {
				auto n4 = table.get<sol::object>("w");
				if (n4.get_type() == sol::type::number)
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), n4.as<float>());
				else
					return libv::vec4f(n1.as<float>(), n2.as<float>(), n3.as<float>(), 1.0f);
			}
		}

		return std::nullopt;

	} else {
		return std::nullopt;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
