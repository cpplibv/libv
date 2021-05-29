#version 330 core

#include <command_arrow.glsl>
#include <block/matrices.glsl>

layout(location = 0) in vec3 vertexPosition;
layout(location = 15) in vec4 vertex_SP_SS_TP_TS; // SegmentPosition, SegmentSize, TotalPosition, TotalSize

out GeometryData vs_out;

void main() {
	float vertexSegmentPosition = vertex_SP_SS_TP_TS.x;
	float vertexSegmentSize = vertex_SP_SS_TP_TS.y;
	float vertexTotalPosition = vertex_SP_SS_TP_TS.z;
	float vertexTotalSize = vertex_SP_SS_TP_TS.w;

	gl_Position = matMVP * vec4(vertexPosition, 1);

	vs_out.segmentPosition = vertexSegmentPosition;
	vs_out.segmentSize = vertexSegmentSize;
	vs_out.totalPosition = vertexTotalPosition;
	vs_out.totalSize = vertexTotalSize;
}
