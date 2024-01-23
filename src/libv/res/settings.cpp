// Project: libv.res, File: src/libv/res/settings.cpp

#include <libv/res/settings.hpp>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

ResourceLoaderSettings::ResourceLoaderSettings(std::string basePath) {
	resourceMappings.emplace_back("", std::move(basePath));
}

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
