// Project: libv.gl, File: src/libv/rev/gl/swizzle.cpp

// hpp
#include <libv/rev/gl/swizzle.hpp>
// libv
#include <libv/gl/enum.hpp>
#include <libv/utility/concat.hpp>
// std
#include <cassert>
#include <stdexcept>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Swizzle parse_swizzle_or_throw(std::string_view string) {
	if (string.size() != 4)
		throw std::runtime_error(libv::concat("Invalid swizzle string. Length must be 4 but it is ", string.size(), " for : \"", string, "\"."));

	auto process = [](int index, char c) {
		switch(c) {
		case 'r': return Swizzle::Value::red;
		case 'g': return Swizzle::Value::green;
		case 'b': return Swizzle::Value::blue;
		case 'a': return Swizzle::Value::alpha;
		case '0': return Swizzle::Value::zero;
		case '1': return Swizzle::Value::one;
		default:
			throw std::runtime_error(libv::concat("Invalid swizzle string. Character at index ", index, " is \"", c, "\" but it must be one of \"rgba01\"."));
		}
	};

	return Swizzle{
		process(0, string[0]),
		process(1, string[1]),
		process(2, string[2]),
		process(3, string[3])
	};
}

libv::gl::Swizzle to_libv_gl_swizzle(Swizzle::Value value) {
	switch(value) {
	case Swizzle::Value::red: return libv::gl::Swizzle::Red;
	case Swizzle::Value::green: return libv::gl::Swizzle::Green;
	case Swizzle::Value::blue: return libv::gl::Swizzle::Blue;
	case Swizzle::Value::alpha: return libv::gl::Swizzle::Alpha;
	case Swizzle::Value::zero: return libv::gl::Swizzle::Zero;
	case Swizzle::Value::one: return libv::gl::Swizzle::One;
	}

	assert(false && "Invalid Swizzle::Value enum value");
	return libv::gl::Swizzle::One; // [[unreachable]]
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
