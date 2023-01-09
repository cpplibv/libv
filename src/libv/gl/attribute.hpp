// Project: libv.gl, File: src/libv/gl/attribute.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/gl/enum.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

template <int32_t Channel, typename T>
struct Attribute;

// -------------------------------------------------------------------------------------------------

struct BaseAttribute {
	int32_t channel;
	AttributeType type;
	int32_t dimension;

	constexpr inline BaseAttribute(int32_t channel, AttributeType type, int32_t dimension) noexcept :
			channel(channel),
			type(type),
			dimension(dimension) {}
};

// -------------------------------------------------------------------------------------------------

template <int32_t Channel>
struct Attribute<Channel, bool> : BaseAttribute {
	using value_type = bool; // The C++ type
	using attribute_type = value_type;
	using underlying_type = int32_t; // The OpenGL type
	static constexpr int32_t channel = Channel;
	static constexpr AttributeType type = AttributeType::INT;
	static constexpr int32_t dimension = 1;

	constexpr inline Attribute() noexcept : BaseAttribute(channel, type, dimension) {}
};

template <int32_t Channel>
struct Attribute<Channel, int32_t> : BaseAttribute {
	using value_type = int32_t; // The C++ type
	using attribute_type = value_type;
	using underlying_type = int32_t; // The OpenGL type
	static constexpr int32_t channel = Channel;
	static constexpr AttributeType type = AttributeType::INT;
	static constexpr int32_t dimension = 1;

	constexpr inline Attribute() noexcept : BaseAttribute(channel, type, dimension) {}
};

template <int32_t Channel>
struct Attribute<Channel, float> : BaseAttribute {
	using value_type = float; // The C++ type
	using attribute_type = value_type;
	using underlying_type = float; // The OpenGL type
	static constexpr int32_t channel = Channel;
	static constexpr AttributeType type = AttributeType::FLOAT;
	static constexpr int32_t dimension = 1;

	constexpr inline Attribute() noexcept : BaseAttribute(channel, type, dimension) {}
};

template <int32_t Channel>
struct Attribute<Channel, double> : BaseAttribute {
	using value_type = double; // The C++ type
	using attribute_type = value_type;
	using underlying_type = double; // The OpenGL type
	static constexpr int32_t channel = Channel;
	static constexpr AttributeType type = AttributeType::DOUBLE;
	static constexpr int32_t dimension = 1;

	constexpr inline Attribute() noexcept : BaseAttribute(channel, type, dimension) {}
};

template <int32_t Channel, std::size_t N, typename T>
struct Attribute<Channel, libv::vec_t<N, T>> : BaseAttribute {
	using value_type = libv::vec_t<N, T>; // The C++ type
	using attribute_type = value_type;
	using underlying_type = T; // The OpenGL type
	static constexpr int32_t channel = Channel;
	static constexpr AttributeType type = Attribute<Channel, T>::type;
	static constexpr int32_t dimension = N;

	constexpr inline Attribute() noexcept : BaseAttribute(channel, type, dimension) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
