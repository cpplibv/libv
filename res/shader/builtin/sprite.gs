#version 330 core

#include <builtin/sprite.glsl>
#include <builtin/sprite_definition.glsl>
#include <rev_sandbox/block/matrices.glsl>


layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in GeometryData vs_out[1];

out FragmentData fs_in;

//uniform vec2 render_resolution; // Program block


// -------------------------------------------------------------------------------------------------

const float pi = 3.14159265359;
const float tau = 2 * pi;

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

	vec2 dirXY = normalize(-vs_out[0].eyeDir.xy);
	float dirZ = vs_out[0].eyeDir.z;

	vec3 normal = vs_out[0].normalW;

	float horizontal = def.tile_num_x + -(atan(dirXY.y, dirXY.x) / tau + 0.5) * def.tile_num_x;
	float horizontal_tile = round(horizontal);

	float vertical = max(0, (asin(dirZ) / (pi / 2.0)) * (def.tile_num_y - 1.0));
	float vertical_tile = round(vertical);

	// Sprite space
	vec3 center = vs_out[0].positionW.xyz;
	vec3 forward = normalize(center - eye);
	vec3 right = normalize(cross(forward, vec3(0, 0, 1)));
	vec3 up = cross(right, forward);

	center += def.model_offset;

	// -------------------------------------------------------------------------------------------------

	fs_in.tile_index = vec2(horizontal_tile, vertical_tile);
//	fs_in.dither0 = fract(angleZ / 3.14 * 2.0 * 8.0);
//	fs_in.dither1 = fract(angleXY / 3.14 / 2.0 * 16.0);
	fs_in.type = vs_out[0].type;

	// v1
	fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(1, 0);
	gl_Position = matMVP * vec4(center + right * 0.5 * def.model_scale - up * 0.5 * def.model_scale, 1);
	EmitVertex();

	// v2
	fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(1, 1);
	gl_Position = matMVP * vec4(center + right * 0.5 * def.model_scale + up * 0.5 * def.model_scale, 1);
	EmitVertex();

	// v0
	fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(0, 0);
	gl_Position = matMVP * vec4(center - right * 0.5 * def.model_scale - up * 0.5 * def.model_scale, 1);
	EmitVertex();

	// v3
	fs_in.color = vec4(1, 0, 0, 1);
	fs_in.uv = vec2(0, 1);
	gl_Position = matMVP * vec4(center - right * 0.5 * def.model_scale + up * 0.5 * def.model_scale, 1);
	EmitVertex();

	EndPrimitive();

	// -------------------------------------------------------------------------------------------------

