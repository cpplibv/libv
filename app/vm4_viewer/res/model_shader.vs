#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 4) in ivec4 vertexBondID;
layout(location = 5) in vec4 vertexBondWieght;
layout(location = 6) in vec3 vertexTangent;
layout(location = 7) in vec3 vertexBitangent;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentPositionW;
out vec3 fragmentNormalW;
out vec3 fragmentTangentW;
out vec3 fragmentBitangentW;
out vec2 fragmentTexture0;

uniform mat4 Mmat;
uniform mat4 MVPmat;

void main() {
	gl_Position = MVPmat * vec4(vertexPosition, 1);

	fragmentPositionW = vec3(Mmat * vec4(vertexPosition, 1 ));
	fragmentNormalW = mat3(Mmat) * vertexNormal;
	fragmentTangentW = mat3(Mmat) * vertexTangent;
	fragmentBitangentW = mat3(Mmat) * vertexBitangent;
	// fragmentBinormalW = cross(fragmentNormalW, fragmentTangentW);

	fragmentTexture0 = vertexTexture0;
}
