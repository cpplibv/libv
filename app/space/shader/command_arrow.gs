#version 330 core

#include <command_arrow.glsl>

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in GeometryData geometry[3];

out FragmentData fragment;

void main() {
	//       Input Line     Output Vertices    Output Triangles
	//   0=Target 1=Source
	//
	//           0                 0                  +         -+
	//          .|.               / \                /|\         |
	//         . | .             /   \              / | \        | Triangle 1, 2, 3
	//        .  |  .           /     \            /1/ \3\       | as Strip 0
	//       .   |   .         /       \          / / 2 \ \      |
	//      .... | ....       6--5   2--1        +--+---+--+    -+
	//         . | .             |   |              |4 /|        |
	//         . | .             |   |              | / |        | Triangle 4, 5
	//         . | .             |   |              |/ 5|        | as Strip 1
	//         ..1..             4---3              +---+       -+
	//

	for (int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		fragment.normal = geometry[i].normal;
		fragment.texture0 = geometry[i].texture0;
		fragment.segmentPosition = geometry[i].segmentPosition;
		fragment.segmentSize = geometry[i].segmentSize;
		fragment.totalPosition = geometry[i].totalPosition;
		fragment.totalSize = geometry[i].totalSize;

		EmitVertex();
	}
	EndPrimitive();
}

// =================================================================================================

//layout (lines) in;
//layout (triangle_strip, max_vertices = 5*3) out;
//
//in vec4 geometryColor[2];
//
//out vec4 fragmentColor;
//
////#include <block/sphere.glsl>
//
////TODO P1: If line thickness is less then 1 px reduce alpha instead
//
//void main() {
//	gl_Position = gl_in[0].gl_Position;
//	fragmentColor = geometryColor[0];
//
//	EmitVertex();
//
//	gl_Position = gl_in[1].gl_Position;
//	fragmentColor = geometryColor[1];
//
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//
//	EndPrimitive();
//
//	EmitVertex();
//	EmitVertex();
//	EmitVertex();
//}

// -------------------------------------------------------------------------------------------------

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
