#version 330 core

uniform sampler2D textureSampler;
uniform vec4 color;

in vec2 fragmentUV;

out vec4 outputColor;

void main() {
	outputColor = vec4(color.r, color.g, color.b, color.a * texture(textureSampler, fragmentUV).r);
	//color = vec4(1, 1, 1, texture(textureSampler, fragmentUV).r);
	//color = vec4(1, 1, 1, texture(textureSampler, fragmentUV).r > 0.5f); //<<< TEST ALPHA TEST!!!!
	//color = vec4(1, 1, 1, texture(textureSampler, fragmentUV).r > 0.5f); //<<< TEST ALPHA TEST!!!!
	// and only if magnified?
	//http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
	//color = vec4(1, 0, 0, 1);
}
