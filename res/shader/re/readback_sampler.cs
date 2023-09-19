#version 450 core

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;

// -------------------------------------------------------------------------------------------------

layout(std430, binding = 0) readonly restrict buffer InputBuffer {
    vec2 coordinates[];
};

layout(std430, binding = 1) writeonly restrict buffer OutputBuffer {
    vec4 results[];
};

// -------------------------------------------------------------------------------------------------

layout(local_size_x = 64, local_size_y = 1) in;
void main() {
	uint index = gl_GlobalInvocationID.x;
	vec2 uv = coordinates[index];
	vec4 texel = texture(textureDiffuse, uv);
	results[index] = texel;
}
