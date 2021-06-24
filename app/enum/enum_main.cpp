// Project: libv, File: app/enum/enum_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/lua/lua.hpp>
// ext
#include <clip/clip.h>
#include <fmt/format.h>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>
// std
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>


// -------------------------------------------------------------------------------------------------

static constexpr std::string_view enum_gen_version = "v14";

// -------------------------------------------------------------------------------------------------

class EnumBuilder {
private:
	std::string	enum_name;
	std::string	enum_type;
	std::string	class_enum_type_name;
	std::string	class_enum_state_name;
	std::string	global_var_enum_state_name;

	struct Value {
		std::string identifier;
		std::string value;
		std::string text;
		bool init;
	};
	std::vector<Value> enum_entries;

	struct Property {
		std::string type;
		std::map<std::string, std::string> values; // Enum entry name -> Property value
	};
	std::map<std::string, Property> properties; // Property name -> Property

	std::vector<std::string> namespaces;
	std::set<std::string> includes;

private:
//	const Value& find_init() {
//		for (const auto& v : values)
//			if (v.init)
//				return v;
//
//		static Value init_fallback{"", "", "", false};
//		return init_fallback;
//	}

private:
	size_t longest_value_name = 0;
	size_t longest_value_text = 0;

public:
//	bool gen_operator_eq = true;
//	bool gen_operator_rel = true;
	bool gen_to_string = true;
	bool gen_to_stream = true;
	bool gen_to_span = true;

public:
	explicit EnumBuilder(std::string enum_name_ = "my_enum", std::string enum_type_ = "int") :
		enum_name(std::move(enum_name_)),
		enum_type(std::move(enum_type_)) {

		class_enum_type_name = enum_name + "_type";
		class_enum_state_name = enum_name + "_state";
		global_var_enum_state_name = enum_name + "_enum";
	}

public:
	void add_namespace(std::string namespace_str) {
		namespaces.emplace_back(std::move(namespace_str));
	}
	void include(std::string include_str) {
		includes.emplace(std::move(include_str));
	}
	void value(std::string name, std::string text, bool init_value_ = true) {
		enum_entries.emplace_back(std::move(name), std::to_string(enum_entries.size()), std::move(text), init_value_);
		longest_value_name = std::max(longest_value_name, name.size());
		longest_value_text = std::max(longest_value_text, text.size());
	}
	void property(std::string property_name, std::string property_type) {
		properties[std::move(property_name)].type = std::move(property_type);
	}
	void property_value(std::string property_name, std::string enum_, std::string value) {
		properties[std::move(property_name)].values.emplace(std::move(enum_), std::move(value));
	}

