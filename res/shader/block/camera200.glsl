#pragma once

layout(std140) uniform Camera200 {
	mat4 matP;
	mat4 matV;
	vec3 eye;
	vec3 cameraForwardW;
	vec3 cameraRightW;
	vec3 cameraUpW;
};
