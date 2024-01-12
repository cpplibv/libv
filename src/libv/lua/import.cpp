// Project: libv.lua, File: src/libv/lua/import.cpp

#include <libv/lua/import.hpp>

#include <fmt/format.h>
#include <sol/state.hpp>
#include <sol/state_view.hpp>

#include <libv/algo/linear_contains.hpp>
#include <libv/utility/guard.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

void Importer::bind(sol::state& lua) {
	lua.set_function("import", sol::protect([this](sol::this_state s, const std::string_view importName) {
		auto lua = sol::state_view(s);
		return this->auxImport(lua, importName);
	}));
}

void Importer::invalidateCache() {
	loadedModules.clear();
}

sol::object Importer::auxImport(sol::state_view& lua, std::string_view importName) {
	// Check import cycle
	if (libv::linear_contains(importStack, importName)) {
		failureInFlight = true;
		failureStack = importStack;
		throw sol::error("Include cycle detected");
	}

	// Check cache
	const auto it = loadedModules.find(importName);
	if (it != loadedModules.end())
		return it->second;

	// Load file
	const auto fileIdentifier = fmt::format("{}{}", importName, importName.ends_with(".lua") ? "" : ".lua");
	std::string scriptSourceCode;
	try {
		scriptSourceCode = fileLoader(fileIdentifier);
	} catch (const std::exception& e) {
		if (!std::exchange(failureInFlight, true)) {
			failure = std::current_exception();
			failureStack = importStack;
		}
		throw sol::error("Load failure");
	}

	// Add to import stack
	importStack.emplace_back(importName);
	auto importStackGuard = libv::guard([&] { importStack.pop_back(); });

	// Execute
	sol::object result;
	auto env = sol::environment(lua, sol::create, lua.globals());
	try {
		result = lua.safe_script(scriptSourceCode, env, sol::script_throw_on_error, fileIdentifier);
	} catch (const std::exception& e) {
		if (!std::exchange(failureInFlight, true)) {
			failure = std::current_exception();
			failureStack = importStack;
			failureStack.pop_back();
		}
		throw sol::error("Execution failure");
	}

	// Add to cache and return
	loadedModules.emplace(importName, result);
	return result;
}

sol::object Importer::import(sol::state_view& lua, std::string_view importName) {
	failureInFlight = false;

	sol::object result;
	try {
		result = auxImport(lua, importName);
	} catch (const std::exception& e) {
		(void) e;
	}

	if (failureInFlight) {
		failureInFlight = false;
		std::rethrow_exception(failure);
	}
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