	// -------------------------------------------------------------------------------------------------

public:
	[[nodiscard]] std::string generate_source_code() const {
		std::stringstream os;

		const auto out = [&os](std::string_view fmt, auto&&... args) {
			os << fmt::format(fmt, std::forward<decltype(args)>(args)...);
		};

		out("// Generated source for enum: {}\n", enum_name);
		out("// Generator: enum {}\n", enum_gen_version);
		out("\n");
		out("#pragma once\n");
		out("\n");

		if (gen_to_string || gen_to_stream || gen_to_span) {
			out("// std\n");

			if (gen_to_string || gen_to_stream || !properties.empty())
				out("#include <cassert>\n");

			if (gen_to_stream) {
				out("#include <iosfwd>\n");
				out("#include <ostream>\n");
			}

			if (gen_to_span)
				out("#include <span>\n");

			if (gen_to_string || gen_to_stream)
				out("#include <string_view>\n");
		}

		if (!includes.empty())
			out("//\n");
		for (const auto& in : includes)
			out("#include <{}>\n", in);

		out("\n");
		out("\n");

		for (const auto& ns : namespaces)
			out("namespace {} {{\n", ns);

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");
		out("\n");

		out("enum class {}{}{} {{\n", enum_name, enum_type.empty() ? "" : " : ", enum_type);
		for (const auto& enum_entry : enum_entries)
			out("	{} = {},\n", enum_entry.identifier, enum_entry.value);
		out("}};\n");

		out("\n");
		out("// --- {} ---\n", class_enum_type_name);

		out("\n");
		out("struct {} {{\n", class_enum_type_name);
		out("public:\n");
		out("	using enum_type = {};\n", enum_name);
		out("	using underlying_type = {};\n", enum_type);

		out("\n");
		out("private:\n");
		out("	enum_type enum_value_;\n");

		out("\n");
		out("public:\n");
		out("	explicit(false) constexpr inline {}(enum_type value) noexcept :\n", class_enum_type_name);
		out("		enum_value_(value) {{\n");
		out("	}}\n");
		out("\n");
		out("	explicit(false) [[nodiscard]] constexpr inline operator enum_type() const noexcept {{\n");
		out("		return enum_value_;\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline enum_type enum_value() const noexcept {{\n");
		out("		return enum_value_;\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline underlying_type underlying() const noexcept {{\n");
		out("		return static_cast<underlying_type>(enum_value_);\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline underlying_type operator+() const noexcept {{\n");
		out("		return static_cast<underlying_type>(enum_value_);\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline enum_type value() const noexcept {{\n");
		out("		return enum_value();\n");
		out("	}}\n");

//      NOTE: No need to generate comparison operators as the implicit conversion to the enum takes care of it
//
//		if (gen_operator_eq || gen_operator_rel) {
//			out("\n");
//			out("public:\n");
//
//			const auto out_op_function = [&](const auto& op_str) {
//				out("	[[nodiscard]] friend constexpr inline bool operator{}(const {}& lhs, const {}& rhs) noexcept {{\n", op_str, class_enum_type_name, class_enum_type_name);
//				out("		return lhs.underlying() {} rhs.underlying();\n", op_str);
//				out("	}}\n");
//			};
//
//			if (gen_operator_eq) {
//				out_op_function("==");
//				out_op_function("!=");
//			}
//
//			if (gen_operator_rel) {
//				out_op_function("<");
//				out_op_function("<=");
//				out_op_function(">");
//				out_op_function(">=");
//			}
//		}

		if (gen_to_string || gen_to_stream) {
			out("\n");
			out("private:\n");
			out("	static constexpr std::string_view table_to_string[] = {{\n");

			const auto longest_text = std::ranges::max(enum_entries | ranges::v3::view::transform([](const Value& v) { return v.text.size(); }));

			for (const auto& enum_entry : enum_entries)
				out("			{:{}} // {}\n", '"' + enum_entry.text + "\",", longest_text + 3, enum_entry.identifier);
			out("	}};\n");
			out("\n");
			out("public:\n");
			out("	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {{\n");
			out("		assert(underlying() >= 0 && underlying() < {});\n", enum_entries.size());
			out("		return table_to_string[underlying()];\n");
			out("	}}\n");
		}

		// TODO P1: parse

		for (const auto& [property_name, property] : properties) {
			out("\n");
			out("private:\n");
			out("	static constexpr {} table_{}[] = {{\n", property.type, property_name);

			const auto longest_prop_value = std::ranges::max(enum_entries | ranges::v3::view::transform([&](const Value& v) { return property.values.at(v.identifier).size(); }));

			for (const auto& enum_entry : enum_entries)
				out("			{:{}} // {}\n", property.values.at(enum_entry.identifier) + ',', longest_prop_value + 1, enum_entry.identifier);
			out("	}};\n");
			out("\n");
			out("public:\n");
			out("	[[nodiscard]] constexpr inline {} {}() const noexcept {{\n", property.type, property_name);
			out("		assert(underlying() >= 0 && underlying() < {});\n", enum_entries.size());
			out("		return table_{}[underlying()];\n", property_name);
			out("	}}\n");
		}

		out("}};\n"); // End of enum_type class

		out("\n");
		out("// --- {} ---\n", class_enum_state_name);

		out("\n");
		out("class {} {{\n", class_enum_state_name);
		out("public:\n");
		out("	using enum_type = {};\n", enum_name);
		out("	using underlying_type = {};\n", enum_type);

		out("\n");
		out("private:\n");
		out("	static constexpr enum_type table_enum_values[] = {{\n");
		for (const auto& enum_entry : enum_entries)
			out("			enum_type::{},\n", enum_entry.identifier);
		out("	}};\n");
		out("\n");
		out("public:\n");
		out("	static constexpr size_t size = {};\n", enum_entries.size());

		if (gen_to_span) {
			out("\n");
			out("	[[nodiscard]] static std::span<const enum_type> values() noexcept {{\n");
			out("		return table_enum_values;\n");
			out("	}}\n");
		}

//		if (!custom_functions.empty()) {
//			member_functions += "\npublic:\n";
//			for (const auto& fmt_fn : functions)
//				member_functions += fmt::format(fmt_fn,
//						fmt::arg("enum_name", enum_name),
//						fmt::arg("enum_type", enum_type)
//				);
//		}

		out("}};\n");

		out("\n");
		out("// --- {} ---\n", "Global ADL functions");

		if (gen_to_stream || gen_to_string) {
			out("\n");
			out("[[nodiscard]] constexpr inline std::string_view to_string({} enum_value) noexcept {{\n", enum_name);
			out("	return {}(enum_value).to_string();\n", class_enum_type_name);
			out("}}\n");

			if (gen_to_stream) {
				out("\n");
				out("std::ostream& operator<<(std::ostream& os, const {} var) {{\n", enum_name);
				out("	return os << {}(var).to_string();\n", class_enum_type_name);
				out("}}\n");
			}
		}

		out("\n");
		out("[[nodiscard]] constexpr inline {} underlying({} enum_value) noexcept {{\n", enum_type, enum_name);
		out("	return static_cast<{}>(enum_value);\n", enum_type);
		out("}}\n");

		out("\n");
		out("[[nodiscard]] constexpr inline {} operator+({} enum_value) noexcept {{\n", enum_type, enum_name);
		out("	return static_cast<{}>(enum_value);\n", enum_type);
		out("}}\n");

		out("\n");
		out("[[nodiscard]] constexpr inline {} type({} enum_value) noexcept {{\n", class_enum_type_name, enum_name);
		out("	return {}(enum_value);\n", class_enum_type_name);
		out("}}\n");

		out("\n");
		out("// --- {} ---\n", "Global state variables");

		out("\n");
		out("static constexpr {} {};\n", class_enum_state_name, global_var_enum_state_name);

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");
		out("\n");

		for (const auto& ns : namespaces | ranges::view::reverse)
			out("}} // namespace {}\n", ns);

		return std::move(os).str();
	}
};

