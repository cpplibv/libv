// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/ui/resource/texture.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

ResourceStepResult UITexture2D::load() {
	return ResourceStepResult::OK;
}

ResourceStepResult UITexture2D::unload() {
	return ResourceStepResult::OK;
}

constexpr const char UITexture2D::ResourceLabel[];
constexpr UITexture2D::Step UITexture2D::ResourceLoadSequence[];
constexpr UITexture2D::Step UITexture2D::ResourceUnloadSequence[];

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
