#version 330 core

#include <command_arrow.glsl>
#include <block/sphere.glsl>

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;
layout(location = 15) in vec4 vertex_SP_SS_TP_TS;// SegmentPosition, SegmentSize, TotalPosition, TotalSize

out GeometryData geometry;
//out GeometryData VS_OUTPUT(geometry, fragment);

void main() {
	float vertexSegmentPosition = vertex_SP_SS_TP_TS.x;
	float vertexSegmentSize = vertex_SP_SS_TP_TS.y;
	float vertexTotalPosition = vertex_SP_SS_TP_TS.z;
	float vertexTotalSize = vertex_SP_SS_TP_TS.w;

	gl_Position = matMVP * vec4(vertexPosition, 1);
	geometry.normal = normalize(vertexNormal);
	geometry.texture0 = vertexTexture0;

	geometry.segmentPosition = vertexSegmentPosition;
	geometry.segmentSize = vertexSegmentSize;
	geometry.totalPosition = vertexTotalPosition;
	geometry.totalSize = vertexTotalSize;
}