// -------------------------------------------------------------------------------------------------

struct EnumBuilderLua {
	libv::lua::State lua;
	std::optional<EnumBuilder> eb;

	EnumBuilderLua() {
		lua.set_function("enum", [this](std::string name, std::string type) {
			eb.emplace(std::move(name), std::move(type));
		});

		lua.set_function("namespace", [this](std::string name) {
			eb->add_namespace(std::move(name));
		});

		lua.set_function("include", [this](std::string name) {
			eb->include(std::move(name));
		});

		lua.set_function("property", [this](std::string name, std::string type) {
			eb->property(name, type);

			auto property_function = sol::make_object(lua, [this, name, type](std::string value) {
				auto property_marked_value = lua.create_table();

				property_marked_value["name"] = name;
				property_marked_value["value"] = value;

				return property_marked_value;
			});

			return property_function;
		});

		lua.set_function("value", [this](std::string entry, std::string text, sol::table properties) {
			eb->value(entry, text, true);

			for (const auto& [_, property] : properties) {
				if (!property.is<sol::table>()) {
					// error management
					std::cerr << "property tables in values can only contain string or marked property table\n";
					continue;
				}

				const auto marked_property_table = property.as<sol::table>();
				auto property_name = marked_property_table["name"].get<std::string>();
				auto property_value = marked_property_table["value"].get<std::string>();

				eb->property_value(
						std::move(property_name),
						entry,
						std::move(property_value)
				);
			}
		});
	}

	std::string generate_source_code(std::string_view source_script) {
		auto env = sol::environment(lua, sol::create, lua.globals());

		lua.script(source_script, env);

		auto result = eb->generate_source_code();
		eb.reset();

		return result;
	}
};

// -------------------------------------------------------------------------------------------------

void clipboard(const std::string& string) {
	bool success = clip::set_text(string);
	if (!success)
		std::cerr << fmt::format("Failed to set clipboard text of length {}: \"{}\"", string.size(), string) << std::endl;
}

// -------------------------------------------------------------------------------------------------

