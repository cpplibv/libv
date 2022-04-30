#version 330 core


in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;
uniform int ssaaSamples; // Sample count in one dimension (whole sampling should be in a box)
uniform bool isColor; // Upsampling happens on: True = colors, False = normals

// -------------------------------------------------------------------------------------------------

void main() {

	// |---|---|---|---|
	//   ^   ^   ^   ^
	//         v
	// |---------------|
	// 4 -> -3/8, -1/8, 1/8, 3/8
	// N -> -0.5 + (1 + 2K) / 2N : K={0..N}

	ivec2 size = textureSize(texture0, 0);
	vec2 input_pixel_size = 1.0 / size;

	int samples = ssaaSamples;

	vec4 color;
	for (int y = 0; y < samples; y++) {
		for (int x = 0; x < samples; x++) {
			vec2 offset = -0.5 + (1.0 + 2.0 * vec2(x, y)) / (2.0 * samples);
			vec4 sample_ = texture(texture0, fragmentTexture0 + input_pixel_size * offset).rgba;

			if (isColor)
				color += sample_;
			else // isNormal
				color += (sample_ * 2.0 - 1.0) * 0.5 + 0.5;
		}
	}

	if (isColor) {
		// For RGB only AVG the "exciting opacity fragments"
		// color /= samples * samples;
		color.rgb /= color.a;
		color.a /= samples * samples;
		color = clamp(color, 0, 1);
	} else {
		color /= samples * samples;
//		color = clamp(color, 0, 1);
		color.rgb =	normalize(color.rgb * 2.0 - 1.0) * 0.5 + 0.5;
//		color.rgb =	normalize(color);
		color.a = 1;
	}

	result = color;
}
