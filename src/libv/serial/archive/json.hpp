// Project: libv.serial, File: src/libv/serial/archive/json.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using JSONInput = cereal::JSONInputArchive;
using JSONOutput = cereal::JSONOutputArchive;

struct JSON {
	using input = JSONInput;
	using output = JSONOutput;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
