// Project: libv.noise, File: src/libv/noise/noise.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
// pro
#include <libv/noise/seed.hpp>


namespace libv {
namespace noise {

// -------------------------------------------------------------------------------------------------

static constexpr float default_amplitude = 1.0f;
static constexpr float default_frequency = 1.0f;
static constexpr float default_lacunarity = 2.0f;
static constexpr float default_persistence = 0.5f;

// --- Value ---------------------------------------------------------------------------------------

constexpr inline struct ValueFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] static LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}

	[[nodiscard]] static LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} value;

// --- Perlin --------------------------------------------------------------------------------------

constexpr inline struct PerlinFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} perlin;

// --- Simplex -------------------------------------------------------------------------------------

constexpr inline struct SimplexFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} simplex;

// --- Cellular ------------------------------------------------------------------------------------

enum class CellularDistance {
	euclidean = 0,
	euclideansq = 1,
	manhattan = 2,
	hybrid = 3,
};

static constexpr CellularDistance default_cellular_distance = CellularDistance::euclidean;

enum class CellularReturn {
	cellvalue = 0,
	distance = 1,
	distance2 = 2,
	distance2add = 3,
	distance2sub = 4,
	distance2mul = 5,
	distance2div = 6,
};

static constexpr CellularReturn default_cellular_return = CellularReturn::cellvalue;

constexpr inline struct CellularFn {
private:
	CellularDistance distanceFn_ = default_cellular_distance;
	CellularReturn returnType_ = default_cellular_return;
	float jitter_ = 1.0f;

public:
	CellularFn() = default;
private:
	CellularFn(
			CellularDistance distanceFn,
			CellularReturn returnType,
			float jitter) :
			distanceFn_(distanceFn),
			returnType_(returnType),
			jitter_(jitter) {}

public:
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE CellularFn operator()(
			CellularDistance distanceFn = default_cellular_distance,
			CellularReturn returnType = default_cellular_return,
			float jitter = 1.0f) noexcept {
		return CellularFn{distanceFn, returnType, jitter};
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, float x, float y) const noexcept {
		return operator()(seed, x, y, distanceFn_, returnType_, jitter_);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, float x, float y, float z) const noexcept {
		return operator()(seed, x, y, z, distanceFn_, returnType_, jitter_);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord) const noexcept {
		return operator()(seed, coord.x, coord.y, distanceFn_, returnType_, jitter_);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord) const noexcept {
		return operator()(seed, coord.x, coord.y, coord.z, distanceFn_, returnType_, jitter_);
	}

	[[nodiscard]] static float operator()(Seed seed, float x, float y,
			CellularDistance distanceFn,
			CellularReturn returnType,
			float jitter) noexcept;

	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z,
			CellularDistance distanceFn,
			CellularReturn returnType,
			float jitter) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord,
			CellularDistance distanceFn,
			CellularReturn returnType,
			float jitter) const noexcept {
		return operator()(seed, coord.x, coord.y, distanceFn, returnType, jitter);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord,
			CellularDistance distanceFn,
			CellularReturn returnType,
			float jitter) const noexcept {
		return operator()(seed, coord.x, coord.y, coord.z, distanceFn, returnType, jitter);
	}
} cellular;

// --- Simplex Gradient ----------------------------------------------------------------------------------------

constexpr inline struct SimplexGradientFn {
	[[nodiscard]] static libv::vec2f operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static libv::vec3f operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE static libv::vec2f operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}

	[[nodiscard]] LIBV_FORCE_INLINE static libv::vec3f operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} simplex_grad;

// --- Fractal -------------------------------------------------------------------------------------

