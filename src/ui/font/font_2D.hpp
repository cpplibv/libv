// File: Font2D.hpp, Created on 2014. november 30. 14:32, Author: Vader

#pragma once

// ext
#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>
// vl
#include <vl/vec.hpp>
// std
#include <iostream>
#include <fstream>
#include <string>

#define FONT2D_ALIGN_TOP 0.0f
#define FONT2D_ALIGN_CENTER 0.5f
#define FONT2D_ALIGN_BOTTOM 1.0f
#define FONT2D_ALIGN_RIGHT 1.0f
#define FONT2D_ALIGN_LEFT 0.0f

namespace vl {
namespace ui {

class Font2D {
public:

	struct Char {
		vec2 vertexCoord[4];
		vec2 textureCoord[4];
		float width;
	};

	int height; // The Height Of The Font.
	int lineHeight; // The Space Between The Lines.

	unsigned int texture;
	Char chars[256];

	Font2D();
	~Font2D();

	void load(const std::string &fname, unsigned int h); //Load a TTF file with h size
	void clean(); //Clean up: free the memory

	void begin(); //Set some gl parameter(have to call before prints)
	void end(); //Set some gl parameter(have to call after prints)

	void alignHorizontal(float x);
	void alignVertical(float x);
	void align(float horizontal, float vertical);

	//Print formated TEXT in X Y with FONT style (TEXT can not be longer (with format) then 255 char.)
	void printf(float x, float y, const char *text, ...);
	//Print TEXT in X Y with FONT style (Faster)
	void print(float x, float y, const char *text);

protected:
	float alignH;
	float alignV;

	float getLineWidth(const char * text, int i);
	int getLineNumber(const char * text);
	void makeChars(FT_Face face);
};

} //namespace ui
} //namespace vl