// Project: libv.ui, File: src/libv/ui/style/style_loader.cpp

#include <libv/ui/style/style_loader.hpp>

#include <sol/state.hpp>

#include <libv/fsw/token.hpp>
#include <libv/fsw/watcher.hpp>
#include <libv/lua/import.hpp>
#include <libv/lua/lua.hpp>
#include <libv/res/load_file.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/style/style_file_load_event.hpp>
#include <libv/ui/style/style_lua_script_context.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/timer.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplStyleLoader {
public:
	using StyleFileID = std::size_t;

	UI& ui;
	Settings& settings;
	libv::fsw::Watcher& fsw;
	sol::state lua;
	libv::lua::Importer importer;

public:
	struct StyleFile {
		std::string fileIdentifier;
		std::vector<libv::fsw::SafeToken> fswTokens;
	};
	std::vector<StyleFile> styleFiles;

public:
	StyleFileID currentStyleFile = -1;
	std::ostringstream currentStyleErrors;

public:
	StyleLuaScriptContext styleLuaScriptContext;

public:
	ImplStyleLoader(UI& ui, Settings& settings, libv::fsw::Watcher& fsw);

	std::string auxLoadFile(StyleFileID id, std::string_view fileIdentifier);
	void auxLoadStyleFile(StyleFileID id, bool firstLoad);
};

// -------------------------------------------------------------------------------------------------

ImplStyleLoader::ImplStyleLoader(UI& ui, Settings& settings, libv::fsw::Watcher& fsw) :
	ui(ui),
	settings(settings),
	fsw(fsw),
	lua(libv::lua::create_state(libv::lualib::basic | libv::lualib::libv_vec)),
	importer([this](std::string_view importFileIdentifier) {
		return this->auxLoadFile(currentStyleFile, importFileIdentifier);
	}),
	styleLuaScriptContext(ui, [this](std::string_view message) {
		currentStyleErrors << message << '\n';
	}) {

	importer.bind(lua);
	styleLuaScriptContext.bind(lua);
}

std::string ImplStyleLoader::auxLoadFile(StyleFileID id, std::string_view fileIdentifier) {
	assert(id < styleFiles.size());
	const auto fileTracker = [this, id](std::string_view physicalFilepath) {
		styleFiles[id].fswTokens.emplace_back(fsw.subscribe_file_safe(physicalFilepath, [this, id](const libv::fsw::Event&) {
			auxLoadStyleFile(id, false);
		}));
	};

	log_ui.trace("Loading style file: '{}'", fileIdentifier);
	auto fileLoadOutcome = libv::res::loadFile(settings.resStyle, fileIdentifier, fileTracker);
	if (!fileLoadOutcome) {
		log_ui.error("Failed to load style file: '{}'\n{}", fileIdentifier, libv::res::toErrorReport(fileLoadOutcome.error()));
		throw StyleFileLoadException{
				id, styleFiles[id].fileIdentifier, importer.currentStack(),
				StyleFileLoadFailure::IncludeFailure{
					std::string(fileIdentifier),
					std::move(fileLoadOutcome.error().unmatchedMappings),
					std::move(fileLoadOutcome.error().mappingErrors),
					std::move(fileLoadOutcome.error().physicalErrors),
				},
				std::nullopt,
				std::nullopt};
	}

	return fileLoadOutcome.value();
}

void ImplStyleLoader::auxLoadStyleFile(StyleFileID id, bool firstLoad) {
	ui.execute_in_ui_loop([this, id, firstLoad] {
		libv::Timer timer;
		currentStyleFile = id;

		if (!firstLoad)
			importer.invalidateCache();

		const auto& fileIdentifier = styleFiles[id].fileIdentifier;
		std::vector<libv::fsw::SafeToken> oldTokens;
		auto& newTokens = styleFiles[id].fswTokens;
		std::swap(oldTokens, newTokens);

		try {
			// Execute style script by importing the main file
			(void) importer.import(lua, styleFiles[id].fileIdentifier);

			if (currentStyleErrors.tellp() == 0) {
				ui.nexus().broadcast_global(StyleFileLoadSuccess{firstLoad, id, fileIdentifier});
				log_ui.trace("Script loading {} successful in {:7.3f}ms", fileIdentifier, timer.timef_ms().count());
			} else {
				auto styleErrorMessages = std::move(currentStyleErrors).str();
				styleErrorMessages.pop_back(); // Remove trailing \n
				ui.nexus().broadcast_global(StyleFileLoadFailure{
						id, fileIdentifier, importer.lastFailureStack(),
						std::nullopt,
						std::nullopt,
						StyleFileLoadFailure::StyleError{std::move(styleErrorMessages)},
				});
				currentStyleErrors.str("");
				currentStyleErrors.clear();
			}
		} catch (const StyleFileLoadException& e) {
			ui.nexus().broadcast_global(e.failure);
		} catch (const std::exception& e) {
			log_ui.error("Failed to load style file: '{}'\n{}", fileIdentifier, e.what());
			ui.nexus().broadcast_global(StyleFileLoadFailure{
					id, fileIdentifier, importer.lastFailureStack(),
					std::nullopt,
					StyleFileLoadFailure::LuaError{std::string(e.what())},
					std::nullopt
			});
		}

		currentStyleFile = -1;
		lua.collect_garbage();
	});
}

// =================================================================================================

StyleLoader::StyleLoader(UI& ui, Settings& settings, libv::fsw::Watcher& fsw) :
	self(std::make_unique<ImplStyleLoader>(ui, settings, fsw)) {
}

StyleLoader::~StyleLoader() {
	// For the sake of forward declared types
}

void StyleLoader::load(std::string_view fileIdentifier) {
	const auto id = self->styleFiles.size();
	self->styleFiles.emplace_back(std::string(fileIdentifier));
	log_ui.trace("Loading blocking style: '{}'", fileIdentifier);
	self->auxLoadStyleFile(id, true);
}

void StyleLoader::terminate() {
	for (auto& styleFile : self->styleFiles)
		styleFile.fswTokens.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
