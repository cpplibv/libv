// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

class JSONInputAny;
class JSONOutputAny;

struct JSONAny {
	using input = JSONInputAny;
	using output = JSONOutputAny;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
