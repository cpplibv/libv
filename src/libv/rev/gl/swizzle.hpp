// Project: libv.rev, File: src/libv/rev/resource/texture.hpp

#pragma once

// libv
#include <libv/gl/enum_fwd.hpp>
// std
#include <cstdint>
#include <string_view>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------
// Note: I am not sure on this Swizzle replication / placement, main reason is only memory: 16byte -> 4byte storage

struct Swizzle {
	enum class Value : uint8_t {
		red,
		green,
		blue,
		alpha,
		zero,
		one,
	};

	Value r = Value::red;
	Value g = Value::green;
	Value b = Value::blue;
	Value a = Value::alpha;

	[[nodiscard]] constexpr inline auto operator<=>(const Swizzle&) const = default;
};

static constexpr Swizzle defaultSwizzle{};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] Swizzle parse_swizzle_or_throw(std::string_view string);

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::gl::Swizzle to_libv_gl_swizzle(Swizzle::Value value);

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
