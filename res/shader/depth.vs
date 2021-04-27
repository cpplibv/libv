#version 330
#extension GL_ARB_separate_shader_objects : enable

uniform mat4 matMVP;

layout(location = 0) in vec3 vertexPos;

void main(){
	gl_Position = matMVP * vec4(vertexPos,1);
}
