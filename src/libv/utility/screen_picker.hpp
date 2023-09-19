// Project: libv.utility, File: src/libv/utility/screen_picker.hpp

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class screen_picker {
	using F = T;
	using mat4 = libv::mat4_t<F>;
	using vec2 = libv::vec2_t<F>;
	using vec3 = libv::vec3_t<F>;
	using vec4 = libv::vec4_t<F>;

private:
	vec2 canvas_size_;
	mat4 matVP;
	mat4 matVPinv;

public:
	[[nodiscard]] constexpr screen_picker() noexcept :
		canvas_size_(1, 1),
		matVP(1),
		matVPinv(1) { }

	[[nodiscard]] constexpr screen_picker(const mat4& matVP, vec2 canvas_size_) noexcept :
		canvas_size_(canvas_size_),
		matVP(matVP),
		matVPinv(matVP.inverse_copy()) { }

	[[nodiscard]] constexpr screen_picker(const mat4& matVP, const mat4& matVPinv, vec2 canvas_size_) noexcept :
		canvas_size_(canvas_size_),
		matVP(matVP),
		matVPinv(matVPinv) { }

	[[nodiscard]] constexpr vec3 to_world(const vec2 canvas_coord) const noexcept {
		auto coord2Scaled = canvas_coord / canvas_size_ * F{2} - F{1};
		auto coord4ScaledStart = vec4(coord2Scaled, F{-1}, F{1});
		auto coord4ScaledEnd = vec4(coord2Scaled, F{0}, F{1});

		auto coordLineWStart = matVPinv * coord4ScaledStart;
		coordLineWStart /= coordLineWStart.w;

		auto coordLineWEnd = matVPinv * coord4ScaledEnd;
		coordLineWEnd /= coordLineWEnd.w;

		return libv::vec::xyz((coordLineWEnd - coordLineWStart).normalize());
	}

	[[nodiscard]] constexpr libv::vec3f world_from_uvd(const vec2 uv, const float depth) const noexcept {
		const auto ndc = libv::vec4f{uv * F{2} - F{1}, depth, 1};
		vec4 world = matVPinv * ndc;
		world /= world.w;
		return xyz(world);
	}

	[[nodiscard]] constexpr vec2 to_uv_corner(const vec3 world_coord) const noexcept {
		auto objectSPosition = matVP * vec4(world_coord, F{1});
		// if objectSPosition.w <= 0 then the point not on screen (it's behind the camera)
		objectSPosition /= objectSPosition.w;
		return xy(objectSPosition) * F{0.5} + F{0.5};
	}

	[[nodiscard]] constexpr vec2 to_uv(const vec3 world_coord) const noexcept {
		return to_uv_corner(world_coord) + F{0.5} / canvas_size_;
	}

	[[nodiscard]] constexpr vec2 to_screen(const vec3 world_coord) const noexcept {
		return to_uv_corner(world_coord) * canvas_size_;
	}

	struct AheadResult {
		vec2 position;
		bool ahead;
	};

	[[nodiscard]] constexpr AheadResult to_uv_corner_ahead(const vec4 world_coord) const noexcept {
		auto objectSPosition = matVP * world_coord;
		// if objectSPosition.w <= 0 then the point not on screen (it's behind the camera)
		const auto ahead = objectSPosition.w > 0;
		objectSPosition /= objectSPosition.w;

		return {xy(objectSPosition) * F{0.5} + F{0.5}, ahead};
	}

	[[nodiscard]] constexpr AheadResult to_uv_corner_ahead(const vec3 world_coord) const noexcept {
		return to_uv_corner_ahead(vec4(world_coord, 1));
	}

	[[nodiscard]] constexpr AheadResult to_uv_ahead(const vec3 world_coord) const noexcept {
		auto result = to_uv_corner_ahead(world_coord);
		result.position += F{0.5} / canvas_size_;
		return result;
	}

	[[nodiscard]] constexpr AheadResult to_pixel_uv_ahead(const vec4 world_coord) const noexcept {
		auto result = to_uv_corner_ahead(world_coord);
		result.position = libv::vec::round(result.position * canvas_size_) / canvas_size_;
		result.position += F{0.5} / canvas_size_;
		return result;
	}

	[[nodiscard]] constexpr AheadResult to_pixel_uv_ahead(const vec3 world_coord) const noexcept {
		return to_pixel_uv_ahead(vec4(world_coord, 1));
	}

	[[nodiscard]] constexpr AheadResult to_screen_ahead(const vec3 world_coord) const noexcept {
		auto result = to_uv_corner_ahead(world_coord);
		result.position *= canvas_size_;
		return result;
	}

	[[nodiscard]] constexpr inline vec2 canvas_size() const noexcept {
		return canvas_size_;
	}

	[[nodiscard]] constexpr inline vec2 pixel_uv_size() const noexcept {
		return 1.f / canvas_size_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
