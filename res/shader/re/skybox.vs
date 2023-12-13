#version 330 core

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;

out vec3 fragmentDirectionUnW;

// -------------------------------------------------------------------------------------------------

void main() {
	// Discard translation from matV with mat4(mat3(matV))
	vec4 pos = matP * mat4(mat3(matV)) * vec4(vertexPosition, 1.0);
//#ifdef RE_REVERSE_Z
    gl_Position = vec4(pos.xy, 0, pos.w); // Set depth to 0.0 for ReverseZ
//#else
//    gl_Position = pos.xyww; // Set depth to 1.0 with xyww
//#endif
    fragmentDirectionUnW = vertexPosition;
}
