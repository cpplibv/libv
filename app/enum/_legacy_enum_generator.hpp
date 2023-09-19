// Project: libv, File: app/enum/_legacy_enum_generator.hpp

// ext
#include <clip/clip.h>
#include <fmt/format.h>
// std
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>


// -------------------------------------------------------------------------------------------------

static constexpr std::string_view enum_gen_version = "v5";

void clipboard(const std::string& string) {
	bool success = clip::set_text(string);
	if (!success)
		std::cerr << fmt::format("Failed to set clipboard text of length {}: \"{}\"", string.size(), string) << std::endl;
}

// -------------------------------------------------------------------------------------------------

const auto fmt_ref_declaration = R"(	static const {enum_name}& {name};
)";

const auto fmt_ref_def = R"(inline const {enum_name}& {enum_name}::{name:{longest_value_name}}{{detail::storage_{enum_name}[{index}]}};
)";

const auto fmt_value_objects = R"(	{enum_name}{{{value}}}, // {name}
)";

const auto fmt_operator =
R"(	[[nodiscard]] friend constexpr inline bool operator{operator}(const {enum_name}& lhs, const {enum_name}& rhs) noexcept {{
		return lhs.value {operator} rhs.value;
	}}
)";

const auto fmt_enum_body =
R"({includes_str}/// Enum generator version: {enum_gen_version}
struct {enum_name} {{
	using value_type = {enum_type};

public:
{ref_decl}
private:
    value_type value{{{init_value}}};{init_comment}

public:
    constexpr inline {enum_name}() noexcept = default;
    constexpr inline {enum_name}(const {enum_name}& other) noexcept = default;
    constexpr inline {enum_name}({enum_name}&& other) noexcept = default;
    constexpr inline {enum_name}& operator=(const {enum_name}& other) & noexcept = default;
    constexpr inline {enum_name}& operator=({enum_name}&& other) & noexcept = default;
    inline ~{enum_name}() noexcept = default;

    explicit constexpr inline {enum_name}(value_type value) : value{{value}} {{ }}

	constexpr inline operator value_type() const noexcept {{
        return value;
    }}

	[[nodiscard]] constexpr inline value_type to_value() const noexcept {{
        return value;
    }}
{member_functions}}};

// <editor-fold defaultstate="collapsed" desc="{enum_name} constant declarations ...">

namespace detail {{
static constexpr {enum_name} storage_{enum_name}[] = {{
{value_objects}}};
}} // namespace detail

{ref_def}
// </editor-fold>

{free_functions})";

// -------------------------------------------------------------------------------------------------

class MyEnumBuilder {
private:
	std::string	enum_name;
	std::string	enum_type;

	struct Value {
		std::string name;
		std::string value;
		std::string text;
		bool init = false;
	};
	std::vector<Value> values;

	std::set<std::string> includes_hpp;
	std::set<std::string> includes_cpp;

	std::vector<std::string> functions;

	struct Info {
		std::string name;
		std::string type;
		std::string value;
	};
	std::vector<Info> infos;

private:
	const Value& find_init() {
		for (const auto& v : values)
			if (v.init)
				return v;

		static Value init_fallback{"", "", "", false};
		return init_fallback;
	}

private:
	std::string includes_str;
	std::string member_functions;
	std::string free_functions;
	std::string cpp_functions;
	std::string ref_decl;
	std::string ref_def;
	std::string value_objects;
	std::size_t longest_value_name = 0;
	std::size_t longest_value_text = 0;

public:
	bool operator_eq = true;
	bool operator_rel = true;
	bool to_string = true;
	bool to_stream = true;
	bool to_span = true;

public:
	explicit MyEnumBuilder(std::string enum_name = "MyEnum", std::string enum_type = "int") :
		enum_name(std::move(enum_name)),
		enum_type(std::move(enum_type)) { }

private:
	void make_value(std::string_view name, std::string_view value, std::size_t index) {
		ref_decl += fmt::format(fmt::runtime(fmt_ref_declaration),
				fmt::arg("enum_name", enum_name),
				fmt::arg("name", name),
				fmt::arg("value", value)
		);
		ref_def += fmt::format(fmt::runtime(fmt_ref_def),
				fmt::arg("enum_name", enum_name),
				fmt::arg("name", name),
				fmt::arg("longest_value_name", longest_value_name),
				fmt::arg("index", index)
		);
		value_objects += fmt::format(fmt::runtime(fmt_value_objects),
				fmt::arg("enum_name", enum_name),
				fmt::arg("name", name),
				fmt::arg("value", value)
		);
	}

