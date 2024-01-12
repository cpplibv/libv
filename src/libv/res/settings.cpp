// Project: libv.ui, File: src/libv/ui/settings.hpp

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
