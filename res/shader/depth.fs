#version 330
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out float fragmentdepth;

void main()
{
	fragmentdepth = gl_FragCoord.z;
}