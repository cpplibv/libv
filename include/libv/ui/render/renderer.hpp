// File: UIRenderVBO.hpp, Created on 2015. április 30. 0:01, Author: Vader

#pragma once

// ext
#include <glm/mat4x4.hpp>
// libv
#include <libv/vec.hpp>
#include <libv/gl/gl.hpp>
// std
#include <stack>
#include <vector>

namespace libv {
namespace ui {

class RenderPrimitiv;
class Vertex;

class Renderer : public gl::GL {
	std::vector<Vertex> data;
public:

	RenderPrimitiv quad(vec3 pos, vec3 size,
			vec2 txbl = vec2(0, 0), vec2 txtr = vec2(1, 1));
	RenderPrimitiv triangle(vec3 a, vec3 b, vec3 c,
			vec2 ta, vec2 tb, vec2 tc);

	void upload();
	void clear();
};

// -------------------------------------------------------------------------------------------------

struct Vertex {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 texCoord0;

	Vertex();
	Vertex(vec3 position, vec3 normal, vec3 tangent,
			vec3 bitangent, vec2 texCoord0);

};

// -------------------------------------------------------------------------------------------------

class RenderPrimitiv {
	size_t first;
	size_t count;
public:
	RenderPrimitiv();
	RenderPrimitiv(size_t first, size_t count);

	void render();
};

} //namespace ui
} //namespace libv