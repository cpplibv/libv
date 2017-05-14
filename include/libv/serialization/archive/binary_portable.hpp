// File: portable_binary.hpp Author: Vader Created on 2017. május 13., 20:07

#pragma once

// pro
#include <libv/serialization/archive/binary_portable_in.hpp>
#include <libv/serialization/archive/binary_portable_out.hpp>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

struct BinaryPortable {
	using Input = BinaryPortableIn;
	using Output = BinaryPortableOut;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv