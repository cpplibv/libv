// Project: libv, File: app/enum/enum_main.cpp

// libv
#include <libv/algo/linear_find.hpp>
#include <libv/lua/lua.hpp>
#include <libv/utility/projection.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// ext
#include <fmt/format.h>
#include <range/v3/view/reverse.hpp>
// std
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>


// -------------------------------------------------------------------------------------------------

static constexpr std::string_view codegen_version = "v3.1.1";

// -------------------------------------------------------------------------------------------------

class SourceGenerator {
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

		std::vector<std::function<void(const ClazzEntry&)>> generators;

		ClazzEntry(std::string identifier, bool struct_) :
			identifier(std::move(identifier)),
			struct_(struct_) {}
	};

	using hook_type = std::function<void(std::function<void(std::string_view)>, const SourceGenerator::ClazzEntry&)>;

private:
	std::vector<std::string> namespaces;
	std::set<std::string> includes_hpp;
	std::set<std::string> includes_cpp;

	std::vector<std::shared_ptr<ClazzEntry>> global_classes;
	std::vector<hook_type> class_hooks_hpp;
	std::vector<hook_type> class_hooks_cpp;

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
	void add_class_hook_hpp(hook_type hook) {
		class_hooks_hpp.emplace_back(std::move(hook));
	}

	void add_class_hook_cpp(hook_type hook) {
		class_hooks_cpp.emplace_back(std::move(hook));
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
//		std::string ident(depth, '\t');

//		out("{}{} {} {{\n", ident, clazz.struct_ ? "struct" : "class", clazz.identifier);

		for (const auto& member_clazz : clazz.member_classes)
			generate_cpp_class(*member_clazz, out, depth + 1);
//		if (!clazz.member_classes.empty())
//			out("\n");

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");
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
		out("\n");

		if (!includes_hpp.empty())
			out("//\n");
		for (const auto& in : includes_hpp)
			out("#include <{}>\n", in);

		out("\n");
		out("\n");

		for (const auto& ns : namespaces)
			out("namespace {} {{\n", ns);
		if (!namespaces.empty())
			out("\n");

		out("// -------------------------------------------------------------------------------------------------\n");

		for (const auto& clazz : global_classes) {
			out("\n");
			generate_hpp_class(*clazz, out);
		}

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");

		if (!namespaces.empty())
			out("\n");
		for (const auto& ns : namespaces | ranges::view::reverse)
			out("}} // namespace {}\n", ns);

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
		out("\n");

		out("//\n");
		for (const auto& in : includes_cpp)
			out("#include <{}>\n", in);

		out("\n");
		out("\n");

		for (const auto& ns : namespaces)
			out("namespace {} {{\n", ns);

		for (const auto& clazz : global_classes) {
			generate_cpp_class(*clazz, out);
		}

		out("\n");
		out("// -------------------------------------------------------------------------------------------------\n");

		if (!namespaces.empty())
			out("\n");
		for (const auto& ns : namespaces | ranges::view::reverse)
			out("}} // namespace {}\n", ns);

		return std::move(os).str();
	}
};

struct SourceGeneratorLua {
	libv::lua::State lua;
	SourceGenerator gen;

	SourceGeneratorLua() {
		lua.open_libraries(sol::lib::base);
		lua.open_libraries(sol::lib::table);
		lua.open_libraries(sol::lib::string);

		{
			auto type = lua.new_usertype<SourceGenerator::MemberEntry>("MemberEntry");
			type.set("identifier", &SourceGenerator::MemberEntry::identifier);
			type.set("type", &SourceGenerator::MemberEntry::type);
			type.set("init_value", &SourceGenerator::MemberEntry::init_value);
			type.set("use_move", &SourceGenerator::MemberEntry::use_move);
		}
		{
			auto type = lua.new_usertype<SourceGenerator::ClazzEntry>("ClazzEntry");
			type.set("identifier", &SourceGenerator::ClazzEntry::identifier);
			type.set("struct", &SourceGenerator::ClazzEntry::struct_);
			type.set("scope_class", &SourceGenerator::ClazzEntry::scope_class);
			type.set("members", &SourceGenerator::ClazzEntry::members);
		}

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

		lua.set_function("enable_hpp", [this](SourceGenerator::hook_type hook) {
			gen.add_class_hook_hpp(std::move(hook));
		});

		lua.set_function("enable_cpp", [this](SourceGenerator::hook_type hook) {
			gen.add_class_hook_cpp(std::move(hook));
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

	[[nodiscard]] std::pair<std::string, std::string> generate(std::string_view input_file, std::string_view source_script) {
		auto env = sol::environment(lua, sol::create, lua.globals());

		lua.script(source_script, env);

		auto result_hpp = gen.generate_hpp(input_file);
		auto result_cpp = gen.generate_cpp(input_file);

		return std::make_pair(result_hpp, result_cpp);
	}
};

// TODO P2: Codegen hooks should be more free formed, more generalized:
//  			enable function should accept a table where multiple hooks
//				could be defined like: cpp/hpp - pre/post - class/members

int main(int argc, const char** argv) {
	SourceGeneratorLua eb;

	if (argc < 3) {
		std::cerr << "Usage: enum <input_file> <output_file_hpp> <output_file_cpp>" << std::endl;
		return EXIT_FAILURE;
	}

	const auto input_file = argv[1];
//	const std::string output_file = argv[2];

//	const std::string input_file = "app/codegen/example/cto.lua";
//	const std::string output_file = "app/codegen/example/cto.out";
//	const std::string output_file_hpp = output_file + ".hpp";
//	const std::string output_file_cpp = output_file + ".cpp";

	const std::string output_file_hpp = argv[2];
	const std::string output_file_cpp = argv[3];

	std::cout << " input_file: " << input_file << "\n";

	const auto generated_source = eb.generate(input_file, libv::read_file_or_throw(input_file));

	// Try to read back the current content of the output file, to check if its changed
	std::cout << "output_file: " << output_file_hpp << std::flush;
	const auto current_source_hpp = libv::read_file_ec(output_file_hpp);

	if (!current_source_hpp.ec && current_source_hpp.data == generated_source.first)
		// Output file is already up-to-date, skip write to not invalidate timestamps on files
		std::cout << " (up-to-date)";
	else
		// The output file is out-of-date, write out the fresh source
		libv::write_file_or_throw(output_file_hpp, generated_source.first);

	std::cout << std::endl;

	std::cout << "output_file: " << output_file_cpp << std::flush;
	const auto current_source_cpp = libv::read_file_ec(output_file_cpp);

	if (!current_source_cpp.ec && current_source_cpp.data == generated_source.second)
		// Output file is already up-to-date, skip write to not invalidate timestamps on files
		std::cout << " (up-to-date)";
	else
		// The output file is out-of-date, write out the fresh source
		libv::write_file_or_throw(output_file_cpp, generated_source.second);

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
