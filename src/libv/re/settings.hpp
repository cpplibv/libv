// Project: libv.re, File: src/libv/re/settings.hpp

#pragma once

#include <chrono>
#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ResourcePathMapping {
	std::string resourcePrefix; /// Resource identifier prefix, must contain the trailing slash (/) or be empty
	std::string virtualPrefix; /// Relative resource path base, must contain the trailing slash (/)
};

struct ResourceLoaderSettings {
	///// Forbid requests with absolute path
	// bool relativePathOnly = true;

	///
	std::vector<ResourcePathMapping> resourceMappings;

	/// Track every resource and reload resource upon file change
	bool trackFiles = true;

	/// Delay fsw reload. Useful to deduplicate hot-reloads when multiple dependency files edited at the same time
	std::chrono::steady_clock::duration fswReloadDelay = std::chrono::milliseconds(0);

	explicit ResourceLoaderSettings(std::string basePath);
};

struct ResourceLoaderSettingsShader : ResourceLoaderSettings {
	std::vector<std::string> globalDefines;

	explicit ResourceLoaderSettingsShader(std::string base_path);
};

// -------------------------------------------------------------------------------------------------

struct Settings {
	ResourceLoaderSettingsShader shader{"res/"};
	ResourceLoaderSettings texture{"res/"};
	ResourceLoaderSettings model{"res/"};
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
