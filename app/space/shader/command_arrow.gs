#version 330 core

#include <command_arrow.glsl>
//#include <lib/linearize_depth.glsl>

layout (lines) in;
layout (triangle_strip, max_vertices = 9) out;

in GeometryData vs_out[2];

out FragmentData fs_in;

uniform vec2 render_resolution; // Program block


// -------------------------------------------------------------------------------------------------

void main() {
	//       Input Line     Output Vertices    Output Triangles
	//   0=Source 1=Target
	//
	//           1                 0                  +         -+
	//          .|.               / \                /|\         |
	//         . | .             /   \              / | \        | Triangle 1, 2, 3
	//        .  |  .           /     \            /1/ \3\       | as Strip 0
	//       .   |   .         /       \          / / 2 \ \      |
	//      .... | ....       6--5   2--1        +--+---+--+    -+
	//         . | .             |   |              |4 /|        |
	//         . | .             |   |              | / |        | Triangle 4, 5
	//         . | .             |   |              |/ 5|        | as Strip 1
	//         ..0..             4---3              +---+       -+
	//

	vec2 pixel_size = 2.0 / render_resolution;

	vec4 source_P = gl_in[0].gl_Position;
	vec4 target_P = gl_in[1].gl_Position;

	vec4 source_SP = vec4(source_P.xyz / source_P.w, source_P.w);
	vec4 target_SP = vec4(target_P.xyz / target_P.w, target_P.w);

	// Determine the screen direction
	// Normalize the direction vector to be based on xy pixel units
	vec4 dir_SP = target_SP - source_SP;
	float dir_pixel_length = length(dir_SP.xy / pixel_size);
	dir_SP /= dir_pixel_length;

	// TODO P1: Vertexes yeeted behind the camera normal plane causes incorrect uvs

	// If only one endpoint is behind the camera the negative w coord would flip the direction
	if (target_P.w * source_P.w < 0)
		dir_SP *= -1;

	// Determine the screen normal from direction (pixel size changes are required)
	vec4 normal_SP = vec4(dir_SP.y / pixel_size.y * pixel_size.x, -dir_SP.x / pixel_size.x * pixel_size.y, 0, 0);


	vec4 v0 = target_SP;
	vec4 v1 = target_SP + normal_SP * +head_width * 0.5 + dir_SP * -head_height;
	vec4 v2 = target_SP + normal_SP * +body_width * 0.5 + dir_SP * -head_height;
	vec4 v3 = source_SP + normal_SP * +body_width * 0.5;
	vec4 v4 = source_SP + normal_SP * -body_width * 0.5;
	vec4 v5 = target_SP + normal_SP * -body_width * 0.5 + dir_SP * -head_height;
	vec4 v6 = target_SP + normal_SP * -head_width * 0.5 + dir_SP * -head_height;

//	if (dir_pixel_length < 1)
		// If the arrow is less than 1 pixel long
	if (dir_pixel_length < head_height)
		// If the arrow is shorter than the head use target's depth to avoid yeeting verticies out of the clip space
		v1.zw = v2.zw = v3.zw = v4.zw = v5.zw = v6.zw = v0.zw;

	v0.xyz *= v0.w;
	v1.xyz *= v1.w;
	v2.xyz *= v2.w;
	v3.xyz *= v3.w;
	v4.xyz *= v4.w;
	v5.xyz *= v5.w;
	v6.xyz *= v6.w;

	float neck_ratio = head_height / dir_pixel_length;
	float neck_v = max(1-neck_ratio, 0);
	float neck_u_left = body_width / head_width;
	float neck_u_right = 1 - neck_u_left;
	float neck_segmentPosition = neck_v * vs_out[0].segmentSize;
	float neck_chainPosition = vs_out[0].chainPosition + neck_v * vs_out[0].segmentSize;

	fs_in.segmentSize = vs_out[0].segmentSize;
	fs_in.chainSize = vs_out[0].chainSize;
	fs_in.animationOffset = vs_out[0].animationOffset;

	vec4 neck_color = mix(vs_out[0].color, vs_out[1].color, neck_v);

	// Head
	fs_in.head = 1;
	// v6
	fs_in.color = neck_color;
	fs_in.uv = vec2(0, neck_v);
	fs_in.part_uv = vec2(0, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v6; EmitVertex();
	// v5
	fs_in.color = neck_color;
	fs_in.uv = vec2(neck_u_left, neck_v);
	fs_in.part_uv = vec2(neck_u_left, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v5; EmitVertex();
	// v0
	fs_in.color = vs_out[1].color;
	fs_in.uv = vec2(0.5, 1);
	fs_in.part_uv = vec2(0.5, 1);
	fs_in.segmentPosition = vs_out[1].segmentPosition;
	fs_in.chainPosition = vs_out[1].chainPosition;
	gl_Position = v0; EmitVertex();
	// v2
	fs_in.color = neck_color;
	fs_in.uv = vec2(neck_u_right, neck_v);
	fs_in.part_uv = vec2(neck_u_right, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v2; EmitVertex();
	// v1
	fs_in.color = neck_color;
	fs_in.uv = vec2(1, neck_v);
	fs_in.part_uv = vec2(1, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v1; EmitVertex();
	EndPrimitive();

	// Body
	fs_in.head = 0;
	// v4
	fs_in.color = vs_out[0].color;
	fs_in.uv = vec2(neck_u_left, 0);
	fs_in.part_uv = vec2(0, 0);
	fs_in.segmentPosition = vs_out[0].segmentPosition;
	fs_in.chainPosition = vs_out[0].chainPosition;
	gl_Position = v4; EmitVertex();
	// v3
	fs_in.color = vs_out[0].color;
	fs_in.uv = vec2(neck_u_right, 0);
	fs_in.part_uv = vec2(1, 0);
	fs_in.segmentPosition = vs_out[0].segmentPosition;
	fs_in.chainPosition = vs_out[0].chainPosition;
	gl_Position = v3; EmitVertex();
	// v5
	fs_in.color = neck_color;
	fs_in.uv = vec2(neck_u_left, neck_v);
	fs_in.part_uv = vec2(0, 1);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v5; EmitVertex();
	// v2
	fs_in.color = neck_color;
	fs_in.uv = vec2(neck_u_right, neck_v);
	fs_in.part_uv = vec2(1, 1);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.chainPosition = neck_chainPosition;
	gl_Position = v2; EmitVertex();
	EndPrimitive();

//	// Arrow Line
//	fs_in.color = vec4(1, 1, 1, 1);
//	gl_Position = v0; EmitVertex();
//	gl_Position = v1; EmitVertex();
//	gl_Position = v2; EmitVertex();
//	gl_Position = v3; EmitVertex();
//	gl_Position = v4; EmitVertex();
//	gl_Position = v5; EmitVertex();
//	gl_Position = v6; EmitVertex();
//	gl_Position = v0; EmitVertex();
//	EndPrimitive();
//
//	// Core line
//	fs_in.color = vec4(1, 0, 1, 0.3);
//	gl_Position = source_P; EmitVertex();
//	gl_Position = target_P; EmitVertex();
//	EndPrimitive();
}
