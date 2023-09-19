#version 450 core

#include <shader/re/outline/jump_flood.glsl>

layout(binding = TEXTURE_UNIT_JFA_STEP) uniform sampler2DArray textureDiffuse;
uniform ivec3 packStepSourceTarget;

layout(rg16_snorm, binding = TEXTURE_UNIT_JFA_STEP_OUTPUT) writeonly uniform image2DArray textureOutput;

// -------------------------------------------------------------------------------------------------

void check(inout float closestDistanceSQ, inout vec2 closestPosition, vec2 fragmentPosition, ivec3 probeCoords) {
	vec2 probePosition = texelFetch(textureDiffuse, probeCoords, 0).rg;
	if (isNullPosition(probePosition))
		return;

	vec2 diff = fragmentPosition - probePosition;
	float distanceSQ = dot(diff, diff);

	if (closestDistanceSQ > distanceSQ) {
		closestDistanceSQ = distanceSQ;
		closestPosition = probePosition;
	}
}

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	int stepWidth = packStepSourceTarget.x;
	int sourceLayer = packStepSourceTarget.y;
	int targetLayer = packStepSourceTarget.z;

	ivec2 coordsSize = imageSize(textureOutput).xy;
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	if (coords.x >= coordsSize.x || coords.y >= coordsSize.y)
		return;

	vec2 fragmentUV = (vec2(coords) + 0.5) / vec2(coordsSize);
	vec2 fragmentPosition = uvToPosition(fragmentUV);

	float closestDistanceSQ = inf;
	vec2 closestPosition = FLOOD_NULL2;

	// Find closest from jump samples
	int xm = max(coords.x - stepWidth, 0);
	int xc = coords.x;
	int xp = min(coords.x + stepWidth, coordsSize.x - 1);
	int ym = max(coords.y - stepWidth, 0);
	int yc = coords.y;
	int yp = min(coords.y + stepWidth, coordsSize.y - 1);
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xm, ym, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xm, yc, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xm, yp, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xc, ym, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xc, yc, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xc, yp, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xp, ym, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xp, yc, sourceLayer));
	check(closestDistanceSQ, closestPosition, fragmentPosition, ivec3(xp, yp, sourceLayer));
	// NOTE: There is a possibility to implement JFA Axis separated (2x3 tap) intead of combined (1x9 tap)
	//		but the 4 extra texture read is faster than the overhead of a completely new pass

	imageStore(textureOutput, ivec3(coords, targetLayer), vec4(closestPosition, 0, 0));
}
