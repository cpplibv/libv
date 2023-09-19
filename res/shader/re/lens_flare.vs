#version 450 core

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>
#include <shader/re/lens_flare.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;
layout(location = VERTEX_ATTR_DIFFUSE) in vec4 vertexColor;
layout(location = VERTEX_ATTR_CUSTOM1) in vec4 vertexCustom1;
layout(location = VERTEX_ATTR_CUSTOM0) in vec4 vertexCustom0;

flat out vec4 fragmentColor;
flat out float fragmentColorLightAttenuation;
out vec2 fragmentUV;

// -------------------------------------------------------------------------------------------------

vec3 div_w(vec4 vec) {
	return vec.xyz / vec.w;
}

void main() {
	float vertexLinePosition = vertexCustom1.x;
	float vertexRotation = vertexCustom1.y;
	float vertexRotateToCenter = vertexCustom1.z;
	float vertexColorLightAttenuation = vertexCustom1.w;
	float vertexSize = vertexCustom0.x;
	float vertexSizeRadialEdge = vertexCustom0.y;
//	float vertex__ = vertexCustom0.z;
//	float vertex__ = vertexCustom0.w;

	// Caculate position
	vec2 positionTargetS = div_w(matVP * targetPositionW).xy;
	vec2 positionCenterS = vec2(0);
	vec2 positionFlareS = mix(positionTargetS, positionCenterS, vertexLinePosition);

	// Calculate vertexOffset:
	vec2 vertexOffset = vertexPosition.xy;

	// Rotate
	float rotationSin = sin(vertexRotation);
	float rotationCos = cos(vertexRotation);
	mat2 matRotation = mat2(rotationCos, rotationSin, -rotationSin, rotationCos);
	vertexOffset = matRotation * vertexOffset;

	// Rotate to center
	if (vertexRotateToCenter != 0.0) {
		vec2 directionToCenterS = normalize(positionCenterS - positionFlareS);
		mat2 matRotateToCenter = mat2(directionToCenterS.x, directionToCenterS.y, -directionToCenterS.y, directionToCenterS.x);
		vertexOffset = matRotateToCenter * vertexOffset;
	}

	// Size
	float size;
	if (vertexSizeRadialEdge < 0.0) {
		size = vertexSize;
	} else {
		float cornerDistanceFromCenterSR = length(canvasSize);
		float targetDistanceFromCenterSR = length((positionTargetS - positionCenterS) * canvasSize);
		size = mix(vertexSize, vertexSizeRadialEdge, targetDistanceFromCenterSR / cornerDistanceFromCenterSR);
	}
	vertexOffset *= size * pixelSize * 2.0;

	// Fragment
	gl_Position = vec4(positionFlareS + vertexOffset, 0, 1);
	fragmentColor = vertexColor;
	fragmentColorLightAttenuation = vertexColorLightAttenuation;
	fragmentUV = vertexUV;
}
