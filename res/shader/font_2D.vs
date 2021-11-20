#version 330 core

//layout(location = 0) in vec3 vertexPos;
//layout(location = 8) in vec2 vertexUV;
//
//uniform mat4 matMVP;
//
//out vec2 fragmentUV;
//
//void main() {
//	gl_Position = matMVP * vec4(vertexPos, 1);
//	fragmentUV = vertexUV;
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



//out vec4 debug_clip_var;
//
//void clip_component_vs(mat4 matM, vec3 vertexPosition, vec2 clip_pos, vec2 clip_size) {
//	vec4 position = vec4(vertexPosition, 1);
//
//	if (debug_clip) {
//		debug_clip_var.x = position.x - clip_pos.x;
//		debug_clip_var.y = position.y - clip_pos.y;
//		debug_clip_var.z = clip_size.x + clip_pos.x - position.x;
//		debug_clip_var.w = clip_size.y + clip_pos.y - position.y;
//
//		gl_ClipDistance[0] = 1;
//		gl_ClipDistance[1] = 1;
//		gl_ClipDistance[2] = 1;
//		gl_ClipDistance[3] = 1;
//
//	} else {
//		gl_ClipDistance[0] = position.x - clip_pos.x;
//		gl_ClipDistance[1] = position.y - clip_pos.y;
//		gl_ClipDistance[2] = clip_size.x + clip_pos.x - position.x;
//		gl_ClipDistance[3] = clip_size.y + clip_pos.y - position.y;
//	}
//}



layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor0;
layout(location = 8) in vec2 vertexTexture0;

layout(location = 15) in vec4 vertexPositionOffset;

out vec4 fragmentColor0;
out vec2 fragmentTexture0;

void main() {
	gl_Position = matMVP * vec4(vertexPositionOffset.xyz + vertexPosition, 1);
	fragmentColor0 = vertexColor0;
	fragmentTexture0 = vertexTexture0;

//	clip_component_vs(matM, vertexPosition, clip_pos, clip_size);
}
