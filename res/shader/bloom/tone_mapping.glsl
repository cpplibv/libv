#pragma once

// -------------------------------------------------------------------------------------------------

float tonemapAces(float x) {
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 tonemapAces(vec3 x) {
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// -------------------------------------------------------------------------------------------------

float tonemapLottes(float x) {
	const float a = 1.6;
	const float d = 0.977;
	const float hdrMax = 8.0;
	const float midIn = 0.18;
	const float midOut = 0.267;

	const float b =
		(-pow(midIn, a) + pow(hdrMax, a) * midOut) /
		((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);
	const float c =
		(pow(hdrMax, a * d) * pow(midIn, a) - pow(hdrMax, a) * pow(midIn, a * d) * midOut) /
		((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);

	return pow(x, a) / (pow(x, a * d) * b + c);
}

vec3 tonemapLottes(vec3 x) {
	const vec3 a = vec3(1.6);
	const vec3 d = vec3(0.977);
	const vec3 hdrMax = vec3(8.0);
	const vec3 midIn = vec3(0.18);
	const vec3 midOut = vec3(0.267);

	const vec3 b =
		(-pow(midIn, a) + pow(hdrMax, a) * midOut) /
		((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);
	const vec3 c =
		(pow(hdrMax, a * d) * pow(midIn, a) - pow(hdrMax, a) * pow(midIn, a * d) * midOut) /
		((pow(hdrMax, a * d) - pow(midIn, a * d)) * midOut);

	return pow(x, a) / (pow(x, a * d) * b + c);
}

// -------------------------------------------------------------------------------------------------

float tonemapReinhard(float x) {
	return x / (1.0 + x);
}

vec3 tonemapReinhard(vec3 x) {
	return x / (1.0 + x);
}

// -------------------------------------------------------------------------------------------------

float tonemapReinhard2(float x) {
	const float L_white = 4.0;

	return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

vec3 tonemapReinhard2(vec3 x) {
	const float L_white = 4.0;

	return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

// -------------------------------------------------------------------------------------------------

float tonemapFilmic(float x) {
	float X = max(0.0, x - 0.004);
	float result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return pow(result, 2.2);
}

vec3 tonemapFilmic(vec3 x) {
	vec3 X = max(vec3(0.0), x - 0.004);
	vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return pow(result, vec3(2.2));
}

// -------------------------------------------------------------------------------------------------

float tonemapUchimura(float x, float P, float a, float m, float l, float c, float b) {
	float l0 = ((P - m) * l) / a;
	float L0 = m - m / a;
	float L1 = m + (1.0 - m) / a;
	float S0 = m + l0;
	float S1 = m + a * l0;
	float C2 = (a * P) / (P - S1);
	float CP = -C2 / P;

	float w0 = 1.0 - smoothstep(0.0, m, x);
	float w2 = step(m + l0, x);
	float w1 = 1.0 - w0 - w2;

	float T = m * pow(x / m, c) + b;
	float S = P - (P - S1) * exp(CP * (x - S0));
	float L = m + a * (x - m);

	return T * w0 + L * w1 + S * w2;
}

float tonemapUchimura(float x) {
	const float P = 1.0;// max display brightness
	const float a = 1.0;// contrast
	const float m = 0.22;// linear section start
	const float l = 0.4;// linear section length
	const float c = 1.33;// black
	const float b = 0.0;// pedestal

	return tonemapUchimura(x, P, a, m, l, c, b);
}

vec3 tonemapUchimura(vec3 x, float P, float a, float m, float l, float c, float b) {
	float l0 = ((P - m) * l) / a;
	float L0 = m - m / a;
	float L1 = m + (1.0 - m) / a;
	float S0 = m + l0;
	float S1 = m + a * l0;
	float C2 = (a * P) / (P - S1);
	float CP = -C2 / P;

	vec3 w0 = vec3(1.0 - smoothstep(0.0, m, x));
	vec3 w2 = vec3(step(m + l0, x));
	vec3 w1 = vec3(1.0 - w0 - w2);

	vec3 T = vec3(m * pow(x / m, vec3(c)) + b);
	vec3 S = vec3(P - (P - S1) * exp(CP * (x - S0)));
	vec3 L = vec3(m + a * (x - m));

	return T * w0 + L * w1 + S * w2;
}

vec3 tonemapUchimura(vec3 x) {
	const float P = 1.0;// max display brightness
	const float a = 1.0;// contrast
	const float m = 0.22;// linear section start
	const float l = 0.4;// linear section length
	const float c = 1.33;// black
	const float b = 0.0;// pedestal

	return tonemapUchimura(x, P, a, m, l, c, b);
}

// -------------------------------------------------------------------------------------------------

float tonemapUncharted2Tonemap(float x) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float tonemapUncharted2(float color) {
	const float W = 11.2;
	const float exposureBias = 2.0;
	float curr = tonemapUncharted2Tonemap(exposureBias * color);
	float whiteScale = 1.0 / tonemapUncharted2Tonemap(W);
	return curr * whiteScale;
}

vec3 tonemapUncharted2Tonemap(vec3 x) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 tonemapUncharted2(vec3 color) {
	const float W = 11.2;
	float exposureBias = 2.0;
	vec3 curr = tonemapUncharted2Tonemap(exposureBias * color);
	vec3 whiteScale = 1.0 / tonemapUncharted2Tonemap(vec3(W));
	return curr * whiteScale;
}

// -------------------------------------------------------------------------------------------------

// Gamma 2.2 correction is baked in, don't use with sRGB conversion!
float tonemapUnreal(float x) {
	return x / (x + 0.155) * 1.019;
}

// Gamma 2.2 correction is baked in, don't use with sRGB conversion!
vec3 tonemapUnreal(vec3 x) {
	return x / (x + 0.155) * 1.019;
}

// -------------------------------------------------------------------------------------------------

// Helper for neutral tonemapping
vec3 neutralCurve(vec3 x, float a, float b, float c, float d, float e, float f) {
	return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

// Neutral tonemapping (Hable/Hejl/Frostbite)
// Input is linear RGB
vec3 tonemapNeutral(vec3 x) {
	const float a = 0.2;
	const float b = 0.29;
	const float c = 0.24;
	const float d = 0.272;
	const float e = 0.02;
	const float f = 0.3;
	const float whiteLevel = 5.3;
	const float whiteClip = 1.0;

	vec3 whiteScale = vec3(1.0) / neutralCurve(vec3(whiteLevel), a, b, c, d, e, f);
	x = neutralCurve(x * whiteScale, a, b, c, d, e, f);
	x *= whiteScale;

	x /= vec3(whiteClip);

	return x;
}

// -------------------------------------------------------------------------------------------------