	void make_op_function(std::string_view fmt_function, std::string_view operator_) {
		member_functions += fmt::format(fmt::runtime(fmt_function),
				fmt::arg("enum_name", enum_name),
				fmt::arg("enum_type", enum_type),
				fmt::arg("operator", operator_)
		);
	}

//	void make_to_string() {
//		const auto fmt_to_string_decl =
//				"	[[nodiscard]] std::string_view to_string() const noexcept;\n";
//		const auto fmt_to_string_def =
//R"(std::string_view {enum_name}::to_string() const noexcept {{
//	switch(value) {{{cases}
//	}}
//
//	assert(false && "Invalid {enum_name} enum value");
//	return "--invalid--";
//}}
//)";
//		const auto fmt_to_string_case = R"(
//		case {enum_name}::{name}: return "{name}";)";
//
//		std::string cases;
//
//		for (const auto& kv : values)
//			cases += fmt::format(fmt::runtime(fmt_to_string_case),
//					fmt::arg("enum_name", enum_name),
//					fmt::arg("name", kv.name)
//			);
//
//		member_functions += fmt::format(fmt::runtime(fmt_to_string_decl));
//		cpp_functions += fmt::format(fmt::runtime(fmt_to_string_def),
//				fmt::arg("enum_name", enum_name),
//				fmt::arg("cases", cases)
//		);
//	}

public:
	void function(std::string_view fmt_function) {
		functions.emplace_back(fmt_function);
	}

//	void value(std::string_view name, std::string_view value, bool init_value_ = false) {
//		values.emplace_back(std::string(name), std::string(value), init_value_);
//		longest_value_name = std::max(longest_value_name, name.size());
//	}
	void value(std::string_view name, std::string_view text, bool init_value_ = false) {
		values.emplace_back(std::string(name), std::to_string(values.size()), std::string(text), init_value_);
		longest_value_name = std::max(longest_value_name, name.size());
		longest_value_text = std::max(longest_value_text, text.size());
	}

