#pragma once

// -------------------------------------------------------------------------------------------------

vec4 downsampleBox4(sampler2D tex, vec2 uv, vec2 pixelSize) {
	vec4 sum =
			texture(tex, uv + pixelSize * vec2(-1.0, -1.0)) +
			texture(tex, uv + pixelSize * vec2(+1.0, -1.0)) +
			texture(tex, uv + pixelSize * vec2(-1.0, +1.0)) +
			texture(tex, uv + pixelSize * vec2(+1.0, +1.0));

	return sum / 4.0;
}

vec4 downsampleBox13(sampler2D tex, vec2 uv, vec2 pixelSize) {

	// .  .  .  .  .  .  .
	// .  A  .  B  .  C  .
	// .  .  D  .  E  .  .
	// .  F  .  G  .  H  .
	// .  .  I  .  J  .  .
	// .  K  .  L  .  M  .
	// .  .  .  .  .  .  .

	vec4 A = texture(tex, uv + pixelSize * vec2(-1.0, -1.0));
	vec4 B = texture(tex, uv + pixelSize * vec2(+0.0, -1.0));
	vec4 C = texture(tex, uv + pixelSize * vec2(+1.0, -1.0));
	vec4 D = texture(tex, uv + pixelSize * vec2(-0.5, -0.5));
	vec4 E = texture(tex, uv + pixelSize * vec2(+0.5, -0.5));
	vec4 F = texture(tex, uv + pixelSize * vec2(-1.0, +0.0));
	vec4 G = texture(tex, uv + pixelSize * vec2(+0.0, +0.0));
	vec4 H = texture(tex, uv + pixelSize * vec2(+1.0, +0.0));
	vec4 I = texture(tex, uv + pixelSize * vec2(-0.5, +0.5));
	vec4 J = texture(tex, uv + pixelSize * vec2(+0.5, +0.5));
	vec4 K = texture(tex, uv + pixelSize * vec2(-1.0, +1.0));
	vec4 L = texture(tex, uv + pixelSize * vec2(+0.0, +1.0));
	vec4 M = texture(tex, uv + pixelSize * vec2(+1.0, +1.0));

	vec4 result =
			(D + E + J + I) * 0.5 / 4.0 +
			(A + B + G + F) * 0.125 / 4.0 +
			(B + C + H + G) * 0.125 / 4.0 +
			(F + G + L + K) * 0.125 / 4.0 +
			(G + H + M + L) * 0.125 / 4.0;

	return result;
}

/// downsampleBox13 with Partial Karis Average
/// Uses the weighted avarage for the 5 h4x4box
vec4 downsampleBox13Balanced(sampler2D tex, vec2 uv, vec2 pixelSize) {

	// .  .  .  .  .  .  .
	// .  A  .  B  .  C  .
	// .  .  D  .  E  .  .
	// .  F  .  G  .  H  .
	// .  .  I  .  J  .  .
	// .  K  .  L  .  M  .
	// .  .  .  .  .  .  .

	vec4 A = texture(tex, uv + pixelSize * vec2(-1.0, -1.0));
	vec4 B = texture(tex, uv + pixelSize * vec2(+0.0, -1.0));
	vec4 C = texture(tex, uv + pixelSize * vec2(+1.0, -1.0));
	vec4 D = texture(tex, uv + pixelSize * vec2(-0.5, -0.5));
	vec4 E = texture(tex, uv + pixelSize * vec2(+0.5, -0.5));
	vec4 F = texture(tex, uv + pixelSize * vec2(-1.0, +0.0));
	vec4 G = texture(tex, uv + pixelSize * vec2(+0.0, +0.0));
	vec4 H = texture(tex, uv + pixelSize * vec2(+1.0, +0.0));
	vec4 I = texture(tex, uv + pixelSize * vec2(-0.5, +0.5));
	vec4 J = texture(tex, uv + pixelSize * vec2(+0.5, +0.5));
	vec4 K = texture(tex, uv + pixelSize * vec2(-1.0, +1.0));
	vec4 L = texture(tex, uv + pixelSize * vec2(+0.0, +1.0));
	vec4 M = texture(tex, uv + pixelSize * vec2(+1.0, +1.0));

	vec4 hboxCC = (D + E + J + I) / 4.0;
	vec4 hboxTL = (A + B + G + F) / 4.0;
	vec4 hboxTR = (B + C + H + G) / 4.0;
	vec4 hboxBL = (F + G + L + K) / 4.0;
	vec4 hboxBR = (G + H + M + L) / 4.0;

	// 	weight = contribution * 1.0 / (1.0 + brigthness);
	float weightCC = 0.500 * 1.0 / (1.0 + max(hboxCC.r, max(hboxCC.g, hboxCC.b)));
	float weightTL = 0.125 * 1.0 / (1.0 + max(hboxTL.r, max(hboxTL.g, hboxTL.b)));
	float weightTR = 0.125 * 1.0 / (1.0 + max(hboxTR.r, max(hboxTR.g, hboxTR.b)));
	float weightBL = 0.125 * 1.0 / (1.0 + max(hboxBL.r, max(hboxBL.g, hboxBL.b)));
	float weightBR = 0.125 * 1.0 / (1.0 + max(hboxBR.r, max(hboxBR.g, hboxBR.b)));

	float weightSum = weightCC + weightTL + weightTR + weightBL + weightBR;

	return
			(weightCC / weightSum) * hboxCC +
			(weightTL / weightSum) * hboxTL +
			(weightTR / weightSum) * hboxTR +
			(weightBL / weightSum) * hboxBL +
			(weightBR / weightSum) * hboxBR;
}

