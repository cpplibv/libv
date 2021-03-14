// Project: libv, File: app/enum/enum_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <clip/clip.h>
#include <fmt/format.h>
#include <range/v3/view/reverse.hpp>
// std
#include <iostream>
#include <set>
#include <string>
#include <string_view>


// -------------------------------------------------------------------------------------------------

static constexpr std::string_view enum_gen_version = "v10";

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
//	bool gen_to_span = true;

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
//			includes_cpp.emplace("ostream");
//		}
//
//		if (to_span) {
//			includes_hpp.emplace("span");
//		}

		out("// First line");
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
		out("   {} enum_value_;\n", enum_name);

		out("\n");
		out("public:\n");
		out("   /* implicit */ constexpr inline {}({} value) noexcept :\n", class_enum_type_name, enum_name);
		out("       enum_value_(value) {{\n");
		out("   }}\n");
		out("\n");
		out("   [[nodiscard]] constexpr inline operator {}() const noexcept {{\n", enum_name);
		out("   	return enum_value_;\n");
		out("   }}\n");
		out("\n");
		out("   [[nodiscard]] constexpr inline {} enum_value() const noexcept {{\n", enum_name);
		out("       return enum_value_;\n");
		out("   }}\n");
		out("\n");
		out("   [[nodiscard]] constexpr inline {} underlying() const noexcept {{\n", enum_type);
		out("       return static_cast<{}>(enum_value_);\n", enum_type);
		out("   }}\n");
		out("\n");
		out("   [[nodiscard]] constexpr inline {} value() const noexcept {{\n", enum_name);
		out("       return enum_value();\n");
		out("   }}\n");

//		out("\n");
//		out("public:\n");

//		if (operator_eq || operator_rel) {
//			member_functions += "\npublic:\n";
//
//			if (operator_eq) {
//				make_op_function(fmt_operator, "==");
//				make_op_function(fmt_operator, "!=");
//			}
//
//			if (operator_rel) {
//				make_op_function(fmt_operator, "<");
//				make_op_function(fmt_operator, "<=");
//				make_op_function(fmt_operator, ">");
//				make_op_function(fmt_operator, ">=");
//			}
//		}

		if (gen_to_string || gen_to_stream) {
			out("\n");
			out("private:\n");
			out("	static constexpr std::string_view table_to_string[] = {{\n");
			for (const auto& enum_entry : enum_entries)
				out("			\"{}\",\n", enum_entry.text);
			out("	}};\n");
			out("public:\n");
			out("	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {{\n");
			out("		assert(underlying() >= 0 && underlying() < {});\n", enum_entries.size());
			out("		return table_to_string[underlying()];\n");
			out("	}}\n");
		}

		if (gen_to_stream) {

		}

		out("}};\n"); // End of enum_type class

		out("\n");
		out("[[nodiscard]] constexpr inline {} type({} enum_value) noexcept {{\n", class_enum_type_name, enum_name);
		out("	return {}(enum_value);\n", class_enum_type_name);
		out("}}\n");

		out("\n");
		out("class {} {{\n", class_enum_state_name);
		out("	static constexpr {} table[] = {{\n", enum_name);
		for (const auto& enum_entry : enum_entries)
			out("			{}::{},\n", enum_name, enum_entry.identifier);
		out("	}};\n");
		out("\n");
		out("public:\n");
		out("	static constexpr size_t size = {};\n", enum_entries.size());

		out("\n");
		out("	// RANGE INTERFACE STUFF\n"); // Range interfaces over table
		out("}};\n");

		out("\n");
		out("static constexpr {} {};\n", class_enum_state_name, global_var_enum_state_name);

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");
		out("\n");

		for (const auto& ns : namespaces | ranges::view::reverse)
			out("}} // namespace {}\n", ns);