	void info_stub(std::string_view info_name, std::string_view info_type, std::string_view value) {
		infos.emplace_back(std::string(info_name), std::string(info_type), std::string(value));
	}

public:
	std::string make() {
		for (std::size_t i = 0; const auto& kv : values)
			make_value(kv.name, kv.value, i++);

		if (operator_eq || operator_rel) {
			member_functions += "\npublic:\n";

			if (operator_eq) {
				make_op_function(fmt_operator, "==");
				make_op_function(fmt_operator, "!=");
			}

			if (operator_rel) {
				make_op_function(fmt_operator, "<");
				make_op_function(fmt_operator, "<=");
				make_op_function(fmt_operator, ">");
				make_op_function(fmt_operator, ">=");
			}
		}

		if (to_string || to_stream) {
			member_functions += "\npublic:\n";

			if (to_string) {
				includes_cpp.emplace("cassert");
				includes_hpp.emplace("string_view");

				member_functions += "	[[nodiscard]] std::string_view to_string() const noexcept;\n";

				cpp_functions += fmt::format(
						"\n"
						"static constexpr std::string_view storage_{enum_name}_to_string[] = {{\n",
						fmt::arg("enum_name", enum_name)
				);
				for (const auto& kv : values)
					cpp_functions += fmt::format("	{name_sp:{longest_value_text}}, // {name}\n",
							fmt::arg("name", kv.name),
							fmt::arg("name_sp", '"' + kv.text + '"'),
							fmt::arg("longest_value_text", longest_value_text + 2)
					);
				cpp_functions += "};\n";

				cpp_functions += fmt::format(
						"\n"
						"std::string_view {enum_name}::to_string() const noexcept {{\n"
						"	if (value >= std::size(storage_{enum_name}_to_string)) {{\n"
						"		assert(false && \"Invalid {enum_name} enum value\");\n"
						"		return \"<<invalid-{enum_name}-enum-value>>\";\n"
						"	}}\n"
						"	return storage_{enum_name}_to_string[value];\n"
						"}}\n",
						fmt::arg("enum_name", enum_name)
				);
			}

			if (to_stream) {
				includes_hpp.emplace("iosfwd");
				includes_cpp.emplace("ostream");

				member_functions += fmt::format(
						"	friend std::ostream& operator<<(std::ostream& os, const {enum_name}& var);\n",
						fmt::arg("enum_name", enum_name));
				cpp_functions += fmt::format(
						"\n"
						"std::ostream& operator<<(std::ostream& os, const {enum_name}& var) {{\n"
						"	return os << var.to_string();\n"
						"}}\n",
						fmt::arg("enum_name", enum_name));

			}
		}

		if (to_span) {
			includes_hpp.emplace("span");

			member_functions += "\npublic:\n";
			member_functions += fmt::format(
					"	[[nodiscard]] static std::span<const {enum_name}> list() noexcept;\n",
					fmt::arg("enum_name", enum_name));
			cpp_functions += fmt::format(
					"\n"
					"std::span<const {enum_name}> {enum_name}::list() noexcept {{\n"
					"	return detail::storage_{enum_name};\n"
					"}}\n",
					fmt::arg("enum_name", enum_name));
		}

		if (!functions.empty()) {
			member_functions += "\npublic:\n";
			for (const auto& fmt_fn : functions)
				member_functions += fmt::format(fmt::runtime(fmt_fn),
						fmt::arg("enum_name", enum_name),
						fmt::arg("enum_type", enum_type)
				);
		}

		if (!infos.empty()) {
			includes_cpp.emplace("cassert");

			member_functions += "\npublic:\n";
			for (const Info& info : infos) {
				member_functions += fmt::format(
						"	[[nodiscard]] {info_type} {info_name}() const noexcept;\n",
						fmt::arg("info_name", info.name),
						fmt::arg("info_type", info.type)
				);

				cpp_functions += fmt::format(
						"\n"
						"static constexpr {info_type} storage_{enum_name}_{info_name}[] = {{\n",
						fmt::arg("info_name", info.name),
						fmt::arg("info_type", info.type),
						fmt::arg("enum_name", enum_name)
				);
				for (const auto& kv : values) {
					cpp_functions += fmt::format("	{value}, // {name}\n",
							fmt::arg("value", info.value),
							fmt::arg("name", kv.name)
					);
				}
				cpp_functions += "};\n";

				cpp_functions += fmt::format(
						"\n"
						"{info_type} {enum_name}::{info_name}() const noexcept {{\n"
						"	if (value >= std::size(storage_{enum_name}_{info_name})) {{\n"
						"		assert(false && \"Invalid {enum_name} enum value\");\n"
						"		return storage_{enum_name}_{info_name}[0];\n"
						"	}}\n"
						"	return storage_{enum_name}_{info_name}[value];\n"
						"}}\n",
						fmt::arg("enum_name", enum_name),
						fmt::arg("info_name", info.name),
						fmt::arg("info_type", info.type)
				);
			}
		}

		// ---

		if (!includes_hpp.empty()) {
			includes_str = "// std\n";
			for (const auto& file : includes_hpp)
				includes_str += "#include <" + file + ">\n";
			includes_str += "\n\n";
		}

		auto result = fmt::format(fmt::runtime(fmt_enum_body),
				fmt::arg("enum_gen_version", enum_gen_version),
				fmt::arg("includes_str", includes_str),
				fmt::arg("enum_name", enum_name),
				fmt::arg("enum_type", enum_type),
				fmt::arg("init_value", find_init().value),
				fmt::arg("init_comment", find_init().init ? " // " + find_init().name : ""),
				fmt::arg("member_functions", member_functions),
				fmt::arg("free_functions", free_functions),
				fmt::arg("ref_decl", ref_decl),
				fmt::arg("ref_def", ref_def),
				fmt::arg("value_objects", value_objects)
		);

		if (cpp_functions.empty())
			return result;

		if (!includes_cpp.empty()) {
			result += "// === CPP FILE ====================================================================================\n";
			result += "\n";

			result += "// hpp\n";
			result += "// HPP FILE\n";
			result += "// std\n";
			for (const auto& file : includes_cpp)
				result += "#include <" + file + ">\n";
			result += "\n";
		}

		result += cpp_functions;

		return result;
	}
};

