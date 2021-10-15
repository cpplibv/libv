// Project: libv, File: app/space/view/make_shader_error_overlay.hpp

// libv
#include <libv/ui/component/component.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

/// Handles ui-wise rev shader load events and displays them:
/// 	libv::rev::ShaderLoadSuccess
/// 	libv::rev::ShaderLoadFailure
/// 	libv::rev::ShaderUnload
libv::ui::Component make_shader_error_overlay();

// -------------------------------------------------------------------------------------------------

} // namespace app
