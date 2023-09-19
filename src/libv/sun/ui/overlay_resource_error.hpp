// Project: libv.sun, File: src/libv/sun/ui/overlay_resource_error.hpp

// libv
#include <libv/ui/component/component.hpp>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

/// @nexus Handles ui-wise rev shader load events and displays them:
/// 	@handle libv::re::ShaderLoadSuccess
/// 	@handle libv::re::ShaderLoadFailure
/// 	@handle libv::re::ShaderUnload
/// 	@handle libv::re::TextureLoadSuccess
/// 	@handle libv::re::TextureLoadFailure
/// 	@handle libv::re::TextureUnload
///
/// @param useStyles marks if the overlay's components should rely on the styles or to use manual overrides
/// @return the shader error overlay
libv::ui::Component overlay_resource_error(bool useStyles = false);

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace libv
