// Project: libv.serial, File: src/libv/serial/archive/json_any_fwd.hpp

#pragma once


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

template <typename CRTP> class BasicJSONAnyInput;
template <typename CRTP> class BasicJSONAnyOutput;

template <typename CRTP = void>
struct BasicJSONAny {
	using input = BasicJSONAnyInput<CRTP>;
	using output = BasicJSONAnyOutput<CRTP>;
};

// -------------------------------------------------------------------------------------------------

using JSONAnyInput = BasicJSONAnyInput<void>;
using JSONAnyOutput = BasicJSONAnyOutput<void>;

using JSONAny = BasicJSONAny<void>;

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv
