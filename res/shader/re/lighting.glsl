


float sqr(float x) {
	return x * x;
}

// -------------------------------------------------------------------------------------------------

float lightDistanceAttenuatePhysicalCustom(float dist, float constant, float linear, float exponent) {
	return 1.0 / (constant + dist * linear + dist * dist * exponent);
}

/// Range is not limiting! It represents where the initial light drops to 25%
float lightDistanceAttenuatePhysical(float dist, float range) {
//	return lightDistanceAttenuatePhysicalCustom(dist, 1.0, 2.0 / range, 1.0 / (range * range));
	return sqr(range) / sqr(dist + range); // Matches the comment above, but simplified
}

float lightDistanceAttenuateLisyarus(float dist, float range, float falloff) {
	float s = dist / range;
	if (s >= 1.0)
		return 0.0;

	float s2 = sqr(s);
	return sqr(1 - s2) / (1 + falloff * s2);
}

float lightDistanceAttenuateLisyarusSharp(float dist, float range, float falloff) {
	float s = dist / range;
	if (s >= 1.0)
		return 0.0;

	float s2 = sqr(s);
	return sqr(1 - s2) / (1 + falloff * s);
}

// -------------------------------------------------------------------------------------------------

float lightDistanceAttenuation(uint formula, float dist, float range, float falloff) {
	if (formula == LIGHT_ATTENUATION_FORMULA_PHYSICAL)
		return lightDistanceAttenuatePhysical(dist, range);
	else if (formula == LIGHT_ATTENUATION_FORMULA_LISYARUS)
		return lightDistanceAttenuateLisyarus(dist, range, falloff);
	else if (formula == LIGHT_ATTENUATION_FORMULA_LISYARUS_SHARP)
		return lightDistanceAttenuateLisyarusSharp(dist, range, falloff);
	else
		return 0.f;
}

// -------------------------------------------------------------------------------------------------

float lightSpotAttenuate(vec3 directionW, vec3 L, float cosAngleInner, float cosAngleOuter) {
	float attenuation = 0;
	attenuation = max(dot(-directionW, L), 0.0); // SpotDir Dot L

	if (cosAngleInner == cosAngleOuter) {
		attenuation = step(cosAngleInner, attenuation); //Spot sharp Cone
	} else {
		float ioCosAngleDif = 1.0 / (cosAngleInner - cosAngleOuter);
		attenuation = clamp(attenuation * ioCosAngleDif - cosAngleOuter * ioCosAngleDif, 0.0, 1.0); //Spot Cone
	}
	return attenuation;
}
