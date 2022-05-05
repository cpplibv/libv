#version 330 core


in vec2 fragmentTexture0;

//out vec4 result;
layout(location = 0) out vec4 resultColor;
layout(location = 1) out vec4 resultNormal;


uniform sampler2D textureColor;
uniform sampler2D textureNormal;
uniform int ssaaSamples; // Sample count in one dimension (whole sampling should be in a box)

// -------------------------------------------------------------------------------------------------

float rankNormal(vec3 normal) {
	return abs(normal.x) + abs(normal.y) + abs(normal.z);
}

vec3 find_any_color_near(sampler2D texture0, vec2 uv, vec2 pixelSize, float startDistance) {
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

vec3 find_any_normal_near(sampler2D texture0, vec2 uv, vec2 pixelSize, float startDistance) {
	for (float ring = startDistance; ring < startDistance + 32; ring += 1.0) {
		for (float i = -ring * 0.5; i < ring * 0.5; i++) {
			vec3 sample_;

			sample_ = texture(texture0, uv + vec2(i, ring) * pixelSize).rgb * 2.0 - 1.0;
			if (rankNormal(sample_) > 0.1)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(i, -ring) * pixelSize).rgb * 2.0 - 1.0;
			if (rankNormal(sample_) > 0.1)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(ring, i) * pixelSize).rgb * 2.0 - 1.0;
			if (rankNormal(sample_) > 0.1)
				return sample_.rgb;

			sample_ = texture(texture0, uv + vec2(-ring, i) * pixelSize).rgb * 2.0 - 1.0;
			if (rankNormal(sample_) > 0.1)
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

	vec2 inputPixelSize = 1.0 / textureSize(textureColor, 0);

	int numSampleInRow = ssaaSamples;

	vec4 sumColor = vec4(0, 0, 0, 0);
	vec3 sumNormal = vec3(0, 0, 0);
	int numColorSample = 0;
	int numNormalSample = 0;

	for (int y = 0; y < numSampleInRow; y++) {
		for (int x = 0; x < numSampleInRow; x++) {
			vec2 offset = -0.5 + (1.0 + 2.0 * vec2(x, y)) / (2.0 * numSampleInRow);
			vec4 sampleColor = texture(textureColor, fragmentTexture0 + inputPixelSize * offset).rgba;
			vec3 sampleNormal = texture(textureNormal, fragmentTexture0 + inputPixelSize * offset).rgb * 2.0 - 1.0; // Normals of [-1..1] is stored as [0..1]

			if (sampleColor.a > sumColor.a) {
				sumColor = sampleColor;
				numColorSample = 1;
			} else if (sampleColor.a > sumColor.a * 0.9) {
				sumColor.rgb += sampleColor.rgb;
				++numColorSample;
			}

			if (rankNormal(sampleNormal) > 0.1) {
				// Only allow non 'empty' normal data
				sumNormal += sampleNormal;
				++numNormalSample;
			}
		}
	}

	if (sumColor.a < 0.05 || numColorSample == 0) {
		resultColor.rgb = find_any_color_near(textureColor, fragmentTexture0, inputPixelSize, numSampleInRow / 2.0);
		resultColor.a = 0;
	} else {
		resultColor.rgb = sumColor.rgb / numColorSample;
		resultColor.a = min(sumColor.a, 1);
	}

	if (numNormalSample == 0) {
		resultNormal.rgb = find_any_normal_near(textureNormal, fragmentTexture0, inputPixelSize, numSampleInRow / 2.0) * 0.5 + 0.5;
		resultNormal.a = 1;
	} else {
		resultNormal = vec4(normalize(sumNormal), 1) * 0.5 + 0.5;
	}
}