auto gen_anchor() {
	MyEnumBuilder builder{"Anchor", "uint16_t"};

	builder.operator_eq = true;
	builder.operator_rel = true;
	builder.to_string = true;
	builder.to_stream = true;
	builder.to_span = true;

	builder.value("top_left"     , "top-left"     , true);
	builder.value("top_center"   , "top-center"   );
	builder.value("top_right"    , "top-right"    );
	builder.value("center_left"  , "center-left"  );
	builder.value("center_center", "center-center");
	builder.value("center_right" , "center-right" );
	builder.value("bottom_left"  , "bottom-left"  );
	builder.value("bottom_center", "bottom-center");
	builder.value("bottom_right" , "bottom-right" );

	builder.info_stub("to_info", "libv::vec3f", "libv::vec3f{0.0f, 0.0f, 0.0f}");

//	builder.value("top_left"     , "top-left"     , "libv::vec3f{0.0f, 1.0f, 0.0f}", true);
//	builder.value("top_center"   , "top-center"   , "libv::vec3f{0.5f, 1.0f, 0.0f}");
//	builder.value("top_right"    , "top-right"    , "libv::vec3f{1.0f, 1.0f, 0.0f}");
//	builder.value("center_left"  , "center-left"  , "libv::vec3f{0.0f, 0.5f, 0.0f}");
//	builder.value("center_center", "center-center", "libv::vec3f{0.5f, 0.5f, 0.0f}");
//	builder.value("center_right" , "center-right" , "libv::vec3f{1.0f, 0.5f, 0.0f}");
//	builder.value("bottom_left"  , "bottom-left"  , "libv::vec3f{0.0f, 0.0f, 0.0f}");
//	builder.value("bottom_center", "bottom-center", "libv::vec3f{0.5f, 0.0f, 0.0f}");
//	builder.value("bottom_right" , "bottom-right" , "libv::vec3f{1.0f, 0.0f, 0.0f}");
//
//	builder.info_stub(0, "to_info", "libv::vec3f");

	return builder.make();
}

auto gen_align_vertical() {
	MyEnumBuilder builder{"AlignVertical", "uint16_t"};

	builder.value("top"        , "top"        , true);
	builder.value("center"     , "center"     );
	builder.value("bottom"     , "bottom"     );
	builder.value("justify"    , "justify"    );
	builder.value("justify_all", "justify-all");

	builder.info_stub("to_info", "float", "0.0f");

	return builder.make();
}

auto gen_align_horizontal() {
	MyEnumBuilder builder{"AlignHorizontal", "uint16_t"};

	builder.value("left"       , "left"       , true);
	builder.value("center"     , "center"     );
	builder.value("right"      , "right"      );
	builder.value("justify"    , "justify"    );
	builder.value("justify_all", "justify-all");

	builder.info_stub("to_info", "float", "0.0f");

	return builder.make();
}

int main() {
//	auto result = gen_anchor();
//	auto result = gen_align_vertical();
	auto result = gen_align_horizontal();
	clipboard(result);
	std::cout << result << std::endl;

	return 0;
}
