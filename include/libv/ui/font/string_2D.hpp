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
	Anchor halign = Anchor::Center;
	Anchor valign = Anchor::Center;

	std::string text;
	std::vector<Font2D::Character> data;
	void setText(const std::string&) { }
	void render() { }
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv