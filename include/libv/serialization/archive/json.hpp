// File: json.hpp - Created on 2017.10.04. 04:41 - Author: Vader

#pragma once

// ext
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using JSONInput = cereal::JSONInputArchive;
using JSONOutput = cereal::JSONOutputArchive;

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
