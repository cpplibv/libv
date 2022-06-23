// Project: libv.serial, File: src/libv/serial/archive/fwd.hpp

#pragma once


// -------------------------------------------------------------------------------------------------

namespace vide {

class JSONInputArchive;
class JSONOutputArchive;

class XMLInputArchive;
class XMLOutputArchive;

} // namespace vide

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace archive {

class Binary;
class BinaryOutput;
class BinaryInput;

class JSON;
using JSONInput = vide::JSONInputArchive;
using JSONOutput = vide::JSONOutputArchive;

class JSONAny;
class JSONAnyInput;
class JSONAnyOutput;

class XML;
using XMLInput = vide::XMLInputArchive;
using XMLOutput = vide::XMLOutputArchive;

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