//			if (to_stream) {
//				includes_hpp.emplace("iosfwd");
//				includes_cpp.emplace("ostream");
//
//				member_functions += fmt::format(
//						"	friend std::ostream& operator<<(std::ostream& os, const {enum_name}& var);\n",
//						fmt::arg("enum_name", enum_name));
//				cpp_functions += fmt::format(
//						"\n"
//						"std::ostream& operator<<(std::ostream& os, const {enum_name}& var) {{\n"
//						"	return os << var.to_string();\n"
//						"}}\n",
//						fmt::arg("enum_name", enum_name));
//
//			}
//		}
//
//		if (to_span) {
//			includes_hpp.emplace("span");
//
//			member_functions += "\npublic:\n";
//			member_functions += fmt::format(
//					"	[[nodiscard]] static std::span<const {enum_name}> list() noexcept;\n",
//					fmt::arg("enum_name", enum_name));
//			cpp_functions += fmt::format(
//					"\n"
//					"std::span<const {enum_name}> {enum_name}::list() noexcept {{\n"
//					"	return detail::storage_{enum_name};\n"
//					"}}\n",
//					fmt::arg("enum_name", enum_name));
//		}
//
//		if (!functions.empty()) {
//			member_functions += "\npublic:\n";
//			for (const auto& fmt_fn : functions)
//				member_functions += fmt::format(fmt_fn,
//						fmt::arg("enum_name", enum_name),
//						fmt::arg("enum_type", enum_type)
//				);
//		}
//
//		if (!infos.empty()) {
//			includes_cpp.emplace("cassert");
//
//			member_functions += "\npublic:\n";
//			for (const Info& info : infos) {
//				member_functions += fmt::format(
//						"	[[nodiscard]] {info_type} {info_name}() const noexcept;\n",
//						fmt::arg("info_name", info.name),
//						fmt::arg("info_type", info.type)
//				);
//
//				cpp_functions += fmt::format(
//						"\n"
//						"static constexpr {info_type} storage_{enum_name}_{info_name}[] = {{\n",
//						fmt::arg("info_name", info.name),
//						fmt::arg("info_type", info.type),
//						fmt::arg("enum_name", enum_name)
//				);
//				for (const auto& kv : values) {
//					cpp_functions += fmt::format("	{value}, // {name}\n",
//							fmt::arg("value", info.value),
//							fmt::arg("name", kv.name)
//					);
//				}
//				cpp_functions += "};\n";
//
//				cpp_functions += fmt::format(
//						"\n"
//						"{info_type} {enum_name}::{info_name}() const noexcept {{\n"
//						"	if (value >= std::size(storage_{enum_name}_{info_name})) {{\n"
//						"		assert(false && \"Invalid {enum_name} enum value\");\n"
//						"		return storage_{enum_name}_{info_name}[0];\n"
//						"	}}\n"
//						"	return storage_{enum_name}_{info_name}[value];\n"
//						"}}\n",
//						fmt::arg("enum_name", enum_name),
//						fmt::arg("info_name", info.name),
//						fmt::arg("info_type", info.type)
//				);
//			}
//		}

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

	//builder.operator_eq = true;
	//builder.operator_rel = true;
	//builder.to_string = true;
	//builder.to_stream = true;
	//builder.to_span = true;

	eb.add_namespace("libv::ns");
	eb.add_namespace("color");
	eb.include("libv/math/vec.hpp");

//	property("rgb", "libv::vec4f")
	eb.value("red", "red", false);
	eb.value("green", "green", false);
	eb.value("blue", "blue", false);
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
		property("rgb", "libv::vec4f")
		custom_function("int calc")
		values{
				red = { rgba("255, 0, 0, 255") },
				green = { rgba("0, 255, 0, 255") },
				blue = { rgba("0, 0, 255, 255") },
		)

	OUTPUT
		see example.hpp

	CONSTEXPR MAP
		#include <array>
		#include <string_view>
		#include <stdexcept>
		#include <algorithm>
		#include <map>

		template <typename Key, typename Value, std::size_t Size>
		struct ConstexprMap {
		  std::array<std::pair<Key, Value>, Size> data;

		  [[nodiscard]] constexpr Value at(const Key &key) const {
			const auto itr =
				std::find_if(begin(data), end(data),
							 [&key](const auto &v) { return v.first == key; });
			if (itr != end(data)) {
			  return itr->second;
			} else {
			  throw std::range_error("Not Found");
			}
		  }

		};

		using namespace std::literals::string_view_literals;
		static constexpr std::array<std::pair<std::string_view, int>, 8> color_values{
			{{"black"sv, 7},
			 {"blue"sv, 3},
			 {"cyan"sv, 5},
			 {"green"sv, 2},
			 {"magenta"sv, 6},
			 {"red"sv, 1},
			 {"white"sv, 8},
			 {"yellow"sv, 4}}};

		int lookup_value(const std::string_view sv) {
		  //static const auto map = std::map<std::string_view, int>{color_values.begin(), color_values.end()};
		  static constexpr auto map =
			  Map<std::string_view, int, color_values.size()>{{color_values}};

		  return map.at(sv);
		}
 */
