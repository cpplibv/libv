#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/re/outline/jump_flood.glsl>

layout(binding = TEXTURE_UNIT_JFA_MASK) uniform sampler2D textureMask;

layout(rg16_snorm, binding = TEXTURE_UNIT_JFA_STEP_OUTPUT) writeonly uniform image2DArray textureOutput;

// -------------------------------------------------------------------------------------------------

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	ivec2 coordsSize = imageSize(textureOutput).xy;
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	if (coords.x >= coordsSize.x || coords.y >= coordsSize.y)
		return;

	vec2 fragmentUV = (vec2(coords) + 0.5) / vec2(coordsSize);
	vec2 fragmentPosition = uvToPosition(fragmentUV);

	// Sample the mask texture for early out
	mat3 values;
	values[1][1] = texelFetch(textureMask, coords, 0).r;

	// Interior, return position
	if (values[1][1] > 0.99) {
		imageStore(textureOutput, ivec3(coords, 0), vec4(fragmentPosition, 0, 0));
		return;
	}

	// Exterior, return no position
	if (values[1][1] < 0.01) {
		imageStore(textureOutput, ivec3(coords, 0), vec4(FLOOD_NULL2, 0, 0));
		return;
	}

	// Sobel to estimate edge direction
	int xm = max(coords.x - 1, 0);
	int xc = coords.x;
	int xp = min(coords.x + 1, coordsSize.x - 1);
	int ym = max(coords.y - 1, 0);
	int yc = coords.y;
	int yp = min(coords.y + 1, coordsSize.y - 1);
	values[0][0] = texelFetch(textureMask, ivec2(xm, ym), 0).r;
	values[0][1] = texelFetch(textureMask, ivec2(xm, yc), 0).r;
	values[0][2] = texelFetch(textureMask, ivec2(xm, yp), 0).r;
	values[1][0] = texelFetch(textureMask, ivec2(xc, ym), 0).r;
	values[1][2] = texelFetch(textureMask, ivec2(xc, yp), 0).r;
	values[2][0] = texelFetch(textureMask, ivec2(xp, ym), 0).r;
	values[2][1] = texelFetch(textureMask, ivec2(xp, yc), 0).r;
	values[2][2] = texelFetch(textureMask, ivec2(xp, yp), 0).r;

	vec2 dir = -vec2(
		values[0][0] + values[0][1] * 2.0 + values[0][2] - values[2][0] - values[2][1] * 2.0 - values[2][2],
		values[0][0] + values[1][0] * 2.0 + values[2][0] - values[0][2] - values[1][2] * 2.0 - values[2][2]
	);

	// If dir length is small, this is either a sub pixel dot or line
	// no way to estimate sub pixel edge, so output position
	if (abs(dir.x) <= 0.005 && abs(dir.y) <= 0.005) {
		imageStore(textureOutput, ivec3(coords, 0), vec4(fragmentPosition, 0, 0));
		return;
	}

	// Normalize direction
	dir = normalize(dir);

	// Sub pixel offset
	vec2 offset = dir * (1.0 - values[1][1]) * pixelSize;

	// Output encoded offset position
	imageStore(textureOutput, ivec3(coords, 0), vec4(uvToPosition(fragmentUV + offset), 0, 0));
}
