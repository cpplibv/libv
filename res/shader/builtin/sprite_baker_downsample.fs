#version 330 core


in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;
uniform int ssaaSamples; // Sample count in one dimension (whole sampling should be in a box)
uniform bool isColor; // Upsampling happens on: True = colors, False = normals

// -------------------------------------------------------------------------------------------------

vec3 find_any_color_near(vec2 uv, vec2 pixelSize, float startDistance) {
	for (float ring = startDistance; ring < startDistance + 32; ring += 1.0) {
		for (float i = -ring * 0.5; i < ring * 0.5; i++) {
			vec4 sample_;

			sample_ = texture(texture0, uv + vec2(i, ring) * pixelSize).rgba;
			if (sample_.a > 0.9)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(i, -ring) * pixelSize).rgba;
			if (sample_.a > 0.9)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(ring, i) * pixelSize).rgba;
			if (sample_.a > 0.9)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(-ring, i) * pixelSize).rgba;
			if (sample_.a > 0.9)
				return sample_.rgb;
		}
	}

	return vec3(0.5, 0.5, 0.5);
}

void main() {
	// |---|---|---|---|
	//   ^   ^   ^   ^
	//         v
	// |---------------|
	// 4 -> -3/8, -1/8, 1/8, 3/8
	// N -> -0.5 + (1 + 2K) / 2N : K={0..N}

	vec2 inputPixelSize = 1.0 / textureSize(texture0, 0);

	int samples = ssaaSamples;

	vec4 color = vec4(0, 0, 0, 0);
	int sample_used = 0;

	for (int y = 0; y < samples; y++) {
		for (int x = 0; x < samples; x++) {
			vec2 offset = -0.5 + (1.0 + 2.0 * vec2(x, y)) / (2.0 * samples);
			vec4 sample_ = texture(texture0, fragmentTexture0 + inputPixelSize * offset).rgba;

			if (sample_.a > color.a) {
				color = sample_;
				sample_used = 1;
			} else if (sample_.a > color.a * 0.9) {
				color.rgb += sample_.rgb;
				++sample_used;
			}
		}
	}

	if (color.a < 0.05 || sample_used == 0) {
		// find closest anything
		color.rgb = find_any_color_near(fragmentTexture0, inputPixelSize, samples / 2.0);
		color.a = 0;

	} else {
		color.rgb /= sample_used;
		color.a = min(color.a, 1);
	}

	if (isColor) {
	} else { // isNormal
		color.a = 1;
	}

	result = color;
}
