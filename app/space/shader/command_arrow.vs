#version 330 core

#include <command_arrow.glsl>
#include <block/matrices.glsl>

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;
layout(location = 14) in vec4 vertex_AO_NU_NU_NU; // AnimationOffset, NotUsed, NotUsed, NotUsed
layout(location = 15) in vec4 vertex_SP_SS_CP_CS; // SegmentPosition, SegmentSize, ChainPosition, ChainSize

out GeometryData vs_out;

void main() {
	float vertexSegmentPosition = vertex_SP_SS_CP_CS.x;
	float vertexSegmentSize = vertex_SP_SS_CP_CS.y;
	float vertexChainPosition = vertex_SP_SS_CP_CS.z;
	float vertexChainSize = vertex_SP_SS_CP_CS.w;
	float vertexAnimationOffset = vertex_AO_NU_NU_NU.x;

	gl_Position = matMVP * vec4(vertexPosition, 1);

	vs_out.color = vertexColor;
	vs_out.segmentPosition = vertexSegmentPosition;
	vs_out.segmentSize = vertexSegmentSize;
	vs_out.chainPosition = vertexChainPosition;
	vs_out.chainSize = vertexChainSize;
	vs_out.animationOffset = vertexAnimationOffset;
}
