// Project: libv.re, File: src/libv/re/uniform/builtin.cpp

#include <libv/re/uniform/std140_verify.hpp>
#include <libv/re/uniform/builtin.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

static_assert(libv::re::is_std140_packed<BlockModelMatrices>);
static_assert(libv::re::is_std140_packed<BlockRenderPass>);
static_assert(libv::re::is_std140_packed<BlockLights>);

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
