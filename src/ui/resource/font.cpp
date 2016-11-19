// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/ui/resource/font.hpp>
// pro
#include <libv/ui/resource/file.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

UIFont2D::UIFont2D(const File_view& file) :
	font_(),
	file(file) { }

ResourceStepResult UIFont2D::loadIN() {
	font_.loadFont(file->c_data(), file->size());
	file.reset();
	return ResourceStepResult::OK;
}

ResourceStepResult UIFont2D::loadGL() {
	font_.loadGL();
	return ResourceStepResult::OK;
}

ResourceStepResult UIFont2D::unloadIN() {
	font_.unloadFont();
	return ResourceStepResult::OK;
}

ResourceStepResult UIFont2D::unloadGL() {
	font_.unloadGL();
	return ResourceStepResult::OK;
}

constexpr const char UIFont2D::ResourceLabel[];
constexpr UIFont2D::Step UIFont2D::ResourceLoadSequence[];
constexpr UIFont2D::Step UIFont2D::ResourceUnloadSequence[];

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
