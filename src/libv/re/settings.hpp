// Project: libv.re, File: src/libv/re/settings.hpp

#pragma once

#include <libv/res/settings.hpp>

#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ResourceLoaderSettingsShader : libv::res::ResourceLoaderSettings {
	std::vector<std::string> globalDefines;

	explicit ResourceLoaderSettingsShader(std::string base_path);
};

// -------------------------------------------------------------------------------------------------

struct Settings {
	ResourceLoaderSettingsShader shader{"res/"};
	libv::res::ResourceLoaderSettings texture{"res/"};
	libv::res::ResourceLoaderSettings model{"res/"};
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
