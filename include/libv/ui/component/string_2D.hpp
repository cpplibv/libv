// File:   string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/gl/vertex_buffer.hpp>
#include <libv/vec.hpp>
// std
#include <string>
#include <vector>
// pro
//#include <libv/ui/font/font_2D.hpp>
#include <libv/ui/resource/font.hpp>
#include <libv/ui/resource/program.hpp>
#include <libv/ui/resource/resource_fwd.hpp>

// TODO P2: fix new line 'inverting'
// TODO P2: String2D alignments (yee, the part where FLOW Layout is kind of familiar...)
// TODO P3: Format
// TODO P4: multi font
// TODO P4: multi program

//newline       \n
//tab           \t
//New-line      [br]{ignore}[/br]
//Bold          [b]{text}[/b]
//Italic        [i]{text}[/i]
//Underline		[u]{text}[/u]
//Line-through  [s]{text}[/s]
//Font          [font={font}]{text}[/font]
//Font-size     [size={number}]{text}[/size]
//Color         [color={color}]{text}[/color]
//Center text   [center]{text}[/center]
//Align text    [align={align}]{text}[/align]
//Program       [program={program}]{text}[/program]

namespace libv {
namespace ui {

class Context;

// -------------------------------------------------------------------------------------------------

enum class Anchor {
	Top,
	Center,
	Bottom,
	Right,
	Left
};

class String2D {
private:
	class Builder;

	struct Character {
		struct Vertex {
			vec2 position;
			vec2 texture0;
		};
		Vertex vertices[6];

		Character(ivec2 offset, const Font2D::Character& ch) {
			vertices[0].position = offset + ch.vertexCoord[0];
			vertices[1].position = offset + ch.vertexCoord[1];
			vertices[2].position = offset + ch.vertexCoord[2];
			vertices[3].position = offset + ch.vertexCoord[2];
			vertices[4].position = offset + ch.vertexCoord[3];
			vertices[5].position = offset + ch.vertexCoord[0];

			vertices[0].texture0 = ch.textureCoord[0];
			vertices[1].texture0 = ch.textureCoord[1];
			vertices[2].texture0 = ch.textureCoord[2];
			vertices[3].texture0 = ch.textureCoord[2];
			vertices[4].texture0 = ch.textureCoord[3];
			vertices[5].texture0 = ch.textureCoord[0];
		}
	};

	struct Segment {
		uint32_t vertexStart = 0;
		uint32_t vertexCount = 0;
		UIFont2D_view font;
		UIProgram_view program;
		fvec4 color;
		uint32_t size;

		Segment(uint32_t vertexStart,
				const UIFont2D_view& font,
				const UIProgram_view& program,
				fvec4 color,
				uint32_t size) :
			vertexStart(vertexStart),
			font(font),
			program(program),
			color(color),
			size(size) { }
	};
private:
	Anchor halign = Anchor::Center;
	Anchor valign = Anchor::Center;

	ivec2 maxSize;
	ivec2 layoutedSize;

	bool renderChanged = true;
	std::string rawText;
	std::vector<Segment> segments;
	std::vector<Character> characters;

	gl::VertexArray vao;
	gl::VertexBuffer vbo;

public:
	String2D() = default;
	String2D(const std::string& text, Anchor halign = Anchor::Center, Anchor valign = Anchor::Center);

public:
	void setText(const std::string& text);
	void setSize(ivec2 size);

	ivec2 getLayoutedSize() const {
		return layoutedSize;
	}

	const std::string& getText() const {
		return rawText;
	}

public:
	ivec2 layout();
	void build(Context& context);
	void destroy(Context& context);
	void render(Context& context);

private:
	void buildImpl(Context& context);
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv