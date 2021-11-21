// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

class BinaryInput;
class BinaryOutput;

struct Binary {
	using input = BinaryInput;
	using output = BinaryOutput;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
