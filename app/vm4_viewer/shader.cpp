// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class RenderMode {
	color = 0,

	positionW = 1,
	normalW = 2,
	tangentW = 3,
	bitangentW = 4,

	texture0 = 5,
	depth = 6,

	sample0 = 10,
	sample1 = 11,
	sample2 = 12,
	sample3 = 13,
	sample4 = 14,
	sample5 = 15,
	sample6 = 16,
	sample7 = 17,
};

ShaderModel::ShaderModel() {
//	define("MODE_COLOR", RenderMode::color);
//
//	define("MODE_POSITION_W", RenderMode::positionW);
//	define("MODE_NORMAL_W", RenderMode::normalW);
//	define("MODE_TANGENT_W", RenderMode::tangentW);
//	define("MODE_BITANGENT_W", RenderMode::bitangentW);
//
//	define("MODE_TEXTURE0", RenderMode::texture0);
//	define("MODE_DEPTH", RenderMode::depth);
//
//	define("MODE_SAMPLE0", RenderMode::sample0);
//	define("MODE_SAMPLE1", RenderMode::sample1);
//	define("MODE_SAMPLE2", RenderMode::sample2);
//	define("MODE_SAMPLE3", RenderMode::sample3);
//	define("MODE_SAMPLE4", RenderMode::sample4);
//	define("MODE_SAMPLE5", RenderMode::sample5);
//	define("MODE_SAMPLE6", RenderMode::sample6);
//	define("MODE_SAMPLE7", RenderMode::sample7);

	vertex(R"(
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
			fragmentNormalW = vertexNormal * mat3(Mmat);
			fragmentTangentW = vertexTangent * mat3(Mmat);
			fragmentBitangentW = vertexBitangent * mat3(Mmat);
//				// fragmentBinormalW = cross(fragmentNormalW, fragmentTangentW);

			fragmentTexture0 = vertexTexture0;
		}
	)");
	fragment(R"(
		#version 330 core

		in vec3 fragmentPositionW;
		in vec3 fragmentNormalW;
		in vec3 fragmentTangentW;
		in vec3 fragmentBitangentW;
		in vec2 fragmentTexture0;

		out vec4 output;

		uniform int mode = 0;
		uniform vec4 color;
		uniform float near; // camera z near
		uniform float far; // camera z far
//			uniform sampler2D textureSampler;

		float linearize_depth(float zoverw) {
			return (2.0 * near) / (far + near - zoverw * (far - near)) * (far - near);
		}

		void main() {
//				vec4 sample = texture(textureSampler, fragmentTexture0).xyzw;
//				vec4 sample = texture(textureSampler, fragmentTexture0);

			if (mode == 0)
				output = color;

			else if (mode == 1)
				output = vec4(abs(mod(fragmentPositionW, 5)) / 5, 1);
			else if (mode == 2)
				output = vec4(abs(fragmentNormalW), 1);
			else if (mode == 3)
				output = vec4(abs(fragmentTangentW), 1);
			else if (mode == 4)
				output = vec4(abs(fragmentBitangentW), 1);
			else if (mode == 5)
				output = vec4(fragmentTexture0, 0, 1);

			else if (mode == 6) {
				float depth = mod(linearize_depth(gl_FragCoord.z), 20) / 20;
				output = vec4(depth, depth, depth, 1);
			}

//			else if (mode == 10) {
//				vec4 sample = texture(textureSampler, fragmentTexture0);
//				output = sample;
//			}
		}
	)");

	assign(uniform_color, "color", {1, 1, 1, 1});
	assign(uniform_Mmat, "Mmat", libv::mat4f::identity());
	assign(uniform_MVPmat, "MVPmat", libv::mat4f::identity());
	assign(uniform_texture, "textureSampler", textureChannel);
	assign(uniform_mode, "mode", 2);
	assign(uniform_near, "near", 0.1f);
	assign(uniform_far, "far", 1000.0f);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
