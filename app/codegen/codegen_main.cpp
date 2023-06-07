// Project: libv, File: app/codegen/codegen_main.cpp

// libv
#include <libv/algo/linear_find.hpp>
#include <libv/algo/slice.hpp>
#include <libv/lua/lua.hpp>
#include <libv/utility/projection.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// ext
#include <sol/state.hpp>
#include <fmt/format.h>
//#include <range/v3/view/reverse.hpp>
// std
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


// -------------------------------------------------------------------------------------------------

constexpr inline std::string_view codegen_version = "v5.0.0";

// -------------------------------------------------------------------------------------------------

class SourceGenerator_v1 {
public:
	struct StaticMemberEntry {
		std::string type;
		std::string identifier;
		std::string init_value;
	};

	struct MemberEntry {
		std::string type;
		std::string identifier;
		std::string init_value;
		bool use_move = false; /// Use std::move in ctor
	};

	struct ClazzEntry {
		std::string identifier;
		bool struct_ = false;

		std::string scope_class;
		std::vector<StaticMemberEntry> static_members;
		std::vector<MemberEntry> members;
		std::vector<std::shared_ptr<ClazzEntry>> member_classes;

//		std::vector<std::function<void(const ClazzEntry&)>> generators;

		ClazzEntry(std::string identifier, bool struct_) :
			identifier(std::move(identifier)),
			struct_(struct_) {}
	};

	using clazz_hook_type = std::function<void(std::function<void(std::string_view)>, const SourceGenerator_v1::ClazzEntry&)>;
	using file_hook_type = std::function<void(std::function<void(std::string_view)>)>;

private:
	std::vector<std::string> namespaces;
	std::set<std::string> includes_hpp;
	std::set<std::string> includes_cpp;

	std::vector<std::shared_ptr<ClazzEntry>> global_classes;
	std::vector<clazz_hook_type> class_hooks_hpp;
	std::vector<clazz_hook_type> class_hooks_cpp;
	std::vector<file_hook_type> file_hooks_hpp;
	std::vector<file_hook_type> file_hooks_cpp;
	std::map<std::string, std::vector<file_hook_type>> file_hooks_external;

	bool first_cpp_class = true;

public:
	void add_namespace(std::string namespace_str) {
		namespaces.emplace_back(std::move(namespace_str));
	}
	void include(std::string include_str) {
		includes_hpp.emplace(include_str);
		includes_cpp.emplace(std::move(include_str));
	}
	void include_hpp(std::string include_str) {
		includes_hpp.emplace(std::move(include_str));
	}
	void include_cpp(std::string include_str) {
		includes_cpp.emplace(std::move(include_str));
	}
	auto add_class(std::string name, bool struct_) {
		auto clazz = std::make_shared<ClazzEntry>(std::move(name), struct_);
		global_classes.emplace_back(clazz);
		return clazz;
	}
	auto grab_class_to_member(const std::string& name) {
		auto it = libv::linear_find_iterator(global_classes, name, libv::proj_indirect(&ClazzEntry::identifier));
		auto clazz = *it;
		global_classes.erase(it);
		return clazz;
	}
	void add_class_hook_hpp(clazz_hook_type hook) {
		class_hooks_hpp.emplace_back(std::move(hook));
	}

	void add_class_hook_cpp(clazz_hook_type hook) {
		class_hooks_cpp.emplace_back(std::move(hook));
	}

	void add_file_hook_hpp(file_hook_type hook) {
		file_hooks_hpp.emplace_back(std::move(hook));
	}
	void add_file_hook_cpp(file_hook_type hook) {
		file_hooks_cpp.emplace_back(std::move(hook));
	}
	void add_file_hook_external(std::string_view filepath, file_hook_type hook) {
		file_hooks_external[std::string(filepath)].emplace_back(std::move(hook));
	}

	template <typename Out>
	void generate_hpp_class(const ClazzEntry& clazz, Out& out, int depth = 0) {
		std::string ident(depth, '\t');

		out("{}{} {} {{\n", ident, clazz.struct_ ? "struct" : "class", clazz.identifier);

		for (const auto& member_clazz : clazz.member_classes)
			generate_hpp_class(*member_clazz, out, depth + 1);
		if (!clazz.member_classes.empty())
			out("\n");

		for (const auto& member : clazz.static_members)
			out("{}\tstatic constexpr {} {}{{{}}};\n", ident, member.type, member.identifier, member.init_value);
		if (!clazz.static_members.empty())
			out("\n");

		for (const auto& member : clazz.members)
			if (member.init_value.empty())
				out("{}\t{} {};\n", ident, member.type, member.identifier);
			else
				out("{}\t{} {} = {};\n", ident, member.type, member.identifier, member.init_value);

		for (const auto& hook : class_hooks_hpp)
			hook([&](const std::string_view str) {
				if (str == "\n")
					out("{}", str);
				else
					out("{}{}", ident, str);
			}, clazz);

		out("{}}};\n", ident);
	}

	template <typename Out>
	void generate_cpp_class(const ClazzEntry& clazz, Out& out, int depth = 0) {
//		if (clazz.generators_cpp.empty())
		if (class_hooks_cpp.empty())
			return;

//		std::string ident(depth, '\t');

//		out("{}{} {} {{\n", ident, clazz.struct_ ? "struct" : "class", clazz.identifier);

		for (const auto& member_clazz : clazz.member_classes)
			generate_cpp_class(*member_clazz, out, depth + 1);
//		if (!clazz.member_classes.empty())
//			out("\n");

		if (!first_cpp_class) {
			out("\n");
			out("// -------------------------------------------------------------------------------------------------\n");
		}
		first_cpp_class = false;

//		for (const auto& member : clazz.members)
//			if (member.init_value.empty())
//				out("{}\t{} {};\n", ident, member.type, member.identifier);
//			else
//				out("{}\t{} {} = {};\n", ident, member.type, member.identifier, member.init_value);

		for (const auto& hook : class_hooks_cpp)
			hook([&](const std::string_view str) {
				out("{}", str);
			}, clazz);
//
//		out("{}}};\n", ident);
	}

	void finalize_class_scopes(ClazzEntry& clazz, std::string scope) {
		clazz.scope_class = scope;
		for (auto& clazz : clazz.member_classes)
			finalize_class_scopes(*clazz, scope + "::" + clazz->identifier);
	}

	void finalize_class_scopes() {
		for (auto& clazz : global_classes)
			finalize_class_scopes(*clazz, clazz->identifier);
	}

public:
	[[nodiscard]] std::string generate_hpp(std::string_view input_file) {
		finalize_class_scopes();

		std::ostringstream os;

		const auto out = [&os](std::string_view fmt, auto&&... args) {
			os << fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...);
		};

		out("//\n"); // Empty first line to allow first line watermark
		out("// Generated source code\n");
		out("// Generator version: {}\n", codegen_version);
		if (!input_file.empty())
			out("// Input file: {}\n", input_file);
		out("\n");
		out("#pragma once\n");

		if (!includes_hpp.empty()) {
			out("\n");
			out("//\n");
		}
		for (const auto& in : includes_hpp)
			out("#include <{}>\n", in);

		out("\n");
		out("\n");

		for (const auto& ns : namespaces)
			out("namespace {} {{\n", ns);
		if (!namespaces.empty())
			out("\n");

		out("// -------------------------------------------------------------------------------------------------\n");

		for (const auto& hook : file_hooks_hpp)
			hook([&](const std::string_view str) {
				out("{}", str);
			});

		for (const auto& clazz : global_classes) {
			out("\n");
			generate_hpp_class(*clazz, out);
		}

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");

		if (!namespaces.empty())
			out("\n");

		for (std::size_t i = namespaces.size(); i > 0; --i)
			out("}} // namespace {}\n", namespaces[i - 1]);

		return std::move(os).str();
	}

	[[nodiscard]] std::string generate_cpp(std::string_view input_file) {
		finalize_class_scopes();

		std::ostringstream os;

		const auto out = [&os](std::string_view fmt, auto&&... args) {
			os << fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...);
		};

		out("//\n"); // Empty first line to allow first line watermark
		out("// Generated source code\n");
		out("// Generator version: {}\n", codegen_version);
		if (!input_file.empty())
			out("// Input file: {}\n", input_file);

		if (!includes_cpp.empty()) {
			out("\n");
			out("//\n");
		}
		for (const auto& in : includes_cpp)
			out("#include <{}>\n", in);

		out("\n");
		out("\n");

		for (const auto& ns : namespaces)
			out("namespace {} {{\n\n", ns);

		out("// -------------------------------------------------------------------------------------------------\n");

		for (const auto& hook : file_hooks_cpp)
			hook([&](const std::string_view str) {
				out("{}", str);
			});

		for (const auto& clazz : global_classes) {
			generate_cpp_class(*clazz, out);
		}

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");

		if (!namespaces.empty())
			out("\n");
		for (std::size_t i = namespaces.size(); i > 0; --i)
			out("}} // namespace {}\n", namespaces[i - 1]);

		return std::move(os).str();
	}

	[[nodiscard]] std::map<std::string, std::string> generate_external(std::string_view input_file) {
		std::map<std::string, std::string> result;

		for (const auto& [filepath, hooks] : file_hooks_external) {
			std::ostringstream os;

			const auto out = [&os](std::string_view fmt, auto&&... args) {
				os << fmt::format(fmt::runtime(fmt), std::forward<decltype(args)>(args)...);
			};

			out("//\n"); // Empty first line to allow first line watermark
			out("// Generated source code\n");
			out("// Generator version: {}\n", codegen_version);
			if (!input_file.empty())
				out("// Input file: {}\n", input_file);

			out("\n");

			for (const auto& hook : hooks)
				hook([&](const std::string_view str) {
					out("{}", str);
				});

			result.emplace(filepath, std::move(os).str());
		}

		return result;
	}
};

