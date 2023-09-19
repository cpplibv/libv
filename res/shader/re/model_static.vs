#version 450 core
//#extension GL_ARB_shader_draw_parameters : enable

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;
layout(location = VERTEX_ATTR_NORMAL) in vec3 vertexNormal;
layout(location = VERTEX_ATTR_TANGENT) in vec3 vertexTangent;
//layout(location = VERTEX_ATTR_BONE_INDICES) in ivec4 vertexBoneIndices;
//layout(location = VERTEX_ATTR_BOND_WEIGHT) in vec4 vertexBondWeight;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW0) in vec4 vertexInstanceMatRow0;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW1) in vec4 vertexInstanceMatRow1;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW2) in vec4 vertexInstanceMatRow2;

invariant gl_Position; // invariant - Enforce consitent depth value calculations

out vec3 fragmentPositionW;
out vec2 fragmentUV;
out vec3 fragmentNormalUnW;
out vec3 fragmentTangentUnW;
out vec3 fragmentBitangentUnW;

// -------------------------------------------------------------------------------------------------

void main() {
	//	gl_DrawIDARB
	//	gl_BaseInstanceARB
	//	gl_InstanceID

	mat4 matSubM = mat4(
		vec4(vertexInstanceMatRow0.x, vertexInstanceMatRow1.x, vertexInstanceMatRow2.x, 0.0),
		vec4(vertexInstanceMatRow0.y, vertexInstanceMatRow1.y, vertexInstanceMatRow2.y, 0.0),
		vec4(vertexInstanceMatRow0.z, vertexInstanceMatRow1.z, vertexInstanceMatRow2.z, 0.0),
		vec4(vertexInstanceMatRow0.w, vertexInstanceMatRow1.w, vertexInstanceMatRow2.w, 1.0)
	);

	fragmentPositionW = vec3(matM * matSubM * vec4(vertexPosition, 1));
	fragmentUV = vertexUV;
	fragmentNormalUnW = mat3(matM) * mat3(matSubM) * vertexNormal;
	fragmentTangentUnW = mat3(matM) * mat3(matSubM) * vertexTangent;
	fragmentBitangentUnW = cross(fragmentNormalUnW, fragmentTangentUnW);

	gl_Position = matP * matMV * matSubM * vec4(vertexPosition, 1);
}
