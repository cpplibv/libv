// Created by dbobula on 2/12/2022.


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename NoiseFn>
[[nodiscard]] float noise_fractal(Seed seed, T x, T y, NoiseFn noiseFn, int octaves,
		float amplitude, float frequency, float lacunarity, float persistence) {
	float sum = 0;

	for (int i = 0; i < octaves; ++i) {
		float noise = noiseFn(seed++, x * frequency, y * frequency);
		sum += noise * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return sum;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