struct SourceGeneratorLua {
	sol::state lua;
//	libv::lua::State lua;
	SourceGenerator_v1 gen;

	SourceGeneratorLua(std::string filepath_in, std::string filepath_hpp, std::string filepath_cpp) {
		lua.open_libraries(sol::lib::base);
		lua.open_libraries(sol::lib::table);
		lua.open_libraries(sol::lib::string);

		{
			auto type = lua.new_usertype<SourceGenerator_v1::MemberEntry>("MemberEntry");
			type.set("identifier", &SourceGenerator_v1::MemberEntry::identifier);
			type.set("type", &SourceGenerator_v1::MemberEntry::type);
			type.set("init_value", &SourceGenerator_v1::MemberEntry::init_value);
			type.set("use_move", &SourceGenerator_v1::MemberEntry::use_move);
		}
		{
			auto type = lua.new_usertype<SourceGenerator_v1::ClazzEntry>("ClazzEntry");
			type.set("identifier", &SourceGenerator_v1::ClazzEntry::identifier);
			type.set("struct", &SourceGenerator_v1::ClazzEntry::struct_);
			type.set("scope_class", &SourceGenerator_v1::ClazzEntry::scope_class);
			type.set("members", &SourceGenerator_v1::ClazzEntry::members);
		}

		lua["__codegen_single_line"] = "// -------------------------------------------------------------------------------------------------\n";
		lua["__codegen_double_line"] = "// =================================================================================================\n";
		lua["__codegen_filepath_in"] = libv::slice_prefix_view(filepath_in, "src/");
		lua["__codegen_filepath_hpp"] = libv::slice_prefix_view(filepath_hpp, "src/");
		if (!filepath_cpp.empty())
			lua["__codegen_filepath_cpp"] = libv::slice_prefix_view(filepath_cpp, "src/");

		lua.set_function("plugin", [this](std::string name) {
			auto plugin_env = sol::environment(lua, sol::create, lua.globals());
			return lua.safe_script_file("app/codegen/plugin/" + name + ".lua", plugin_env);
		});

		lua.set_function("namespace", [this](std::string name) {
			gen.add_namespace(std::move(name));
		});

		lua.set_function("include", [this](std::string name) {
			gen.include(std::move(name));
		});

		lua.set_function("include_hpp", [this](std::string name) {
			gen.include_hpp(std::move(name));
		});

		lua.set_function("include_cpp", [this](std::string name) {
			gen.include_cpp(std::move(name));
		});

		lua.set_function("member_sc", [this](std::string identifier, std::string type, std::string init_value) {
			auto result = lua.create_table();
			result["entity_type"] = "static_variable";
			result["identifier"] = std::move(identifier);
			result["type"] = std::move(type);
			result["init_value"] = std::move(init_value);
			return result;
		});

		lua.set_function("member_v", [this](std::string identifier, std::string type, std::string init_value) {
			auto result = lua.create_table();
			result["entity_type"] = "variable";
			result["identifier"] = std::move(identifier);
			result["type"] = std::move(type);
			result["init_value"] = std::move(init_value);
			result["use_move"] = false;
			return result;
		});

		lua.set_function("member_mv", [this](std::string identifier, std::string type, std::string init_value) {
			auto result = lua.create_table();
			result["entity_type"] = "variable";
			result["identifier"] = std::move(identifier);
			result["type"] = std::move(type);
			result["init_value"] = std::move(init_value);
			result["use_move"] = true;
			return result;
		});

		lua.set_function("enable_hpp", [this](SourceGenerator_v1::clazz_hook_type hook) {
			gen.add_class_hook_hpp(std::move(hook));
		});

		lua.set_function("enable_cpp", [this](SourceGenerator_v1::clazz_hook_type hook) {
			gen.add_class_hook_cpp(std::move(hook));
		});

		lua.set_function("generate_hpp", [this](SourceGenerator_v1::file_hook_type hook) {
			gen.add_file_hook_hpp(std::move(hook));
		});

		lua.set_function("generate_cpp", [this](SourceGenerator_v1::file_hook_type hook) {
			gen.add_file_hook_cpp(std::move(hook));
		});

		lua.set_function("generate_external_file", [this](std::string_view filepath, SourceGenerator_v1::file_hook_type hook) {
			gen.add_file_hook_external(filepath, std::move(hook));
		});

		lua.set_function("struct", [this](std::string identifier, sol::table members) {
			auto clazz = gen.add_class(identifier, true);

			for (const auto& [_, member] : members) {
				if (member.get_type() != sol::type::table)
					throw std::runtime_error("Class member has to be a table");

				auto member_table = sol::table(member);
				auto member_type = member_table["entity_type"].get<std::string>();

				if (member_type == "variable") {
					auto member_name = member_table["identifier"].get<std::string>();
					auto member_type = member_table["type"].get<std::string>();
					auto member_init_value = member_table["init_value"].get<std::string>();
					auto member_use_move = member_table["use_move"].get<bool>();
					clazz->members.emplace_back(
							member_name,
							member_type,
							member_init_value,
							member_use_move
					);

				} else if (member_type == "static_variable") {
					auto member_name = member_table["identifier"].get<std::string>();
					auto member_type = member_table["type"].get<std::string>();
					auto member_init_value = member_table["init_value"].get<std::string>();
					clazz->static_members.emplace_back(
							member_name,
							member_type,
							member_init_value
					);

				} else if (member_type == "class") {
					auto class_name = member_table["identifier"].get<std::string>();

					auto member_class = gen.grab_class_to_member(class_name);
					clazz->member_classes.emplace_back(std::move(member_class));

				} // else
//					error();

//				std::cout << "member_type: " << member_type << std::endl;
			}

			auto result = lua.create_table();
			result["entity_type"] = "class";
			result["identifier"] = identifier;
			return result;
		});
	}

