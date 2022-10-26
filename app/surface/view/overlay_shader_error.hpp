// Project: libv, File: app/surface/view/overlay_shader_error.hpp

// libv
#include <libv/ui/component/component.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

/// Handles ui-wise rev shader load events and displays them:
/// 	libv::rev::ShaderLoadSuccess
/// 	libv::rev::ShaderLoadFailure
/// 	libv::rev::ShaderUnload
libv::ui::Component overlay_shader_error();

// -------------------------------------------------------------------------------------------------

} // namespace surface
