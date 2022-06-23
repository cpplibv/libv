// Project: libv.serial, File: src/libv/serial/archive/json.hpp

#pragma once

// ext
#include <vide/archives/json.hpp>
#include <vide/vide.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using JSONInput = vide::JSONInputArchive;
using JSONOutput = vide::JSONOutputArchive;

struct JSON {
	using input = JSONInput;
	using output = JSONOutput;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
