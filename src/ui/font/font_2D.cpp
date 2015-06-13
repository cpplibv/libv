// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include "font_2D.hpp"
// vl
#include <vl/gl/log.hpp>
#include <vl/gl/vgl.hpp>

namespace vl {
namespace ui {

inline int next_p2(int a) {
	int rval = 1;
	while (rval < a) rval <<= 1;
	return rval;
}

Font2D::Font2D() : alignH(FONT2D_ALIGN_LEFT), alignV(FONT2D_ALIGN_TOP) { }

Font2D::~Font2D() {
	clean();
}

void Font2D::load(const std::string &fname, unsigned int h) {
	this->height = h;
	this->lineHeight = 3;

	//	std::string filePath = GAME_FONT_FOLDER + fname;
	std::string filePath = fname;

	FT_Library library; // Create And Initilize A FreeType Font.
	FT_Face face;
	if (FT_Init_FreeType(&library))
		std::cout << "FT_Init_FreeType failed." << std::endl;
	if (FT_New_Face(library, filePath.c_str(), 0, &face))
		std::cout << "FT_New_Face failed. [" << filePath << "]" << std::endl;
	if (FT_Set_Char_Size(face, h << 6, h << 6, 96, 96))
		std::cout << "FT_Set_Char_Size failed. [" << h << "]" << std::endl;

	makeChars(face);

	FT_Done_Face(face); // We Don't Need The Face Information Now That The Display
	FT_Done_FreeType(library); // Lists Have Been Created, So We Free The Assosiated Resources.
}

void Font2D::clean() {
	glDeleteTextures(1, &texture);
}

void Font2D::makeChars(FT_Face face) {
	FT_Glyph glyphs[256];
	int charWidth(0), charHeight(0);

	for (int ch = 0; ch < 256; ch++) { //Get bitmaps
		FT_Glyph& glyph = glyphs[ch];
		int charpos = FT_Get_Char_Index(face, ch);
		//	if (charpos)std::cout << "FT_Get_Char_Index failed: " << ch << std::endl; //This fail is acceptable
		if (FT_Load_Glyph(face, charpos, FT_LOAD_DEFAULT))// Load The Glyph For Our Character.
			std::cout << "FT_Load_Glyph failed." << std::endl;
		if (FT_Get_Glyph(face->glyph, &glyph))// Move The Face's Glyph Into A Glyph Object.
			std::cout << "FT_Get_Glyph failed." << std::endl;
		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1); // Convert The Glyph To A Bitmap.
		FT_BitmapGlyph bitmapglyph = (FT_BitmapGlyph) glyphs[ch];

		if (bitmapglyph->bitmap.width > charWidth) charWidth = bitmapglyph->bitmap.width;
		if (bitmapglyph->bitmap.rows > charHeight) charHeight = bitmapglyph->bitmap.rows;

		chars[ch].width = face->glyph->advance.x >> 6;

	}

	int width = next_p2(charWidth * 16);
	int height = next_p2(charHeight * 16);
	GLubyte data[2 * width * height];

	for (int i = 0; i < width * height; i++) {
		data[i * 2] = 255;
		data[i * 2 + 1] = 0;
	}

	for (int i = 0; i < 16; i++) { //sor
		for (int j = 0; j < 16; j++) { //oszlop

			int ch = i * 16 + j;
			FT_BitmapGlyph glyph = (FT_BitmapGlyph) glyphs[ch];
			FT_Bitmap& bitmap = glyph->bitmap;

			float x0 = (float) (j * charWidth) / (float) width;
			float y0 = (float) (i * charHeight) / (float) height;
			float x1 = (float) (j * charWidth + bitmap.width) / (float) width;
			float y1 = (float) (i * charHeight + bitmap.rows) / (float) height;
			float left = glyph->left;
			float top = glyph->top - bitmap.rows;

			chars[ch].textureCoord[0] = vec2(x0, y1);
			chars[ch].textureCoord[1] = vec2(x1, y1);
			chars[ch].textureCoord[2] = vec2(x0, y0);
			chars[ch].textureCoord[3] = vec2(x1, y0);
			chars[ch].vertexCoord[0] = vec2(left, top);
			chars[ch].vertexCoord[1] = vec2(left + bitmap.width, top);
			chars[ch].vertexCoord[2] = vec2(left, top + bitmap.rows);
			chars[ch].vertexCoord[3] = vec2(left + bitmap.width, top + bitmap.rows);

			for (int k = 0; k < bitmap.rows; k++) { //bitmap sor
				for (int l = 0; l < bitmap.width; l++) { //bitmap sor
					data[(j * charWidth + l + (i * charHeight + k) * width) * 2 + 1] = bitmap.buffer[l + bitmap.width * k];
				}
			}
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGL();

	for (unsigned int ch = 0; ch < 256; ch++) { //free glyphs
		FT_Done_Glyph(glyphs[ch]);
	}
}

void Font2D::begin() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
}

void Font2D::end() {
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

float Font2D::getLineWidth(const char * text, int i) {
	float result = 0;
	while (text[i] != '\n' && text[i] != '\0')
		result += this->chars[(unsigned char) text[i++]].width;
	return result;
}

int Font2D::getLineNumber(const char * text) {
	int i = 0, result = 1;
	while (text[i] != '\0') {
		if (text[i] == '\n')
			result++;
		i++;
	}
	return result;
}

void Font2D::printf(float x, float y, const char *fmt, ...) {
	char text[256];

	va_list ap; // Pointer To List Of Arguments
	if (fmt == NULL) // If There's No Text
		*text = 0; // Do Nothing
	else {
		va_start(ap, fmt); // Parses The String For Variables
		vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
		va_end(ap); // Results Are Stored In Text
		text[255] = '\0'; //not sure if its need, but dont hurt.
	}

	vec2 pen(x - (int) (getLineWidth(text, 0) * this->alignH), y - this->height + (int) (getLineNumber(text) * (this->height + this->lineHeight) * this->alignV));
	int i = 0;
	while (text[i] != '\0') {
		unsigned char ch = text[i];
		if (text[i] == '\n') {
			pen.x = x - (int) getLineWidth(text, i + 1) * this->alignH;
			pen.y -= this->lineHeight + this->height;
		} else {
			glTexCoord2f(this->chars[ch].textureCoord[0].x, this->chars[ch].textureCoord[0].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[0].x, pen.y + this->chars[ch].vertexCoord[0].y);
			glTexCoord2f(this->chars[ch].textureCoord[1].x, this->chars[ch].textureCoord[1].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[1].x, pen.y + this->chars[ch].vertexCoord[1].y);
			glTexCoord2f(this->chars[ch].textureCoord[3].x, this->chars[ch].textureCoord[3].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[3].x, pen.y + this->chars[ch].vertexCoord[3].y);
			glTexCoord2f(this->chars[ch].textureCoord[2].x, this->chars[ch].textureCoord[2].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[2].x, pen.y + this->chars[ch].vertexCoord[2].y);
			pen.x += this->chars[ch].width;
		}
		i++;
	}
}

int Hex2Int(char n) {
	if (n >= '0' && n <= '9')
		return (n - '0');
	else
		if (n >= 'A' && n <= 'F')
		return (n - 'A' + 10);
	else
		return 0;
}

void Font2D::print(float x, float y, const char *text) {
	//chars
	// \0 - 00 - end mark
	// \n - 13 -  new line
	//str
	// "\" - protector
	// "#" - hex color start (#FFFFFF)
	vec2 pen(x - (int) (getLineWidth(text, 0) * this->alignH), y - this->height + (int) (getLineNumber(text) * (this->height + this->lineHeight) * this->alignV));
	int i = 0;

	while (text[i] != '\0') {
		unsigned char ch = text[i];
		if (text[i] == '\n') {
			pen.x = x - (int) (getLineWidth(text, i + 1) * this->alignH);
			pen.y -= this->lineHeight + this->height;
		} else if (text[i] == '#' && (i == 0 || text[i - 1] != '\\')) { //pass if "#" but not if "\#"
			glColor4f((Hex2Int(text[i + 1])*16 + Hex2Int(text[i + 2])) / 255.0f,
					(Hex2Int(text[i + 3])*16 + Hex2Int(text[i + 4])) / 255.0f,
					(Hex2Int(text[i + 5])*16 + Hex2Int(text[i + 6])) / 255.0f,
					1.0f);
			i += 6;
		} else if (text[i] == '\\' && text[i + 1] == '#') { //if char is a protector -> do nothing
		} else {
			glTexCoord2f(this->chars[ch].textureCoord[0].x, this->chars[ch].textureCoord[0].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[0].x, pen.y + this->chars[ch].vertexCoord[0].y);
			glTexCoord2f(this->chars[ch].textureCoord[1].x, this->chars[ch].textureCoord[1].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[1].x, pen.y + this->chars[ch].vertexCoord[1].y);
			glTexCoord2f(this->chars[ch].textureCoord[3].x, this->chars[ch].textureCoord[3].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[3].x, pen.y + this->chars[ch].vertexCoord[3].y);
			glTexCoord2f(this->chars[ch].textureCoord[2].x, this->chars[ch].textureCoord[2].y);
			glVertex2f(pen.x + this->chars[ch].vertexCoord[2].x, pen.y + this->chars[ch].vertexCoord[2].y);
			pen.x += this->chars[ch].width;
		}
		i++;
	}
}

void Font2D::alignHorizontal(float x) {
	this->alignH = x;
}

void Font2D::alignVertical(float x) {
	this->alignV = x;
}

void Font2D::align(float horizontal, float vertical) {
	this->alignH = horizontal;
	this->alignV = vertical;
}

} //namespace ui
} //namespace vl