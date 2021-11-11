#version 330 core

out vec2 fragmentTexture0;

const vec2 positions[3] = vec2[3](
		vec2(-1, -1),
		vec2( 3, -1),
		vec2(-1,  3)
);

const vec2 uvs[3] = vec2[3](
		vec2(0, 0),
		vec2(2, 0),
		vec2(0, 2)
);

void main() {
	fragmentTexture0 = uvs[gl_VertexID];
	gl_Position = vec4(positions[gl_VertexID], 0, 1);
}
