#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/re/outline/jump_flood.glsl>

in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_JFA_MASK) uniform sampler2D textureMask;
layout(binding = TEXTURE_UNIT_JFA_STEP) uniform sampler2DArray textureDiffuse;

out vec4 outputColor;

// -------------------------------------------------------------------------------------------------

layout(std140) uniform JFAOutline {
	int sourceLayer;
	float outlineRadius;
	float outlineColorIDDecoder;
	vec4 outlineColors[8];
};

// -------------------------------------------------------------------------------------------------

void main() {
	ivec2 coords = ivec2(fragmentUV * canvasSize);
	vec2 closestPosition = texelFetch(textureDiffuse, ivec3(coords, sourceLayer), 0).rg;
	if (isNullPosition(closestPosition)) {
		outputColor = vec4(0);
		return;
	}

	vec2 fragmentPosition = uvToPositionWithPackLoss(fragmentUV);
	float distance = positionToPixelDistance(fragmentPosition - closestPosition);

	// Mask off any internal rounding/packing/floating artifact
	distance *= smoothstep(1.5 / 16.0, 3.0 / 16.0, distance);

	// Lookup color
	vec2 closestUV = positionToUV(closestPosition);
	vec2 mask = texture(textureMask, closestUV).rg;
	float colorID = mask.g / mask.r;
	uint colorIndex = uint(colorID * outlineColorIDDecoder + 0.5);
	outputColor = outlineColors[colorIndex];

	// Apply outline mask
	outputColor.a *= clamp(outlineRadius - distance, 0, 1); // Mask outside the outline
	outputColor.a *= clamp(distance, 0, 1); // Mask inside the object
}
