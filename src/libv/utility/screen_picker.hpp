// Project: libv.utility, File: src/libv/utility/screen_picker.hpp

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class screen_picker {
	using float_type = T;
	using mat4 = libv::mat4_t<float_type>;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using vec4 = libv::vec4_t<float_type>;

private:
	vec2 canvas_size_;
	mat4 matrixPV;
	mat4 inversePV;

public:
	[[nodiscard]] constexpr screen_picker(const mat4& matrixPV, vec2 canvas_size_) noexcept :
		canvas_size_(canvas_size_),
		matrixPV(matrixPV),
		inversePV(matrixPV.inverse_copy()) { }

	[[nodiscard]] constexpr vec3 to_world(const vec2 canvas_coord) const noexcept {
		auto coord2Scaled = canvas_coord / canvas_size_ * float_type{2} - float_type{1};
		auto coord4ScaledStart = vec4(coord2Scaled, float_type{-1}, float_type{1});
		auto coord4ScaledEnd = vec4(coord2Scaled, float_type{0}, float_type{1});

		auto coordLineWStart = inversePV * coord4ScaledStart;
		coordLineWStart /= coordLineWStart.w;

		auto coordLineWEnd = inversePV * coord4ScaledEnd;
		coordLineWEnd /= coordLineWEnd.w;

		return libv::vec::xyz((coordLineWEnd - coordLineWStart).normalize());
	}

	[[nodiscard]] constexpr vec2 to_screen(const vec3 worldCoord) const noexcept {
		auto objectSPosition = matrixPV * vec4(worldCoord, float_type{1});

		// if objectSPosition.w <= 0 then the point not on screen (it's behind the camera)

		objectSPosition /= objectSPosition.w;

		return (libv::vec::xy(objectSPosition) + float_type{1}) * float_type{0.5} * canvas_size_;
	}

	[[nodiscard]] constexpr auto to_screen_ahead(const vec3 worldCoord) const noexcept {
		struct Result {
			vec2 position;
			bool on_screen;
		};

		auto objectSPosition = matrixPV * vec4(worldCoord, float_type{1});

		// if objectSPosition.w <= 0 then the point not on screen (it's behind the camera)
		const auto on_screen = objectSPosition.w > 0;

		objectSPosition /= objectSPosition.w;

		return Result{
			(libv::vec::xy(objectSPosition) + float_type{1}) * float_type{0.5} * canvas_size_,
			on_screen
		};
	}

	[[nodiscard]] constexpr inline vec2 canvas_size() const noexcept {
		return canvas_size_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
