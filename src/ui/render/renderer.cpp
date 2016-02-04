// File: ui_render_vbo.cpp, Created on 2015. április 30. 0:09, Author: Vader

// hpp
#include <libv/ui/render/renderer.hpp>
// ext
#include <GL/glew.h>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

RenderPrimitiv Renderer::quad(vec3 pos, vec3 size, vec2 txbl, vec2 txtr) {
	float xmin = pos.x;
	float xmax = pos.x + size.x;
	float ymin = pos.y;
	float ymax = pos.y + size.y;

	float umin = txbl.x;
	float umax = txtr.x;
	float vmin = txbl.y;
	float vmax = txtr.y;

	data.emplace_back(vec3(xmin, ymin, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umin, vmin));
	data.emplace_back(vec3(xmax, ymin, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umax, vmin));
	data.emplace_back(vec3(xmax, ymax, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umax, vmax));
	data.emplace_back(vec3(xmax, ymax, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umax, vmax));
	data.emplace_back(vec3(xmin, ymax, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umin, vmax));
	data.emplace_back(vec3(xmin, ymin, pos.z), vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), vec2(umin, vmin));

	return RenderPrimitiv(data.size() - 6, 6);
}

RenderPrimitiv Renderer::triangle(
		vec3 a, vec3 b, vec3 c,
		vec2 ta, vec2 tb, vec2 tc) {
	data.emplace_back(a, vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), ta);
	data.emplace_back(b, vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), tb);
	data.emplace_back(c, vec3(0, 0, 1), vec3(0, 1, 0), vec3(1, 0, 0), tc);

	return RenderPrimitiv(data.size() - 3, 3);
}

// -------------------------------------------------------------------------------------------------

Vertex::Vertex() { }

Vertex::Vertex(vec3 position, vec3 normal, vec3 tangent,
		vec3 bitangent, vec2 texCoord0) :
	position(position),
	normal(normal),
	tangent(tangent),
	bitangent(bitangent),
	texCoord0(texCoord0) { }


// -------------------------------------------------------------------------------------------------

RenderPrimitiv::RenderPrimitiv() :
	first(0), count(0) { }

RenderPrimitiv::RenderPrimitiv(size_t first, size_t count) :
	first(first), count(count) { }

void RenderPrimitiv::render() {
	glDrawArrays(GL_TRIANGLES, static_cast<GLsizei> (first), static_cast<GLint> (count));
}

} //namespace ui
} //namespace libv