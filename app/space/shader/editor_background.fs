#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D textureNoise;
uniform vec2 noiseUVScale;
uniform vec4 noiseScale;
uniform vec4 noiseOffset;

//uniform vec4 color0;
//uniform vec4 color1;
//uniform vec4 colorCurve;

void main() {
	vec4 noise = texture2D(textureNoise, fragmentTexture0 * noiseUVScale, 0).rgba;
	result = noise * noiseScale + noiseOffset;
}
