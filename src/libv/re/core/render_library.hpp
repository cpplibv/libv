// Project: libv.re, File: src/libv/re/core/render_library.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/re/settings.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct RenderLibrary {
	RenderLibrary(libv::re::Settings settings, libv::Nexus& nexus, libv::GL& gl);
	~RenderLibrary();
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