//	vec2 pixel_size = 2.0 / render_resolution;
//
//	vec4 source_P = gl_in[0].gl_Position;
//	vec4 target_P = gl_in[1].gl_Position;
//
//	vec4 source_SP = vec4(source_P.xyz / source_P.w, source_P.w);
//	vec4 target_SP = vec4(target_P.xyz / target_P.w, target_P.w);
//
//	// Determine the screen direction
//	// Normalize the direction vector to be based on xy pixel units
//	vec4 dir_SP = target_SP - source_SP;
//	float dir_pixel_length = length(dir_SP.xy / pixel_size);
//	dir_SP /= dir_pixel_length;
//
//	// If only one endpoint is behind the camera the negative w coord would flip the direction
//	if (target_P.w * source_P.w < 0)
//		dir_SP *= -1;
//
//	// Determine the screen normal from direction (pixel size changes are required)
//	vec4 normal_SP = vec4(dir_SP.y / pixel_size.y * pixel_size.x, -dir_SP.x / pixel_size.x * pixel_size.y, 0, 0);
//
//
//	vec4 v0 = target_SP;
//	vec4 v1 = target_SP + normal_SP * +head_width * 0.5 + dir_SP * -head_height;
//	vec4 v2 = target_SP + normal_SP * +body_width * 0.5 + dir_SP * -head_height;
//	vec4 v3 = source_SP + normal_SP * +body_width * 0.5;
//	vec4 v4 = source_SP + normal_SP * -body_width * 0.5;
//	vec4 v5 = target_SP + normal_SP * -body_width * 0.5 + dir_SP * -head_height;
//	vec4 v6 = target_SP + normal_SP * -head_width * 0.5 + dir_SP * -head_height;
//
////	if (dir_pixel_length < 1)
//		// If the arrow is less than 1 pixel long
//	if (dir_pixel_length < head_height)
//		// If the arrow is shorter than the head use target's depth to avoid yeeting verticies out of the clip space
//		v1.zw = v2.zw = v3.zw = v4.zw = v5.zw = v6.zw = v0.zw;
//
//	v0.xyz *= v0.w;
//	v1.xyz *= v1.w;
//	v2.xyz *= v2.w;
//	v3.xyz *= v3.w;
//	v4.xyz *= v4.w;
//	v5.xyz *= v5.w;
//	v6.xyz *= v6.w;
//
//	float neck_ratio = head_height / dir_pixel_length;
//	float neck_v = max(1-neck_ratio, 0);
//	float neck_u_left = body_width / head_width;
//	float neck_u_right = 1 - neck_u_left;
//	float neck_segmentPosition = neck_v * vs_out[0].segmentSize;
//	float neck_chainPosition = vs_out[0].chainPosition + neck_v * vs_out[0].segmentSize;
//
//	fs_in.segmentSize = vs_out[0].segmentSize;
//	fs_in.chainSize = vs_out[0].chainSize;
//	fs_in.animationOffset = vs_out[0].animationOffset;
//
//	vec4 neck_color = mix(vs_out[0].color, vs_out[1].color, neck_v);
//
//	// Head
//	fs_in.head = 1;
//	// v6
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(0, neck_v);
//	fs_in.part_uv = vec2(0, 0);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v6; EmitVertex();
//	// v5
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(neck_u_left, neck_v);
//	fs_in.part_uv = vec2(neck_u_left, 0);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v5; EmitVertex();
//	// v0
//	fs_in.color = vs_out[1].color;
//	fs_in.uv = vec2(0.5, 1);
//	fs_in.part_uv = vec2(0.5, 1);
//	fs_in.segmentPosition = vs_out[1].segmentPosition;
//	fs_in.chainPosition = vs_out[1].chainPosition;
//	gl_Position = v0; EmitVertex();
//	// v2
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(neck_u_right, neck_v);
//	fs_in.part_uv = vec2(neck_u_right, 0);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v2; EmitVertex();
//	// v1
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(1, neck_v);
//	fs_in.part_uv = vec2(1, 0);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v1; EmitVertex();
//	EndPrimitive();
//
//	// Body
//	fs_in.head = 0;
//	// v4
//	fs_in.color = vs_out[0].color;
//	fs_in.uv = vec2(neck_u_left, 0);
//	fs_in.part_uv = vec2(0, 0);
//	fs_in.segmentPosition = vs_out[0].segmentPosition;
//	fs_in.chainPosition = vs_out[0].chainPosition;
//	gl_Position = v4; EmitVertex();
//	// v3
//	fs_in.color = vs_out[0].color;
//	fs_in.uv = vec2(neck_u_right, 0);
//	fs_in.part_uv = vec2(1, 0);
//	fs_in.segmentPosition = vs_out[0].segmentPosition;
//	fs_in.chainPosition = vs_out[0].chainPosition;
//	gl_Position = v3; EmitVertex();
//	// v5
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(neck_u_left, neck_v);
//	fs_in.part_uv = vec2(0, 1);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v5; EmitVertex();
//	// v2
//	fs_in.color = neck_color;
//	fs_in.uv = vec2(neck_u_right, neck_v);
//	fs_in.part_uv = vec2(1, 1);
//	fs_in.segmentPosition = neck_segmentPosition;
//	fs_in.chainPosition = neck_chainPosition;
//	gl_Position = v2; EmitVertex();
//	EndPrimitive();
}
