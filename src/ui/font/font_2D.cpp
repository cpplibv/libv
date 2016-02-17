// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/font/font_2D.hpp>
// ext
#include <boost/filesystem/path.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
// libv
#include <libv/read_file.hpp>
#include <libv/vec.hpp>
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>
// std
#include <atomic>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

static constexpr const char ADDITIONAL_PREFETCH_CHAR_LIST[] = "!@#\\/%?|()[]{}<>+*_,.-=^'\"";

// -------------------------------------------------------------------------------------------------

static FT_Library ftLib = nullptr;
static std::atomic<size_t> FT_LibUseCount{0};

// -------------------------------------------------------------------------------------------------

Font2D::Font2D() {
	if (!ftLib)
		if (const auto err = FT_Init_FreeType(&ftLib))
			LIBV_UI_FT_ERROR("FT_Init_FreeType failed: [%d]", err);
	++FT_LibUseCount;

	for (size_t i = 0; i < textureData.size(); ++i) {
		textureData[i] = 0;
	}
}

Font2D::Font2D(const boost::filesystem::path& filePath) : Font2D() {
	load(filePath);
}

Font2D::Font2D(const void* data, const size_t size) : Font2D() {
	load(data, size);
}

Font2D::~Font2D() {
	if (textureID)
		unload();
	if (!--FT_LibUseCount)
		if (const auto err = FT_Done_FreeType(ftLib))
			LIBV_UI_FT_ERROR("FT_Done_FreeType failed: [%d]", err);
}

void Font2D::load(const boost::filesystem::path& filePath) {
	fileContent = readFile(filePath, std::ios_base::binary);
	loadFace();
}

void Font2D::load(const void* data, const size_t size) {
	fileContent = std::string(static_cast<const char*> (data), size);
	loadFace();
}

void Font2D::loadFace() {
	if (const auto err = FT_New_Memory_Face(ftLib,
			reinterpret_cast<const FT_Byte*> (fileContent.data()), fileContent.size(), 0, &face))
		return LIBV_UI_FT_ERROR("FT_New_Memory_Face failed: [%d]", err);

	setSize(activeFontSize);

	defaultCharacter = getCharacter(0);
	for (unsigned int i = '0'; i <= '9'; ++i)
		getCharacter(i);
	for (unsigned int i = 'A'; i <= 'Z'; ++i)
		getCharacter(i);
	for (unsigned int i = 'a'; i <= 'z'; ++i)
		getCharacter(i);
	for (unsigned int i = 0; i < sizeof (ADDITIONAL_PREFETCH_CHAR_LIST) - 1; ++i)
		getCharacter(ADDITIONAL_PREFETCH_CHAR_LIST[i]);
}

void Font2D::unload() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	FT_Done_Face(face);
	face = nullptr;
}

// -------------------------------------------------------------------------------------------------

bool Font2D::isLoaded() const {
	return face != nullptr;
}

void Font2D::setSize(int px) {
	activeFontSize = px;
	if (!face)
		return;

	if (const auto err = FT_Set_Char_Size(face, activeFontSize << 6, activeFontSize << 6, 96, 96))
		return LIBV_UI_FT_ERROR("FT_Set_Char_Size failed: [%d]", err);
}

int Font2D::getLineAdvance() const {
	return face->size->metrics.height >> 6; // 26.6 format
}

int Font2D::getDescender() const {
	return face->size->metrics.descender >> 6; // 26.6 format
}

ivec2 Font2D::getKerning(uint32_t left, uint32_t right) const {
	FT_Vector kerning;
	kerning.x = 0;
	kerning.y = 0;

	if (FT_HAS_KERNING(face)) {
		int leftID = FT_Get_Char_Index(face, left);
		int rightID = FT_Get_Char_Index(face, right);

		if (const auto err = FT_Get_Kerning(face, leftID, rightID, FT_KERNING_DEFAULT, &kerning))
			LIBV_UI_FT_ERROR("FT_Get_Kerning failed: [%d]", err);
	}

	return ivec2(kerning.x >> 6, kerning.y >> 6); // 26.6 float
}

const Font2D::Character & Font2D::getCharacter(uint32_t unicode) {
	const auto infoIndex = CharacterIndex{unicode, activeFontSize};
	auto infoIt = characterInfos.find(infoIndex);
	if (infoIt == characterInfos.end()) {
		infoIt = characterInfos.emplace(infoIndex, renderCharacter(unicode)).first;
	}
	return infoIt->second;
}

