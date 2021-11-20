#version 330 core

//uniform sampler2D textureSampler;
//uniform vec4 color;
//
//in vec2 fragmentUV;
//
//out vec4 outputColor;
//
//void main() {
//	outputColor = vec4(color.r, color.g, color.b, color.a * texture(textureSampler, fragmentUV).r);
//
//	//color = vec4(1, 1, 1, texture(textureSampler, fragmentUV).r);
//	//color = vec4(1, 1, 1, texture(textureSampler, fragmentUV).r > 0.5f); //<<< TEST ALPHA TEST!!!!
//	// and only if magnified?
//	//http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
//	//color = vec4(1, 0, 0, 1);
//}



// =================================================================================================




layout(std140) uniform Matrices {
	mat4 matMVP;
	mat4 matP;
	mat4 matM;
	vec3 eye;
};

//layout(std140) uniform UIInfo {
//	mat4 matMVP;
//	mat4 matM;
//	vec2 clip_pos;
//	vec2 clip_size;
//	vec2 component_pos;
//	vec2 component_size;
//	vec2 mouse_position;
//	vec2 ui_size;
//	float time;
//};



//in vec4 debug_clip_var;
//
//void clip_component_fs(inout vec4 result) {
//	if (debug_clip && debug_clip_var.x < 0 || debug_clip_var.y < 0 || debug_clip_var.z < 0 || debug_clip_var.w < 0)
//	result = result * vec4(1.5, 0.3, 0.3, 0.6);
//}
//
//void clip_component_fs(inout vec4 result, inout vec4 mask) {
//	if (debug_clip && debug_clip_var.x < 0 || debug_clip_var.y < 0 || debug_clip_var.z < 0 || debug_clip_var.w < 0) {
//		result = mix(vec4(0.8, 0.2, 0.2, 0.0), vec4(1.0, 0.5, 0.5, 1.0), result);
//		mask = mask * 0.6;
//	}
//}



in vec4 fragmentColor0;
in vec2 fragmentTexture0;

layout(location = 0, index = 0) out vec4 outputColor;
layout(location = 0, index = 1) out vec4 outputMask;

uniform sampler2D texture_font;

void main() {
	ivec2 size = textureSize(texture_font, 0);
	vec3 sample0 = texture(texture_font, fragmentTexture0 / size).xyz;
	outputColor = fragmentColor0;
	outputMask = vec4(sample0, 1);

//	clip_component_fs(outputColor, outputMask);
}
