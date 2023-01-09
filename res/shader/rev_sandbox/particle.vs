#version 330 core

#include <block/matrices200.glsl>
#include <block/camera200.glsl>

// -------------------------------------------------------------------------------------------------

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor0;
layout(location = 10) in vec3 vertexXYZ;
layout(location = 11) in vec2 packSizeRotation;

// -------------------------------------------------------------------------------------------------

out vec2 fragmentTexture0;
out vec4 fragmentColor0;

// -------------------------------------------------------------------------------------------------

void main() {
	float vertexSize = packSizeRotation.x;
	float vertexRotation = packSizeRotation.y;

	fragmentTexture0 = vertexPosition.xy + 0.5;
	fragmentColor0 = vertexColor0;
//	fragmentColor0.xyz *= 30;
//	fragmentColor0 = vertexColor0 * (vec4(vec3(1), 1));
//	fragmentColor0 += vec4(abs(vertexXYZ.z), 0, 0, 1);
//	fragmentColor0 = vec4(vertexColor0.rg, 1, 1);
//	fragmentColor0 = vec4(0, 0, 0, 1);

	float vertexRotatedPositionX = vertexPosition.x * cos(vertexRotation) - vertexPosition.y * sin(vertexRotation);
	float vertexRotatedPositionY = vertexPosition.x * sin(vertexRotation) + vertexPosition.y * cos(vertexRotation);

	vec3 vertexOffset =
			+ cameraRightW * vertexRotatedPositionX * vertexSize
			+ cameraUpW * vertexRotatedPositionY * vertexSize;

	vec3 position = vertexXYZ + vertexOffset;

	gl_Position = matMVP * vec4(position, 1);
}