Font2D::Character Font2D::renderCharacter(uint32_t unicode) {
	FT_Glyph glyph = nullptr;

	int charIndex = FT_Get_Char_Index(face, unicode);

	if (const auto err = FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT))
		LIBV_UI_FT_ERROR("FT_Load_Glyph failed: [%d]", err);
	if (const auto err = FT_Get_Glyph(face->glyph, &glyph))
		LIBV_UI_FT_ERROR("FT_Get_Glyph failed: [%d]", err);
	if (const auto err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true))
		LIBV_UI_FT_ERROR("FT_Glyph_To_Bitmap failed: [%d]", err);

	const auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph> (glyph);
	const auto bitmap = bitmapGlyph->bitmap;
	const auto bitmapWidth = int64_t{bitmap.width};
	const auto bitmapHeight = int64_t{bitmap.rows};

	Character result;
	result.advance.x = glyph->advance.x / 65536.0f; // 16.16 float
	result.advance.y = glyph->advance.y / 65536.0f; // 16.16 float

	if (texturePen.x + bitmapWidth > textureSize.x) // Warp to new line
		texturePen = uivec2(0, textureNextLine);

	if (texturePen.y + bitmapHeight > textureSize.y) { // Detect 'full' texture
		LIBV_UI_FT_ERROR("Failed to render character: Not enough place in font texture: "
				"unicode: [%d] size: [%dpx] bitmap size: [%d,%d], pen pos: [%d,%d]",
				unicode, activeFontSize, bitmapWidth, bitmapHeight, texturePen.x, texturePen.y);
		return defaultCharacter;
	}

	if (texturePen.y + bitmapHeight > textureNextLine) // Expand current line
		textureNextLine = texturePen.y + bitmapHeight;

	for (size_t y = 0; y < bitmap.rows; ++y) {
		for (size_t x = 0; x < bitmap.width; ++x) {
			const auto bitmapIndex = (bitmapHeight - y - 1) * bitmap.pitch + x;
			const auto textureIndex = (texturePen.y + y) * textureSize.x + texturePen.x + x;

			textureData[textureIndex] = bitmap.buffer[bitmapIndex];
		}
	}

	const auto tx0 = float(texturePen.x) / textureSize.x;
	const auto tx1 = float(texturePen.x + bitmapWidth) / textureSize.x;
	const auto ty0 = float(texturePen.y) / textureSize.y;
	const auto ty1 = float(texturePen.y + bitmapHeight) / textureSize.y;
	result.textureCoord[0] = vec2(tx0, ty0);
	result.textureCoord[1] = vec2(tx1, ty0);
	result.textureCoord[2] = vec2(tx1, ty1);
	result.textureCoord[3] = vec2(tx0, ty1);
	// <<< validate texture and pixel coord if they are ok (0.5ness)
	// and if its ok that the chars dont have any gap in the texture?

	const auto vx0 = bitmapGlyph->left;
	const auto vx1 = bitmapGlyph->left + bitmapWidth;
	const auto vy0 = bitmapGlyph->top - bitmapHeight;
	const auto vy1 = bitmapGlyph->top;

	result.vertexCoord[0] = vec2(vx0, vy0);
	result.vertexCoord[1] = vec2(vx1, vy0);
	result.vertexCoord[2] = vec2(vx1, vy1);
	result.vertexCoord[3] = vec2(vx0, vy1);

	texturePen.x += bitmapWidth; // Progress pen
	FT_Done_Glyph(glyph);

	dirty = true;
	return result;
}

void Font2D::uploadTexture() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, textureSize.x, textureSize.y, 0, GL_LUMINANCE,
			GL_UNSIGNED_BYTE, textureData.data());
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGL();
}

void Font2D::bind() {
	glEnable(GL_TEXTURE_2D);
	if (dirty) {
		glGenTextures(1, &textureID);
		uploadTexture();
		dirty = false;
	}

	gl::activeTexture(gl::TextureType::diffuse);
	glBindTexture(GL_TEXTURE_2D, textureID);
	checkGL();
}

void Font2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv


// Texture Fill algorithm (painfully basic version)
//
//                    Y+      texturePen.x   textureSize.x
//                    ^          |                |
//                    |          V                V
//   textureSize.y -> +---------------------------+
//                    |                           |
//                    |                           |
// textureNextLine -> |                           |
//                    |#  ## ## #+--+             | A
//                    |##### ## #|  |             | | bitmapSize.y
//    texturePen.y -> |##########+--+             | V
//                    |###   ########  #### ##  ##|
//                    |####  ############## ######|
//                    |###########################|
//                   -+---------------------------+-> X+
//                0,0 |          <-->
//                         bitmapSize.x


