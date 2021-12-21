// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

template <typename CRTP> class BasicBinaryInput;
template <typename CRTP> class BasicBinaryOutput;

template <typename CRTP = void>
struct BasicBinary {
	using input = BasicBinaryInput<CRTP>;
	using output = BasicBinaryOutput<CRTP>;
};

// -------------------------------------------------------------------------------------------------

using BinaryInput = BasicBinaryInput<void>;
using BinaryOutput = BasicBinaryOutput<void>;

using Binary = BasicBinary<void>;

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
