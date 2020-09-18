// Project: libv.ui, File: app/vm4_viewer/ui/overlay_shader_inspector.hpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/fwd.hpp>
// std
#include <string>
#include <string_view>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

class OverlayShaderInspector : public libv::ui::PanelLine {
	// List of shader uniforms
	// Interactive options to change uniforms
	// Other information about a shader
	// Shader selector
	// https://www.khronos.org/opengl/wiki/Program_Introspection

public:
	explicit OverlayShaderInspector(libv::ui::CoreComponent& parent);
	OverlayShaderInspector(libv::ui::CoreComponent& parent, std::string name);
	OverlayShaderInspector(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
	~OverlayShaderInspector();

public:
	// ...
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
