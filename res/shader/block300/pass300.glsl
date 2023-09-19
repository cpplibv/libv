//

#pragma once

#ifndef BUILTIN_BLOCK_IDENTIFIER_PASS
#define BUILTIN_BLOCK_IDENTIFIER_PASS
#endif


layout(std140) uniform Pass300 {
	mat4 matP;                /// Current projection matrix
//	mat4 matPinv;             /// Inverse of the current projection matrix
	mat4 matV;                /// Current view matrix
	mat4 matVP;               /// Current view * projection matrix
	mat4 matVPinv;            /// Inverse of the current view * projection matrix

	vec3 eyeW;                /// World space position of the camera
	float timeSimLooping;     /// Simulation time in seconds looping between -1800 and 1800 starting from 0

	vec3 cameraForwardW;
	float timeSimUncapped;    /// Simulation time in seconds

	vec3 cameraRightW;
	float timeRealLooping;    /// Real time in seconds looping between -1800 and 1800 starting from 0

	vec3 cameraUpW;
	float timeRealUncapped;   /// Real time in seconds

	vec2 canvasSize;          /// Render resolution
	vec2 pixelSize;           /// Pixel size in NDC (Normalized Device Coordinates): 1.0 / canvasSize

	vec2 mousePosition;       /// Mouse position relative to the canvas (can be outside canvas)
    float cameraNear;
    float cameraFar;
} BUILTIN_BLOCK_IDENTIFIER_PASS;
