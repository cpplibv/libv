// Project: libv.serial, File: src/libv/serial/archive/xml.hpp

#pragma once

// ext
#include <vide/archives/xml.hpp>
#include <vide/vide.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

using XMLInput = vide::XMLInputArchive;
using XMLOutput = vide::XMLOutputArchive;

struct XML {
	using input = XMLInput;
	using output = XMLOutput;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