//float Font2D::getLineWidth(const char * text, int i) {
//	float result = 0;
//	while (text[i] != '\n' && text[i] != '\0')
//		result += this->chars[(unsigned char) text[i++]].width;
//	return result;
//}
//
//int Font2D::getLineNumber(const char * text) {
//	int i = 0, result = 1;
//	while (text[i] != '\0') {
//		if (text[i] == '\n')
//			result++;
//		i++;
//	}
//	return result;
//}
//
//void Font2D::printf(float x, float y, const char *fmt, ...) {
//	char text[256];
//
//	va_list ap; // Pointer To List Of Arguments
//	if (fmt == NULL) // If There's No Text
//		*text = 0; // Do Nothing
//	else {
//		va_start(ap, fmt); // Parses The String For Variables
//		vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
//		va_end(ap); // Results Are Stored In Text
//		text[255] = '\0'; //not sure if its need, but dont hurt.
//	}
//
//	vec2 pen(
//			x - std::floor(getLineWidth(text, 0) * this->alignH),
//			y - (float) this->height + std::floor((float) getLineNumber(text) * ((float) this->height + (float) this->lineHeight) * this->alignV));
//	int i = 0;
//	while (text[i] != '\0') {
//		unsigned char ch = text[i];
//		if (text[i] == '\n') {
//			pen.x = x - std::floor(getLineWidth(text, i + 1) * this->alignH);
//			pen.y -= (float) this->lineHeight + (float) this->height;
//		} else {
//			glTexCoord2f(this->chars[ch].textureCoord[0].x, this->chars[ch].textureCoord[0].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[0].x, pen.y + this->chars[ch].vertexCoord[0].y);
//			glTexCoord2f(this->chars[ch].textureCoord[1].x, this->chars[ch].textureCoord[1].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[1].x, pen.y + this->chars[ch].vertexCoord[1].y);
//			glTexCoord2f(this->chars[ch].textureCoord[3].x, this->chars[ch].textureCoord[3].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[3].x, pen.y + this->chars[ch].vertexCoord[3].y);
//			glTexCoord2f(this->chars[ch].textureCoord[2].x, this->chars[ch].textureCoord[2].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[2].x, pen.y + this->chars[ch].vertexCoord[2].y);
//			pen.x += this->chars[ch].width;
//		}
//		i++;
//	}
//}
//
//int Hex2Int(char n) {
//	if (n >= '0' && n <= '9')
//		return (n - '0');
//	else
//		if (n >= 'A' && n <= 'F')
//		return (n - 'A' + 10);
//	else
//		return 0;
//}
//
//void Font2D::print(float x, float y, const char *text) {
//	//chars
//	// \0 - 00 - end mark
//	// \n - 13 -  new line
//	//str
//	// "\" - protector
//	// "#" - hex color start (#FFFFFF)
//	vec2 pen(
//			x - std::floor(getLineWidth(text, 0) * this->alignH),
//			y - (float) this->height + std::floor((float) getLineNumber(text) * ((float) this->height + (float) this->lineHeight) * this->alignV));
//	int i = 0;
//
//	while (text[i] != '\0') {
//		unsigned char ch = text[i];
//		if (text[i] == '\n') {
//			pen.x = x - std::floor(getLineWidth(text, i + 1) * this->alignH);
//			pen.y -= (float) this->lineHeight + (float) this->height;
//		} else if (text[i] == '#' && (i == 0 || text[i - 1] != '\\')) { //pass if "#" but not if "\#"
//			glColor4f(
//					(float) (Hex2Int(text[i + 1])*16 + Hex2Int(text[i + 2])) / 255.0f,
//					(float) (Hex2Int(text[i + 3])*16 + Hex2Int(text[i + 4])) / 255.0f,
//					(float) (Hex2Int(text[i + 5])*16 + Hex2Int(text[i + 6])) / 255.0f,
//					1.0f);
//			i += 6;
//		} else if (text[i] == '\\' && text[i + 1] == '#') { //if char is a protector -> do nothing
//		} else {
//			glTexCoord2f(this->chars[ch].textureCoord[0].x, this->chars[ch].textureCoord[0].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[0].x, pen.y + this->chars[ch].vertexCoord[0].y);
//			glTexCoord2f(this->chars[ch].textureCoord[1].x, this->chars[ch].textureCoord[1].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[1].x, pen.y + this->chars[ch].vertexCoord[1].y);
//			glTexCoord2f(this->chars[ch].textureCoord[3].x, this->chars[ch].textureCoord[3].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[3].x, pen.y + this->chars[ch].vertexCoord[3].y);
//			glTexCoord2f(this->chars[ch].textureCoord[2].x, this->chars[ch].textureCoord[2].y);
//			glVertex2f(pen.x + this->chars[ch].vertexCoord[2].x, pen.y + this->chars[ch].vertexCoord[2].y);
//			pen.x += this->chars[ch].width;
//		}
//		i++;
//	}
//}
//
//void Font2D::alignHorizontal(float x) {
//	this->alignH = x;
//}
//
//void Font2D::alignVertical(float x) {
//	this->alignV = x;
//}
//
//void Font2D::align(float horizontal, float vertical) {
//	this->alignH = horizontal;
//	this->alignV = vertical;
//}
