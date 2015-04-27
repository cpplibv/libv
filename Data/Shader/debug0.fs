#version 330 core

uniform sampler2D textureDiffuseSampler;
uniform sampler2D textureNormalSampler;
uniform sampler2D textureAmbientSampler;

in vec4 fragmentColor;
in vec3 fragmentNormalW;
in vec3 fragmentTangentW;
in vec3 fragmentBitangentW;
in vec2 fragmentUV;

out vec4 color;

void main() {
	vec3 normal = normalize(fragmentNormalW);
	//color = texture2D( textureDiffuseSampler, fragmentUV ).rgba * fragmentColor;
	//color = fragmentColor;
	//color = vec4(1, 0, 0, 1);

	//color = material.diffuse * (vec4(fragmentNormal * 0.5 + vec3(0.5, 0.5, 0.5), 1));
	//color = vec4(normal * 0.5 + vec3(0.5, 0.5, 0.5), 1);
	vec4 diffuseTex = texture2D( textureDiffuseSampler, fragmentUV, 0).rgba;
	vec4 ambientTex = texture2D( textureAmbientSampler, fragmentUV, 0).rgba;
	vec4 normalTex = texture2D( textureNormalSampler, fragmentUV, 0).rgba;
	color = diffuseTex * ambientTex;// + normalTex;
}