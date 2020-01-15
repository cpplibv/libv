#version 330 core

#include <app/vm4_viewer/res/common/depth.glsl>
#include <app/vm4_viewer/res/common/light.glsl>
#include <app/vm4_viewer/res/common/material.glsl>
#include <app/vm4_viewer/res/common/phong.glsl>

in vec3 fragmentPositionW;
in vec3 fragmentNormalW;
in vec3 fragmentTangentW;
in vec3 fragmentBitangentW;
in vec2 fragmentTexture0;

out vec4 output;

uniform int mode = 70;
uniform vec4 color;
uniform float near; // camera z near
uniform float far; // camera z far

uniform sampler2D textureSampler0;
uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;
uniform sampler2D textureSampler3;

uniform vec3 eyeW;
uniform Light sun;

void main() {
	if (mode == 0)
		output = vec4(1, 0, 0, 1);
	else if (mode == 1)
		output = color;

	else if (mode == 10)
		output = vec4(fragmentPositionW, 1);
	else if (mode == 11)
		output = vec4(abs(mod(fragmentPositionW, 50)) / 50, 1);
	else if (mode == 12)
		output = vec4(abs(mod(fragmentPositionW, 5)) / 5, 1);

	else if (mode == 20)
		output = vec4(fragmentNormalW, 1);
	else if (mode == 21)
		output = vec4(abs(fragmentNormalW), 1);
	else if (mode == 22)
		output = vec4(fragmentTangentW, 1);
	else if (mode == 23)
		output = vec4(abs(fragmentTangentW), 1);
	else if (mode == 24)
		output = vec4(fragmentBitangentW, 1);
	else if (mode == 25)
		output = vec4(abs(fragmentBitangentW), 1);

	else if (mode == 30)
		output = vec4(fragmentTexture0, 0, 1);

	else if (mode == 40)
		output = texture(textureSampler0, fragmentTexture0);
	else if (mode == 41)
		output = texture(textureSampler1, fragmentTexture0);
	else if (mode == 42)
		output = texture(textureSampler2, fragmentTexture0);
	else if (mode == 43)
		output = texture(textureSampler3, fragmentTexture0);

	else if (mode == 50)
		output = vec4(vec3(1, 1, 1) * gl_FragCoord.z, 1);
	else if (mode == 51)
		output = vec4(vec3(1, 1, 1) * linearize_depth(gl_FragCoord.z, near, far), 1);
	else if (mode == 52)
		output = vec4(vec3(1, 1, 1) * mod(linearize_depth(gl_FragCoord.z, near, far), 25) / 25, 1);

	else if (mode == 70) {
		Material material;

		material.ambient = vec3(1.0, 1.0, 1.0);
		material.diffuse = vec3(0.8, 0.8, 0.8);
		material.specular = vec3(1.0, 1.0, 1.0);
		material.emission = vec3(0.0, 0.0, 0.0);

		material.shininess = 16.0;

		output = phong(eyeW, fragmentPositionW, normalize(fragmentNormalW), material, sun);
	}
}