int main() {
//	EnumBuilder eb("color", "int32_t");
//
////	eb.gen_operator_eq = true;
////	eb.gen_operator_rel = true;
//	eb.gen_to_string = true;
//	eb.gen_to_stream = true;
//	eb.gen_to_span = true;
//
//	eb.add_namespace("libv::ns");
//	eb.add_namespace("color");
//	eb.include("libv/math/vec.hpp");
//
//	eb.property("rgba", "libv::vec4f");
//	eb.value("red", "red", false);     eb.property_value("rgba", "red",   "libv::vec4f{1, 0, 0, 1}");
//	eb.value("green", "green", false); eb.property_value("rgba", "green", "libv::vec4f{0, 1, 0, 1}");
//	eb.value("blue", "blue", false);   eb.property_value("rgba", "blue",  "libv::vec4f{0, 0, 1, 1}");

	EnumBuilderLua eb;

	const auto script = R"(
		enum("color", "int32_t")
		namespace("libv::ns")
		include("libv/math/vec.hpp")
		rgba = property("rgba", "libv::vec4f")

--		custom_function("int calc() { return 42; }")
		value("red"  , "Red"  , {rgba("libv::vec4f{1, 0, 0, 1}")})
		value("green", "Green", {rgba("libv::vec4f{0, 1, 0, 1}")})
		value("blue" , "Blue" , {rgba("libv::vec4f{0, 0, 1, 1}")})
)";

//	clipboard(result);
	std::cout << eb.generate_source_code(script) << std::endl;

	return 0;
}


// =================================================================================================
// === GEN 2.0 =====================================================================================


/*
	INPUT - LUA script
		enum("color", "int32_t")
		namespace("libv::ns")
		include("libv/math/vec.hpp")
		rgba = property("rgba", "libv::vec4f")
		custom_function("int calc() { return 42; }")
		values{
				red =   { rgba("1, 0, 0, 1") },
				green = { rgba("0, 1, 0, 1") },
				blue =  { rgba("0, 0, 1, 1") },
		)

	OUTPUT
		see example.hpp

	TEST
		auto foo(color a) {
		    return
		        a == a ||
		        a != a ||
		        a <= a ||
		        a < a ||
		        a >= a ||
		        a > a ||

		        type(a) == type(a) ||
		        type(a) != type(a) ||
		        type(a) <= type(a) ||
		        type(a) < type(a) ||
		        type(a) >= type(a) ||
		        type(a) > type(a) ||

		        a == type(a) ||
		        a != type(a) ||
		        a <= type(a) ||
		        a < type(a) ||
		        a >= type(a) ||
		        a > type(a) ||

		        type(a) == a ||
		        type(a) != a ||
		        type(a) <= a ||
		        type(a) < a ||
		        type(a) >= a ||
		        type(a) > a;
		}
		auto foo2(color a) {
		    return type(a).to_string();
		}
		auto foo3(color a) {
		    return to_string(a);
		}
		auto foo4(color a) {
		    return to_string(type(a));
		}
		auto foo5(color a) {
		    return
		        std::hash<color>()(a) +
		        std::hash<color>()(type(a)) +
		        0;
		        //std::hash<color_type>()(type(a)) +
		        //std::hash<color_type>()(a);
		}
		auto foo6() {
		    return color_enum.values();
		}
		auto foo7() {
		    for (const auto c : color_enum.values());
		}
		void foo8(std::ostream& os, color c) {
		    os << c;
		    os << type(c);
		}
		auto foo9(color a) {
		    return +a;
		}
 */

//CONSTEXPR MAP - Might be useful for something
//
//	#include <array>
//	#include <string_view>
//	#include <stdexcept>
//	#include <algorithm>
//	#include <map>
//
//	template <typename Key, typename Value, std::size_t Size>
//	struct ConstexprMap {
//		std::array<std::pair<Key, Value>, Size> data;
//
//		[[nodiscard]] constexpr Value at(const Key &key) const {
//			const auto itr =
//					std::find_if(begin(data), end(data),
//							[&key](const auto &v) { return v.first == key; });
//			if (itr != end(data)) {
//				return itr->second;
//			} else {
//				throw std::range_error("Not Found");
//			}
//		}
//	};
//
//	using namespace std::literals::string_view_literals;
//	static constexpr std::array<std::pair<std::string_view, int>, 8> color_values{{
//			{"black"sv, 7},
//			{"blue"sv, 3},
//			{"cyan"sv, 5},
//			{"green"sv, 2},
//			{"magenta"sv, 6},
//			{"red"sv, 1},
//			{"white"sv, 8},
//			{"yellow"sv, 4}
//	}};
//
//	int lookup_value(const std::string_view sv) {
//		//static const auto map = std::map<std::string_view, int>{color_values.begin(), color_values.end()};
//		static constexpr auto map =
//		Map<std::string_view, int, color_values.size()>{{color_values}};
//
//		return map.at(sv);
//	}
