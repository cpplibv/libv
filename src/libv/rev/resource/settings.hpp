// Project: libv.rev, File: src/libv/rev/resource/settings.hpp

#pragma once

// std
#include <string>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct ResourceManagerSettings {
	std::string base_path; /// Relative resource path base

	// bool track_files = true; /// Track every resource and reload resource upon file change
	// bool relative_path_only = true; /// Forbid requests with absolute path
	// bool restict_under_base = true; /// Forbid requests that would leave the base path
	// bool cache_fallback = true; /// Insert failed resource lookups into cache as the fallback value
	// std::unordered_set<std::filesystem::path> track; /// Track specific resource and reload resource upon file change
};

struct TextureLoaderSettings : ResourceManagerSettings {
	//	bool builtin_colors_enabled = true;
	//	std::string builtin_colors_prefix = "builtin:";
};

struct Settings {
	TextureLoaderSettings texture = {"res/texture/"};
	ResourceManagerSettings shader = {"res/shader/"};
	ResourceManagerSettings model = {"res/model/"};
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
