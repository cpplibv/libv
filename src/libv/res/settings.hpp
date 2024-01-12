// Project: libv.ui, File: src/libv/ui/settings.hpp

#pragma once

#include <chrono>
#include <string>
#include <vector>


namespace libv {
namespace res {

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

public:
	explicit ResourceLoaderSettings(std::string basePath);
};

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
