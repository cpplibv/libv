// Project: libv.ui, File: app/vm4_viewer/ui/overlay_shader_inspector.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <vm4_viewer/ui/overlay_shader_inspector.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

OverlayShaderInspector::OverlayShaderInspector(libv::ui::CoreComponent& parent) :
	PanelLine(parent, libv::ui::GenerateName, "OverlayShaderInspector") { }

OverlayShaderInspector::OverlayShaderInspector(libv::ui::CoreComponent& parent, std::string name) :
	PanelLine(parent, std::move(name)) { }

OverlayShaderInspector::OverlayShaderInspector(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type) :
	PanelLine(parent, libv::ui::GenerateName, type) { }

OverlayShaderInspector::~OverlayShaderInspector() { }

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