	[[nodiscard]] auto generate(std::string_view input_file, std::string_view source_script) {
		auto env = sol::environment(lua, sol::create, lua.globals());

		lua.script(source_script, env);

		auto result_hpp = gen.generate_hpp(input_file);
		auto result_cpp = gen.generate_cpp(input_file);
		auto result_externals = gen.generate_external(input_file);

		return std::make_tuple(result_hpp, result_cpp, result_externals);
	}
};

// TODO P2: Codegen hooks should be more free formed, more generalized:
//  			enable function should accept a table where multiple hooks
//				could be defined like: cpp/hpp - pre/post - class/members

void save(const std::string& filepath, const std::string& content, bool external) {
	// Try to read back the current content of the output file, to check if its changed
	std::cout << (external ? "output_file (external): " : "output_file: ") << filepath << std::flush;
	const auto current_source_hpp = libv::read_file_ec(filepath);

	if (!current_source_hpp.ec && current_source_hpp.data == content)
		// Output file is already up-to-date, skip write to not invalidate timestamps on files
		std::cout << " (up-to-date)";
	else
		// The output file is out-of-date, write out the fresh source
		libv::write_file_or_throw(filepath, content);

	std::cout << std::endl;
}

int main(int argc, const char** argv) {
	if (argc < 3) {
		std::cerr << "Usage: codegen <input_file> <output_file_hpp> [<output_file_cpp>]" << std::endl;
		return EXIT_FAILURE;
	}

	const auto header_only_mode = argc < 4;

	const auto input_file = argv[1];
//	const std::string output_file = argv[2];

//	const std::string input_file = "app/codegen/example/cto.lua";
//	const std::string output_file = "app/codegen/example/cto.out";
//	const std::string output_file_hpp = output_file + ".hpp";
//	const std::string output_file_cpp = output_file + ".cpp";

	const std::string output_file_hpp = argv[2];
	const std::string output_file_cpp = header_only_mode ? "" : argv[3];

	SourceGeneratorLua eb(input_file, output_file_hpp, output_file_cpp);

	std::cout << " input_file: " << input_file << "\n";

	const auto input_source = libv::read_file_ec(input_file);
	if (input_source.ec) {
		std::cerr << "Failed to read file: " << input_file << " - " << input_source.ec << ": " << input_source.ec.message();
		return EXIT_FAILURE;
	}

	const auto generated_sources = eb.generate(input_file, input_source.data);

	save(output_file_hpp, std::get<0>(generated_sources), false);
	if (!header_only_mode)
		save(output_file_cpp, std::get<1>(generated_sources), false);
	for (const auto& [filepath, source_code] : std::get<2>(generated_sources))
		save(filepath, source_code, true);

	return EXIT_SUCCESS;
}

// -- fix file generated source stamp
// -- [hpp|cpp] head before
// -- [hpp|cpp] head after
// -- [hpp|cpp] include before
// -- [hpp|cpp] include after
// -- [hpp|cpp] namespace begin before
// -- [hpp|cpp] namespace begin after
// -- [hpp|cpp] namespace end before
// -- [hpp|cpp] namespace end after
// -- [hpp|cpp] tail before
// -- [hpp|cpp] tail after
