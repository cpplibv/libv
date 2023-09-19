#version 450 core

#include <lib/noise/noise.glsl>


layout(local_size_x = 8, local_size_y = 8) in;

layout(r32f, binding = 0) uniform image2D target;
uniform float time;
uniform sampler2D texture_cpu;

void main() {
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 coordsSize = ivec2(gl_NumWorkGroups.xy * gl_WorkGroupSize.xy);
	vec2 uv = vec2(coords) / vec2(coordsSize);


	//	vec3 pos = vec3(uv * vec2(10.0, 10.0), time * 0.25);
	//
	//	pos.xy = seed(pos.xy, 0);
	//
	//	float value = noise_fractal_simplex_3d(pos, 6, 1.0, 1.0, 2.0, 0.5) * 0.5 + 0.5;

	//	float value = simplex(0x5EED, vec3(uv * 10, time * 0.4)) * 0.5 + 0.5;
	//	float value = simplex(0x5EED, uv) * 0.5 + 0.5;
	//	float value = simplex(0x5EED, uv) * 0.5 + 0.5;
	//	float value = simplex(0x5EED, vec3(uv, 3.1415f)) * 0.5 + 0.5;
	//	float value = simplex(0x5EED, vec3(uv, 3.1415)) * 0.5 + 0.5;
//		float value = cellular(0x5EED, vec2(uv), FNL_CELLULAR_DISTANCE_EUCLIDEAN, FNL_CELLULAR_RETURN_TYPE_CELLVALUE, 1.0f) * 0.5 + 0.5;

//	float value = cellular(0x5EED, vec3(uv * 20, time * 0.4), 1, 1, 1.0f) * 0.5 + 0.5;
	//	float value = simplex(0x5EED, uv * 20) * 0.5 + 0.5;
	//	float value = uv.y;

//	float value = cellular(0x511D, vec2(uv * 20.f),
//		FNL_CELLULAR_DISTANCE_EUCLIDEAN,
//		FNL_CELLULAR_RETURN_TYPE_CELLVALUE,
//		abs(fract(time * 0.4) * 2 - 1) * 1.f) * 0.5 + 0.5;
	uint seed_offset = coords.x / 64;
	uint seed = 0x5EED + seed_offset;
	float cpu = texture(texture_cpu, uv + .5 / textureSize(texture_cpu, 0).xy).r;
	float gpu = simplex(seed, uv*10) * 0.5 + 0.5;
//	vec2 cpu_warp = texture(texture_cpu, uv + .5 / textureSize(texture_cpu, 0).xy).gb;
//	vec2 warp = fractal_simplex_grad_progressive(0x511D, uv * 5.f, 5, 50.f, 1.0f, 2.0f, 0.5f).xy;
//	vec2 warp = fractal_simplex_grad_progressive(0x511D, vec3(uv * 5.f, time * 0.2), 5, 50.f, 1.0f, 2.0f, 0.5f).xy;
//	vec2 warp = fractal_simplex_grad_progressive(0x511D, vec3(uv * 2.f, time * 0.2), 5, sin(time * 0.6) * 8.f, 1.0f, 2.0f, 0.5f).xy;
//	vec2 warp = fractal_simplex_grad_independent(0x511D, vec3(uv * 5.f, time * 0.2), 5, sin(time * 0.6) * 50.f, 1.0f, 2.0f, 0.5f).xy;
//	vec2 warp = fractal_simplex_grad_progressive(0x511D, vec3(uv * 5.f, time * 0.2), 5, sin(time * 0.6) * 50.f, 1.0f, 2.0f, 0.5f).xy;
//	value = cpu;

//	float value = cellular(0x511D, vec3(uv * 20.f + warp, 32*20.f),
//			FNL_CELLULAR_DISTANCE_EUCLIDEANSQ,
//			FNL_CELLULAR_RETURN_TYPE_DISTANCE,
////			FNL_CELLULAR_RETURN_TYPE_CELLVALUE,
//			1.0f) * 0.5 + 0.5;

//	float value = cellular(0x511D, vec3(uv * 10.f + warp, 32*20.f),
//			FNL_CELLULAR_DISTANCE_EUCLIDEAN,
////			FNL_CELLULAR_RETURN_TYPE_DISTANCE,
//			FNL_CELLULAR_RETURN_TYPE_CELLVALUE,
//			0.0f) * 0.5 + 0.5;

//	float value = step(0.65, length(warp + uv - 0.5) * 2.f) - 0.4;
//	float value = smoothstep(0.55, 0.75, length(warp + uv - 0.5) * 2.f);
//	float value = length(warp + uv - 0.5);

//	value = abs(cpu_warp.x - warp.x) + abs(cpu_warp.y - warp.y);

//	value = pow(cpu * 1.75, 3);
//	float cpu = texture(texture_cpu, uv).r;
//	value = abs(value - cpu);

//	if (fract(time * 0.2) < 0.4)
//		value = cpu;
//	else if (fract(time * 0.2) < 0.8)
//		value = value;
//	else
//		value = abs(cpu - value);

	vec4 result = vec4(gpu - cpu, 0, 0, 1);
	imageStore(target, coords, result);
}