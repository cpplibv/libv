#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture_noise;
uniform vec2 render_resolution; // Program block
uniform vec4 noise_scale;
uniform vec4 base_color;


void main() {
	vec2 uv_scale = render_resolution / textureSize(texture_noise, 0);
	vec4 noise_sample = texture2D(texture_noise, fragmentTexture0 * uv_scale, 0).rgba;

	result = base_color - noise_scale * 0.5 + noise_sample * noise_scale;
}
