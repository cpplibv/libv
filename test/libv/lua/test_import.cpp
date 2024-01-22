// Project: libv.lua, File: test/libv/lua/test_import.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <sol/state.hpp>
// libv
#include <libv/utility/concat.hpp>
//#include <libv/math/vec.hpp>
//#include <libv/parse/color.hpp>
// std
//#include <string_view>
#include <map>
// pro
#include <libv/lua/import.hpp>
#include <libv/lua/lua.hpp>


// -------------------------------------------------------------------------------------------------

// struct test_source_loader {
// 	std::map<std::string, std::string, std::less<>> sources;
//
// public:
// 	void add(std::string name, std::string source) {
// 		sources.emplace(std::move(name), std::move(source));
// 	}
//
// //	void import();
//
// 	auto make_loader_function() {
// 		return [this](const std::string_view path) {
// 			const auto it = sources.find(path);
// 			if (it == sources.end())
// 				return libv::lua::Importer::LoadResult{false, libv::concat("Requested source not found: \"", path, "\"")};
// 			else
// 				return libv::lua::Importer::LoadResult{true, it->second};
// 		};
// 	}
// };

// -------------------------------------------------------------------------------------------------

// TEST_CASE("Simple lua importer", "[libv.lua.import]") {
// 	auto lua = libv::lua::create_state(libv::lualib::basic);
// //	sol::state lua;
// //	lua.open_libraries(libv::lualib::base);
//
// 	libv::lua::Importer importer;
// 	importer.bind(lua);
//
// 	test_source_loader tsl0;
// 	test_source_loader tsl1;
//
// 	tsl0.add("source0.lua", R"(
// 		local m1s1 = import("module1.source0")
// 		print("module0 source0")
// 		return "module0 source0"
// 	)");
// 	tsl0.add("source1.lua", R"(
// 		print("module0 source1")
// 		return "module0 source1"
// 	)");
// 	tsl1.add("source0.lua", R"(
// 		print("module1 source0")
// 		return "module1 source0"
// 	)");
//
// //	importer.map("module0.", tsl0.make_loader_function(), importer2_context);
// 	importer.map("module0.", tsl0.make_loader_function());
// 	importer.map("module1.", tsl1.make_loader_function());
// //	importer.map("", current_mod_folder); // Add the special case for the current mod's folder
//
// 	const auto src_main = R"(
// 		local m0s0 = import("module0.source0")
// 		local m0s0 = import("module0.source0")
// 		local m1s0 = import("module1.source0")
//
// 		print("main")
//
// 		return true;
// 	)";
//
// 	bool success = lua.script(src_main);
// 	CHECK(success);
// }

// -------------------------------------------------------------------------------------------------

struct ImportTester {
	sol::state lua = libv::lua::create_state(libv::lualib::basic);

	libv::lua::Importer importer;

	std::map<std::string, std::string> testFiles;

	ImportTester() : importer([this](std::string_view fileIdentifier) {
		return testFiles.at(std::string(fileIdentifier));
	}) {
		importer.bind(lua);
	}

	void file(std::string&& identifier, std::string&& source) {
		testFiles.emplace(std::move(identifier), std::move(source));
	}

	bool run(std::string_view main) {
		auto result = lua.safe_script(main);
		return result.valid();
	}
};

TEST_CASE("Lua importer", "[libv.lua.import]") {
	ImportTester test;

	test.file("module0/source0.lua", R"(
		local m1s1 = import("module1/source0")
		print("module0 source0")
		return "module0 source0"
	)");
	test.file("module0/source1.lua", R"(
		print("module0 source1")
		return "module0 source1"
	)");
	test.file("module1/source0.lua", R"(
		print("module1 source0")
		return "module1 source0"
	)");

	CHECK(test.run(R"(
		local m0s0 = import("module0/source0")
		local m0s0 = import("module0/source1")
		local m1s0 = import("module1/source0")

		print("main")
	)"));
}
