#version 330 core

#include <block/matrices.glsl>
#include <builtin/sprite.glsl>
#include <builtin/sprite_definition.glsl>
#include <lib/constants.glsl>
#include <lib/quat.glsl>


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in GeometryData vs_out[1];

out FragmentData fs_in;

//uniform vec2 render_resolution; // Program block


// -------------------------------------------------------------------------------------------------

void main() {
	//      Input Point     Output Vertices    Output Triangles
	//
	//                        3---------2        +---------+
	//                        |         |        | 1   __/ |
	//           0            |         |        |  __/    |
	//                        |         |        | /    0  |
	//                        0---------1        +---------+

	// -------------------------------------------------------------------------------------------------

	// Early discard: visibility below 2% is discarded
	if (vs_out[0].fogFactor > 0.98)
		return;

	// -------------------------------------------------------------------------------------------------

	SpriteDefinition def = spriteDefinitions[vs_out[0].type];
	//	def.tile_num_x
	//	def.tile_num_y
	//	def.model_offset
	//	def.model_scale
	//	def.angle_x_min
	//	def.angle_x_max
	//	def.angle_y_min
	//	def.angle_y_max

	// -------------------------------------------------------------------------------------------------

	//	vec4 origin = gl_in[0].gl_Position;
	//	origin /= origin.w;

	//	vec2 pixel_size = 2.0 / render_resolution;
	//	vec2 pixel_size = vec2(0.09, 0.16);

	// gl_PrimitiveIDIn

	// -------------------------------------------------------------------------------------------------

	// View direction / rotation
	vec4 rotationQuat = qmul(
			qfromUnitVectors(vec3(0, 0, 1), vs_out[0].normalUp),
			qAngleAxis(vs_out[0].rotation, vec3(0, 0, 1))
	);

	vec3 localEyeDir = qrotate(qinverse(rotationQuat), vs_out[0].eyeDir);
	vec2 dirXY = normalize(-localEyeDir.xy);
	float dirZ = localEyeDir.z;

	// Tile selection
	float horizontal = def.tile_num_x + -(atan(dirXY.y, dirXY.x) / tau + 0.5) * def.tile_num_x;
	float horizontal_tile = round(horizontal);

	float vertical = max(0, (asin(dirZ) / (pi / 2.0)) * (def.tile_num_y - 1.0));
	float vertical_tile = round(vertical);

	// Sprite space
	vec3 center = vs_out[0].positionW.xyz;
	vec3 forward = normalize(center - eye);
	vec3 right = normalize(cross(forward, vs_out[0].normalUp));
	vec3 up = cross(right, forward);

	center += qrotate(rotationQuat, def.model_offset * vs_out[0].scale);

	float spriteHalfSize = def.model_scale * vs_out[0].scale * 0.5;

	// -------------------------------------------------------------------------------------------------

	fs_in.tile_index = vec2(horizontal_tile, vertical_tile);
//	fs_in.dither0 = fract(angleZ / 3.14 * 2.0 * 8.0);
//	fs_in.dither1 = fract(angleXY / 3.14 / 2.0 * 16.0);
	fs_in.rotationQuat = rotationQuat;
	fs_in.hsvColorShift = vs_out[0].hsvColorShift;
	fs_in.type = vs_out[0].type;
	fs_in.fogFactor = vs_out[0].fogFactor;

	// v1
	// fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(1, 0);
	fs_in.positionW = (matM * vec4(center + right * spriteHalfSize - up * spriteHalfSize, 1)).xyz;
	gl_Position = matMVP * vec4(center + right * spriteHalfSize - up * spriteHalfSize, 1);
	EmitVertex();

	// v2
	// fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(1, 1);
	fs_in.positionW = (matM * vec4(center + right * spriteHalfSize + up * spriteHalfSize, 1)).xyz;
	gl_Position = matMVP * vec4(center + right * spriteHalfSize + up * spriteHalfSize, 1);
	EmitVertex();

	// v0
	// fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(0, 0);
	fs_in.positionW = (matM * vec4(center - right * spriteHalfSize - up * spriteHalfSize, 1)).xyz;
	gl_Position = matMVP * vec4(center - right * spriteHalfSize - up * spriteHalfSize, 1);
	EmitVertex();

	// v3
	// fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(0, 1);
	fs_in.positionW = (matM * vec4(center - right * spriteHalfSize + up * spriteHalfSize, 1)).xyz;
	gl_Position = matMVP * vec4(center - right * spriteHalfSize + up * spriteHalfSize, 1);
	EmitVertex();

	EndPrimitive();

	// -------------------------------------------------------------------------------------------------
}
