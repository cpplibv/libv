// File: xml.hpp - Created on 2017.10.04. 04:41 - Author: Vader

#pragma once

// ext
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using XMLInput = cereal::XMLInputArchive;
using XMLOutput = cereal::XMLOutputArchive;

struct XML {
	using input = XMLInput;
	using output = XMLOutput;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
