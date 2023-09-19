// Project: libv.sun, File: src/libv/sun/ui/overlay_shader_error.hpp

// libv
#include <libv/ui/component/component.hpp>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

/// @nexus Handles ui-wise rev shader load events and displays them:
/// 	@handle libv::rev::ShaderLoadSuccess
/// 	@handle libv::rev::ShaderLoadFailure
/// 	@handle libv::rev::ShaderUnload
///
/// @param useStyles marks if the overlay's components should rely on the styles or to use manual overrides
/// @return the shader error overlay
libv::ui::Component overlay_shader_error(bool useStyles = false);

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace libv
