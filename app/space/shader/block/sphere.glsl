#pragma once

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};