/// FBM (Fractional Brownian Motion)
template <typename NoiseFunc>
[[nodiscard]] constexpr inline auto fractal(
		Seed seed,
		float x, float y,
		NoiseFunc&& noiseFunc,
		int octaves,
		float amplitude = default_amplitude,
		float frequency = default_frequency,
		float lacunarity = default_lacunarity,
		float persistence = default_persistence) noexcept {
	using Result = std::invoke_result_t<NoiseFunc, Seed, float, float>;

	Result result{0};

	for (int32_t i = 0; i < octaves; ++i) {
		result += noiseFunc(seed++, x * frequency, y * frequency) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

/// FBM (Fractional Brownian Motion)
template <typename NoiseFunc>
[[nodiscard]] constexpr inline auto fractal(
		Seed seed,
		float x, float y, float z,
		NoiseFunc&& noiseFunc,
		int octaves,
		float amplitude = default_amplitude,
		float frequency = default_frequency,
		float lacunarity = default_lacunarity,
		float persistence = default_persistence) noexcept {
	using Result = std::invoke_result_t<NoiseFunc, Seed, float, float, float>;

	Result result{0};

	for (int32_t i = 0; i < octaves; ++i) {
		result += noiseFunc(seed++, x * frequency, y * frequency, z * frequency) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

template <typename NoiseFunc>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto fractal( Seed seed, libv::vec2f coord, NoiseFunc&& noiseFunc, int octaves, float amplitude = default_amplitude, float frequency = default_frequency, float lacunarity = default_lacunarity, float persistence = default_persistence) noexcept {
	return fractal(seed, coord.x, coord.y, noiseFunc, octaves, amplitude, frequency, lacunarity, persistence);
}

template <typename NoiseFunc>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto fractal( Seed seed, libv::vec3f coord, NoiseFunc&& noiseFunc, int octaves, float amplitude = default_amplitude, float frequency = default_frequency, float lacunarity = default_lacunarity, float persistence = default_persistence) noexcept {
	return fractal(seed, coord.x, coord.y, coord.z, noiseFunc, octaves, amplitude, frequency, lacunarity, persistence);
}

/// Homebrew progressive fractal
template <typename GradientNoiseFunc>
[[nodiscard]] constexpr inline auto fractal_progressive(
		Seed seed,
		float x, float y,
		GradientNoiseFunc&& noiseFunc,
		int octaves,
		float amplitude = default_amplitude,
		float frequency = default_frequency,
		float lacunarity = default_lacunarity,
		float persistence = default_persistence) noexcept {
	using Result = std::invoke_result_t<GradientNoiseFunc, Seed, float, float>;

	Result result{0};

	for (int32_t i = 0; i < octaves; ++i) {
		result += noiseFunc(seed, x * frequency + result.x, y * frequency + result.y) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

/// Homebrew progressive fractal
template <typename GradientNoiseFunc>
[[nodiscard]] constexpr inline auto fractal_progressive(
		Seed seed,
		float x, float y, float z,
		GradientNoiseFunc&& noiseFunc,
		int octaves,
		float amplitude = default_amplitude,
		float frequency = default_frequency,
		float lacunarity = default_lacunarity,
		float persistence = default_persistence) noexcept {
	using Result = std::invoke_result_t<GradientNoiseFunc, Seed, float, float, float>;

	Result result{0};

	for (int32_t i = 0; i < octaves; ++i) {
		result += noiseFunc(seed, x * frequency + result.x, y * frequency + result.y, z * frequency + result.z) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

/// Homebrew progressive fractal
template <typename GradientNoiseFunc>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto fractal_progressive( Seed seed, libv::vec2f coord, GradientNoiseFunc&& noiseFunc, int octaves, float amplitude = default_amplitude, float frequency = default_frequency, float lacunarity = default_lacunarity, float persistence = default_persistence) noexcept {
	return fractal_progressive(seed, coord.x, coord.y, noiseFunc, octaves, amplitude, frequency, lacunarity, persistence);
}

/// Homebrew progressive fractal
template <typename GradientNoiseFunc>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto fractal_progressive( Seed seed, libv::vec3f coord, GradientNoiseFunc&& noiseFunc, int octaves, float amplitude = default_amplitude, float frequency = default_frequency, float lacunarity = default_lacunarity, float persistence = default_persistence) noexcept {
	return fractal_progressive(seed, coord.x, coord.y, coord.z, noiseFunc, octaves, amplitude, frequency, lacunarity, persistence);
}

// -------------------------------------------------------------------------------------------------

} // namespace noise
} // namespace libv
