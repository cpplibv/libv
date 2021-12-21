// Project: libv.serial, File: src/libv/serial/archive/fwd.hpp

#pragma once


// -------------------------------------------------------------------------------------------------

namespace cereal {

class JSONInputArchive;
class JSONOutputArchive;

class XMLInputArchive;
class XMLOutputArchive;

} // namespace cereal

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace archive {

class Binary;
class BinaryOutput;
class BinaryInput;

class JSON;
using JSONInput = cereal::JSONInputArchive;
using JSONOutput = cereal::JSONOutputArchive;

class JSONAny;
class JSONAnyInput;
class JSONAnyOutput;

class XML;
using XMLInput = cereal::XMLInputArchive;
using XMLOutput = cereal::XMLOutputArchive;

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
