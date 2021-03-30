// Project: libv, File: app/enum/enum_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <clip/clip.h>
#include <fmt/format.h>
#include <range/v3/view/reverse.hpp>
// std
#include <iostream>
#include <map>
#include <set>
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
		bool init = false;
	};
	std::vector<Value> enum_entries;

	struct Property {
		std::string type;
		std::map<std::string, std::string> values;
	};
	std::map<std::string, Property> properties;

	std::vector<std::string> namespaces;
	std::set<std::string> includes;

//	std::set<std::string> includes_hpp;
//	std::set<std::string> includes_cpp;

//	std::vector<std::string> functions;
//
//	struct Info {
//		std::string name;
//		std::string type;
//		std::string value;
//	};
//	std::vector<Info> infos;

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
//	void function(std::string_view fmt_function) {
//		functions.emplace_back(fmt_function);
//	}

//	void value(std::string_view name, std::string_view value, bool init_value_ = false) {
//		values.emplace_back(std::string(name), std::string(value), init_value_);
//		longest_value_name = std::max(longest_value_name, name.size());
//	}

	void add_namespace(std::string_view namespace_str) {
		namespaces.emplace_back(namespace_str);
	}
	void include(std::string_view include_str) {
		includes.emplace(include_str);
	}
	void value(std::string_view name, std::string_view text, bool init_value_ = false) {
		enum_entries.emplace_back(std::string(name), std::to_string(enum_entries.size()), std::string(text), init_value_);
		longest_value_name = std::max(longest_value_name, name.size());
		longest_value_text = std::max(longest_value_text, text.size());
	}
	void property(std::string_view name, std::string_view type) {
		properties.emplace(std::string(name), std::string(type));
	}
	void property_value(std::string_view property, std::string_view enum_, std::string_view value) {
		properties[std::string(property)].values.emplace(std::string(enum_), std::string(value));
	}

//	void info_stub(std::string_view info_name, std::string_view info_type, std::string_view value) {
//		infos.emplace_back(std::string(info_name), std::string(info_type), std::string(value));
//	}

	// -------------------------------------------------------------------------------------------------

private:
	template <typename... Args>
	void out(std::string_view fmt, Args&&... args) {
		std::cout << fmt::format(fmt, std::forward<Args>(args)...);
//		os << fmt::format(fmt, std::forward<Args>(args)...);
	}

public:
	std::string make() {

//		if (to_string) {
//			includes_cpp.emplace("cassert");
//			includes_hpp.emplace("string_view");
//		}
//
//		if (to_stream) {
//			includes_hpp.emplace("iosfwd");
//			includes_hpp.emplace("string_view");
//			includes_cpp.emplace("ostream");
//		}
//
//		if (to_span) {
//			includes_hpp.emplace("span");
//		}

		out("// First line\n");
		out("// Enum generator version: {}\n", enum_gen_version);

//		out("// std\n");
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
		out("struct {} {{\n", class_enum_type_name);
		out("private:\n");
		out("	{} enum_value_;\n", enum_name);

		out("\n");
		out("public:\n");
		out("	/* implicit */ constexpr inline {}({} value) noexcept :\n", class_enum_type_name, enum_name);
		out("		enum_value_(value) {{\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline operator {}() const noexcept {{\n", enum_name);
		out("		return enum_value_;\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline {} enum_value() const noexcept {{\n", enum_name);
		out("		return enum_value_;\n");
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline {} underlying() const noexcept {{\n", enum_type);
		out("		return static_cast<{}>(enum_value_);\n", enum_type);
		out("	}}\n");
		out("\n");
		out("	[[nodiscard]] constexpr inline {} value() const noexcept {{\n", enum_name);
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
			for (const auto& enum_entry : enum_entries)
				out("			\"{}\", // {}\n", enum_entry.text, enum_entry.identifier);
			out("	}};\n");
			out("\n");
			out("public:\n");
			out("	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {{\n");
			out("		assert(underlying() >= 0 && underlying() < {});\n", enum_entries.size());
			out("		return table_to_string[underlying()];\n");
			out("	}}\n");

			if (gen_to_stream) {
				out("\n");
				out("	friend std::ostream& operator<<(std::ostream& os, const {} var) {{\n", class_enum_type_name);
				out("		return os << var.to_string();\n");
				out("	}}\n");
			}
		}

		// TODO P1: parse

		for (const auto& [property_name, property] : properties) {
			out("\n");
			out("private:\n");
			out("	static constexpr {} table_{}[] = {{\n", property.type, property_name);
			for (const auto& enum_entry : enum_entries)
				out("			{}, // {}\n", property.values.at(enum_entry.identifier), enum_entry.identifier);
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
		out("class {} {{\n", class_enum_state_name);
		out("private:\n");
		out("	static constexpr {} table_enum_values[] = {{\n", enum_name);
		for (const auto& enum_entry : enum_entries)
			out("			{}::{},\n", enum_name, enum_entry.identifier);
		out("	}};\n");
		out("\n");
		out("public:\n");
		out("	static constexpr size_t size = {};\n", enum_entries.size());

		if (gen_to_span) {
			out("\n");
			out("	[[nodiscard]] static std::span<const {}> values() noexcept {{\n", enum_name);
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

		if (gen_to_string) {
			out("\n");
			out("[[nodiscard]] constexpr inline std::string_view to_string({} enum_value) noexcept {{\n", enum_name);
			out("	return {}(enum_value).to_string();\n", class_enum_type_name);
			out("}}\n");
		}

		out("\n");
		out("[[nodiscard]] constexpr inline {} type({} enum_value) noexcept {{\n", class_enum_type_name, enum_name);
		out("	return {}(enum_value);\n", class_enum_type_name);
		out("}}\n");

		out("\n");
		out("static constexpr {} {};\n", class_enum_state_name, global_var_enum_state_name);

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");
		out("\n");

		for (const auto& ns : namespaces | ranges::view::reverse)
			out("}} // namespace {}\n", ns);

		std::string result;
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
	EnumBuilder eb("color", "int32_t");

//	eb.gen_operator_eq = true;
//	eb.gen_operator_rel = true;
	eb.gen_to_string = true;
	eb.gen_to_stream = true;
	eb.gen_to_span = true;

	eb.add_namespace("libv::ns");
	eb.add_namespace("color");
	eb.include("libv/math/vec.hpp");

	eb.property("rgb", "libv::vec4f");
	eb.value("red", "red", false);     eb.property_value("rgb", "red",   "libv::vec4f{1, 0, 0, 1}");
	eb.value("green", "green", false); eb.property_value("rgb", "green", "libv::vec4f{0, 1, 0, 1}");
	eb.value("blue", "blue", false);   eb.property_value("rgb", "blue",  "libv::vec4f{0, 0, 1, 1}");
	eb.make();

//	clipboard(result);
//	std::cout << result << std::endl;

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
