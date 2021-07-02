// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/ui/component/component.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

/// Handles ui wise rev shader load events and displays them
/// libv::rev::ShaderLoadSuccess
/// libv::rev::ShaderLoadFailure
/// libv::rev::ShaderUnload
libv::ui::Component make_shader_error_overlay();

// -------------------------------------------------------------------------------------------------

} // namespace app
