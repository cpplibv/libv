#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;
layout(location = 15) in vec4 vertex_SP_SS_TP_TS;// SegmentPosition, SegmentSize, TotalPosition, TotalSize

out vec3 fragmentNormal;
out vec2 fragmentTexture0;
out float fragmentSegmentPosition;
out float fragmentSegmentSize;
out float fragmentTotalPosition;
out float fragmentTotalSize;

#include <block/sphere.glsl>

void main() {
	float vertexSegmentPosition = vertex_SP_SS_TP_TS.x;
	float vertexSegmentSize = vertex_SP_SS_TP_TS.y;
	float vertexTotalPosition = vertex_SP_SS_TP_TS.z;
	float vertexTotalSize = vertex_SP_SS_TP_TS.w;

	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentNormal = normalize(vertexNormal);
	fragmentTexture0 = vertexTexture0;

	fragmentSegmentPosition = vertexSegmentPosition;
	fragmentSegmentSize = vertexSegmentSize;
	fragmentTotalPosition = vertexTotalPosition;
	fragmentTotalSize = vertexTotalSize;
}
