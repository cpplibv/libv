#version 330 core

#include <command_arrow.glsl>
#include <lib/linearize_depth.glsl>

layout (lines) in;
layout (triangle_strip, max_vertices = 9) out;

in GeometryData vs_out[2];

out FragmentData fs_in;

uniform vec2 render_resolution;

const float body_width = 7;
const float head_width = 16;
const float head_height = 28;
//const float body_width = 15;
//const float head_width = 50;
//const float head_height = 50;


// -------------------------------------------------------------------------------------------------

void main() {
	//       Input Line     Output Vertices    Output Triangles
	//   0=Target 1=Source
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

	// TODO P1: If dir_pixel_length is very small the neck coordinates are catapulted out off clip space

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
	float neck_totalPosition = vs_out[0].totalPosition + neck_v * vs_out[0].segmentSize;

	fs_in.segmentSize = vs_out[0].segmentSize;
	fs_in.totalSize = vs_out[0].totalSize;

	// Head
	fs_in.head = 1;
	// v6
	fs_in.uv = vec2(0, neck_v);
	fs_in.part_uv = vec2(0, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
	gl_Position = v6; EmitVertex();
	// v5
	fs_in.uv = vec2(neck_u_left, neck_v);
	fs_in.part_uv = vec2(neck_u_left, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
	gl_Position = v5; EmitVertex();
	// v0
	fs_in.uv = vec2(0.5, 1);
	fs_in.part_uv = vec2(0.5, 1);
	fs_in.segmentPosition = vs_out[0].segmentPosition;
	fs_in.totalPosition = vs_out[0].totalPosition + vs_out[0].segmentSize;
	gl_Position = v0; EmitVertex();
	// v2
	fs_in.uv = vec2(neck_u_right, neck_v);
	fs_in.part_uv = vec2(neck_u_right, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
	gl_Position = v2; EmitVertex();
	// v1
	fs_in.uv = vec2(1, neck_v);
	fs_in.part_uv = vec2(1, 0);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
	gl_Position = v1; EmitVertex();
	EndPrimitive();

	// Body
	fs_in.head = 0;
	// v4
	fs_in.uv = vec2(neck_u_left, 0);
	fs_in.part_uv = vec2(0, 0);
	fs_in.segmentPosition = vs_out[0].segmentPosition;
	fs_in.totalPosition = vs_out[0].totalPosition;
	gl_Position = v4; EmitVertex();
	// v3
	fs_in.uv = vec2(neck_u_right, 0);
	fs_in.part_uv = vec2(1, 0);
	fs_in.segmentPosition = vs_out[0].segmentPosition;
	fs_in.totalPosition = vs_out[0].totalPosition;
	gl_Position = v3; EmitVertex();
	// v5
	fs_in.uv = vec2(neck_u_left, neck_v);
	fs_in.part_uv = vec2(0, 1);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
	gl_Position = v5; EmitVertex();
	// v2
	fs_in.uv = vec2(neck_u_right, neck_v);
	fs_in.part_uv = vec2(1, 1);
	fs_in.segmentPosition = neck_segmentPosition;
	fs_in.totalPosition = neck_totalPosition;
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

//	vec4 source_P = gl_in[0].gl_Position;
//	vec4 target_P = gl_in[1].gl_Position;
//	vec4 source_S = source_P / source_P.w;
//	vec4 target_S = target_P / target_P.w;
//
//	vec2 diff = target_S.xy - source_S.xy;
//	vec2 dir = normalize(vec2(diff.x / aspect, diff.y));
//	vec2 dir = normalize(diff / pixel_size);
//	vec2 dir = normalize((target_S.xy - source_S.xy) / pixel_size);
//	if (target_P.w * source_P.w < 0) // If only one endpoint is behind the camera the negative w coord would flip the direction
//		dir *= -1;
//
//	vec2 normal = vec2(dir.y, -dir.x);

// =================================================================================================

//[maxvertexcount(4)]
//void geo(line v2g v[2], inout TriangleStream<g2f> ts)
//{
//    g2f g[4];
//
//    float4 p1 = mul(UNITY_MATRIX_MVP, v[0].vertex);
//    float4 p2 = mul(UNITY_MATRIX_MVP, v[1].vertex);
//
//    float2 dir = normalize(p2.xy - p1.xy);
//    float2 normal = float2(-dir.y, dir.x);
//
//    float4 offset1 = float4(normal * p1.w * _Thickness / 2.0f, 0, 0);
//    float4 offset2 = float4(normal * p2.w * _Thickness / 2.0f, 0, 0);
//
//    float4 o1 = p1 + offset1;
//    float4 o2 = p1 - offset1;
//    float4 o3 = p2 + offset2;
//    float4 o4 = p2 - offset2;
//
//    g[0].pos = o1;
//    g[1].pos = o2;
//    g[2].pos = o3;
//    g[3].pos = o4;
//
//    ts.Append(g[0]);
//    ts.Append(g[1]);
//    ts.Append(g[2]);
//    ts.Append(g[3]);
//}
//
// Fix #1
//float2 dir = normalize(p2.xy /p2.w - p1.xy /p1.w);
//
// Fix #2
//offset.x *= _ScreenParams.y/_ScreenParams.x;
//