/// 9-tap gaussian blur on the downsampling source
vec4 downsampleHorizontalBlur9(sampler2D tex, vec2 uv, vec2 pixelSize) {
	return
			texture(tex, uv + pixelSize * vec2(-4.0, 0.0)) * 0.01621622 +
			texture(tex, uv + pixelSize * vec2(-3.0, 0.0)) * 0.05405405 +
			texture(tex, uv + pixelSize * vec2(-2.0, 0.0)) * 0.12162162 +
			texture(tex, uv + pixelSize * vec2(-1.0, 0.0)) * 0.19459459 +
			texture(tex, uv + pixelSize * vec2(+0.0, 0.0)) * 0.22702703 +
			texture(tex, uv + pixelSize * vec2(+1.0, 0.0)) * 0.19459459 +
			texture(tex, uv + pixelSize * vec2(+2.0, 0.0)) * 0.12162162 +
			texture(tex, uv + pixelSize * vec2(+3.0, 0.0)) * 0.05405405 +
			texture(tex, uv + pixelSize * vec2(+4.0, 0.0)) * 0.01621622;
}

/// (9-tap equivalent) bilinear 5-tap gaussian on the same-sized source
vec4 downsampleVerticalBlur5(sampler2D tex, vec2 uv, vec2 pixelSize) {
	return
			texture(tex, uv + pixelSize * vec2(0.0, -3.23076923)) * 0.07027027 +
			texture(tex, uv + pixelSize * vec2(0.0, -1.38461538)) * 0.31621622 +
			texture(tex, uv + pixelSize * vec2(0.0, +0.00000000)) * 0.22702703 +
			texture(tex, uv + pixelSize * vec2(0.0, +1.38461538)) * 0.31621622 +
			texture(tex, uv + pixelSize * vec2(0.0, +3.23076923)) * 0.07027027;
}

// -------------------------------------------------------------------------------------------------

vec4 upsampleBox4(sampler2D tex, vec2 uv, vec2 pixelSize) {
	vec4 sum =
			texture(tex, uv + pixelSize * vec2(-1, -1)) +
			texture(tex, uv + pixelSize * vec2(+1, -1)) +
			texture(tex, uv + pixelSize * vec2(-1, +1)) +
			texture(tex, uv + pixelSize * vec2(+1, +1));

	return sum / 4.0;
}

vec4 upsampleTent9(sampler2D tex, vec2 uv, vec2 pixelSize, float sampleScale) {
	vec4 sum =
			texture(tex, uv + pixelSize * sampleScale * vec2(-1, -1)) +
			texture(tex, uv + pixelSize * sampleScale * vec2(+0, -1)) * 2.0 +
			texture(tex, uv + pixelSize * sampleScale * vec2(+1, -1)) +

			texture(tex, uv + pixelSize * sampleScale * vec2(-1, +0)) * 2.0 +
			texture(tex, uv + pixelSize * sampleScale * vec2(+0, +0)) * 4.0 +
			texture(tex, uv + pixelSize * sampleScale * vec2(+1, +0)) * 2.0 +

			texture(tex, uv + pixelSize * sampleScale * vec2(-1, +1)) +
			texture(tex, uv + pixelSize * sampleScale * vec2(+0, +1)) * 2.0 +
			texture(tex, uv + pixelSize * sampleScale * vec2(+1, +1));

	return sum / 16.0;
}

// -------------------------------------------------------------------------------------------------
