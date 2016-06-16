// File:   string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/gl/shader.hpp>
#include <libv/vec.hpp>
// std
#include <string>
#include <vector>
// pro
#include <libv/ui/font/font_2D.hpp>
#include <libv/ui/render/renderer.hpp>

// TODO P2: fix new line 'inverting'
// TODO P2: String2D alignments (yee, the part where FLOW Layout is kind of familiar...)
// TODO P3: Format
// TODO P4: multi font
// TODO P4: multi shader

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class Anchor {
	Top,
	Center,
	Bottom,
	Right,
	Left
};

class String2D {
public:
	struct Character {
		vec2 vertexCoord[6];
		vec2 textureCoord[6];
	};
private:
	Anchor halign = Anchor::Center;
	Anchor valign = Anchor::Center;

	ivec2 maxSize;
	ivec2 layoutedSize;

	std::string rawText;
	std::vector<String2D::Character> data;

	//VBO
	//VBA

	Font2D deafultFont;
	std::shared_ptr<gl::ShaderProgram> defaultShader;
	gl::Uniform_fmat4 uniformMVPmat;
	gl::Uniform_texture uniformTextureSampler;
	// TODO P5: Shader should be part of the "font descriptor" to allow same font with multiple shader...

public:
	String2D() = default;
	String2D(const std::string& text, Anchor halign = Anchor::Center, Anchor valign = Anchor::Center);

public:
	void setText(const std::string& text);
	void setSize(ivec2 size);

	ivec2 getLayoutedSize() const {
		return layoutedSize;
	}

	std::string getText() const {
		return rawText;
	}

public:
	void build();
	void destroy();
	void render(Renderer& gl);

private:
	void buildImpl();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv