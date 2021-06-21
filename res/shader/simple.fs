#version 330 core

uniform sampler2D textureDiffuseSampler;

in vec4 fragmentColor;
in vec3 fragmentNormalW;
in vec3 fragmentTangentW;
in vec3 fragmentBitangentW;
in vec2 fragmentUV;

out vec4 color;

void main() {
	vec3 normal = normalize(fragmentNormalW);
	//color = texture( textureDiffuseSampler, fragmentUV ).rgba * fragmentColor;
	//color = fragmentColor;
	//color = vec4(1, 0, 0, 1);

	//color = material.diffuse * (vec4(fragmentNormal * 0.5 + vec3(0.5, 0.5, 0.5), 1));
	color = vec4(normal * 0.5 + vec3(0.5, 0.5, 0.5), 1);
}
