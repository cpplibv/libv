// File:   string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/vec.hpp>
// std
#include <string>
#include <vector>
// pro
#include <libv/ui/font/font_2D.hpp>

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
		vec2 vertexCoord[4];
		vec2 textureCoord[4];
	};
private:
	Anchor halign = Anchor::Center;
	Anchor valign = Anchor::Center;

	std::string rawText;
	std::vector<String2D::Character> data;

	ivec2 size{0, 0};

	//VBO
	//VBA

	Font2D deafultFont;

public:
	String2D() = default;
	String2D(const std::string& text, Anchor halign = Anchor::Center, Anchor valign = Anchor::Center);

public:
	void setText(const std::string& text);
	void setSize(ivec2 size);

public:
	void build();
	void destroy();
	void render();

private:
	void buildImpl();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv