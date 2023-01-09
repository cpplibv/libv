#pragma once

layout(std140) uniform Camera {
	mat4 camera_matP; // Rename to matP
	mat4 matV;
	vec3 camera_eye; // Rename to eye
	vec3 cameraForwardW;
	vec3 cameraRightW;
	vec3 cameraUpW;
};
