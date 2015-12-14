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

	RenderPrimitiv quad(const vec3& pos, const vec3& size,
			const vec2& txbl = vec2(0, 0), const vec2& txtr = vec2(1, 1));
	RenderPrimitiv triangle(const vec3& a, const vec3& b, const vec3& c,
			const vec2& ta, const vec2& tb, const vec2& tc);

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
	Vertex(const vec3& position, const vec3& normal, const vec3& tangent,
			const vec3& bitangent, const vec2& texCoord0);

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