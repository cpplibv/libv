// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/font/font_2D.hpp>
// ext
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
// libv
#include <libv/math/vec.hpp>
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>
// std
#include <atomic>
// pro
#include <libv/ui/config.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

static FT_Library ftLib = nullptr; // <<< Mutex ?!
static std::atomic<size_t> FT_LibUseCount{0};

void incFreetypeLibRef() {
	if (!ftLib)
		if (const auto err = FT_Init_FreeType(&ftLib))
			LIBV_LOG_UI_FT_ERROR("FT_Init_FreeType failed: [%d]", err);
	++FT_LibUseCount;
}

void decFreetypeLibRef() {
	if (!--FT_LibUseCount)
		if (const auto err = FT_Done_FreeType(ftLib))
			LIBV_LOG_UI_FT_ERROR("FT_Done_FreeType failed: [%d]", err);
}

} // namespace

// -------------------------------------------------------------------------------------------------

Font2D::Font2D() {
	incFreetypeLibRef();
	textureData.fill(0);
}

Font2D::~Font2D() {
	LIBV_UI_DEBUG_ASSERT(face == nullptr);
	LIBV_UI_DEBUG_ASSERT(texture.id() == 0);
	decFreetypeLibRef();
}

// -------------------------------------------------------------------------------------------------

void Font2D::loadFont(const void* data, const size_t size) {
	fontData = std::string(static_cast<const char*> (data), size);
	if (const auto err = FT_New_Memory_Face(ftLib,
			reinterpret_cast<const FT_Byte*> (fontData.data()), fontData.size(), 0, &face))
		return LIBV_LOG_UI_FT_ERROR("FT_New_Memory_Face failed: [%d]", err);
	// TODO P5: FT error code to error message if possible.
	//			https://www.freetype.org/freetype2/docs/reference/ft2-error_code_values.html
	getCharacter(0, 12); // pre-fetch default char
}

void Font2D::unloadFont() {
	dirty = true;

	currentSize = 0;
	fontData.clear();
	FT_Done_Face(face);
	face = nullptr;

	texturePen = vec2ui();
	textureNextLine = 0;
	textureData.fill(0);

	characterInfos.clear();
}

// -------------------------------------------------------------------------------------------------

void Font2D::loadGL() {
	LIBV_UI_DEBUG_ASSERT(texture.id() == 0);

	texture.create();
	auto tBindGuard = texture.bindGuard();
	texture.setMagFilter(gl::MagFilter::Linear);
	texture.setMinFilter(gl::MinFilter::Linear);
	// TODO P4: validate Linear filters

	LIBV_GL_DEBUG_CHECK();
}

void Font2D::updateGL() {
	LIBV_UI_DEBUG_ASSERT(texture.id() != 0);
	LIBV_UI_DEBUG_ASSERT(face != nullptr);

	if (!dirty)
		return;
	updateTexture();
	dirty = false;

	LIBV_GL_DEBUG_CHECK();
}

void Font2D::unloadGL() {
	LIBV_UI_DEBUG_ASSERT(texture.id() != 0);

	texture.destroy();

	LIBV_GL_DEBUG_CHECK();
}

// -------------------------------------------------------------------------------------------------

int Font2D::getLineAdvance(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	changeSizeOnDemand(size);

	return face->size->metrics.height >> 6; // 26.6 format
}

int Font2D::getDescender(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	changeSizeOnDemand(size);

	return face->size->metrics.descender >> 6; // 26.6 format
}

vec2i Font2D::getKerning(uint32_t left, uint32_t right, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	changeSizeOnDemand(size);

	FT_Vector kerning;
	kerning.x = 0;
	kerning.y = 0;

	if (FT_HAS_KERNING(face)) {
		int leftID = FT_Get_Char_Index(face, left);
		int rightID = FT_Get_Char_Index(face, right);

		if (const auto err = FT_Get_Kerning(face, leftID, rightID, FT_KERNING_DEFAULT, &kerning))
			LIBV_LOG_UI_FT_ERROR("FT_Get_Kerning failed: [%d]", err);
	}

	return vec2i(kerning.x >> 6, kerning.y >> 6); // 26.6 float
}

const Font2D::Character& Font2D::getCharacter(uint32_t unicode, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);

	const auto infoIndex = CharacterIndex{unicode, size};
	auto it = characterInfos.find(infoIndex);
	if (it == characterInfos.end())
		it = characterInfos.emplace(infoIndex, renderCharacter(unicode, size)).first;
	return it->second;
}

Font2D::Character Font2D::renderCharacter(uint32_t unicode, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);

	changeSizeOnDemand(size);

	FT_Glyph glyph = nullptr;

	int charIndex = FT_Get_Char_Index(face, unicode);

	if (const auto err = FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT))
		LIBV_LOG_UI_FT_ERROR("FT_Load_Glyph failed: [%d]", err);
	if (const auto err = FT_Get_Glyph(face->glyph, &glyph))
		LIBV_LOG_UI_FT_ERROR("FT_Get_Glyph failed: [%d]", err);
	if (const auto err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true))
		LIBV_LOG_UI_FT_ERROR("FT_Glyph_To_Bitmap failed: [%d]", err);

	const auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph> (glyph);
	const auto bitmap = bitmapGlyph->bitmap;
	const auto bitmapWidth = int64_t{bitmap.width};
	const auto bitmapHeight = int64_t{bitmap.rows};

	Character result;
	result.advance.x = glyph->advance.x / 65536.0f; // 16.16 float
	result.advance.y = glyph->advance.y / 65536.0f; // 16.16 float

	if (texturePen.x + bitmapWidth > textureSize.x) // Warp to new line
		texturePen = vec2ui(0, textureNextLine + 1); // Leaves 1 px gap between lines

	if (texturePen.y + bitmapHeight > textureSize.y) { // Detect 'full' texture
		LIBV_LOG_UI_FT_ERROR("Failed to render character: Not enough space in font texture: "
				"unicode: [%d] size: [%dpx] bitmap size: [%d,%d], pen pos: [%d,%d], texture id [%d]",
				unicode, currentSize, bitmapWidth, bitmapHeight, texturePen.x, texturePen.y, texture.id());
		FT_Done_Glyph(glyph);
		return getCharacter(0, 12); // Fallback to pre-fetched default char
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
	result.textureCoord[0] = vec2f(tx0, ty0);
	result.textureCoord[1] = vec2f(tx1, ty0);
	result.textureCoord[2] = vec2f(tx1, ty1);
	result.textureCoord[3] = vec2f(tx0, ty1);
	// <<< validate texture and pixel coord if they are ok (0.5ness)

	const auto vx0 = bitmapGlyph->left;
	const auto vx1 = bitmapGlyph->left + bitmapWidth;
	const auto vy0 = bitmapGlyph->top - bitmapHeight;
	const auto vy1 = bitmapGlyph->top;

	result.vertexCoord[0] = vec2f(vx0, vy0);
	result.vertexCoord[1] = vec2f(vx1, vy0);
	result.vertexCoord[2] = vec2f(vx1, vy1);
	result.vertexCoord[3] = vec2f(vx0, vy1);

	texturePen.x += bitmapWidth + 1; // Progress pen. Leaves 1 px gap between chars
	FT_Done_Glyph(glyph);

	dirty = true;
	return result;
}

void Font2D::updateTexture() {
	auto tBindGuard = texture.bindGuard();
	texture.image2D(0, gl::InternalFormat::Luminance8, gl::Format::RED, gl::DataType::UnsignedByte,
			textureSize.x, textureSize.y, textureData.data());
	LIBV_GL_DEBUG_CHECK();
}

void Font2D::changeSizeOnDemand(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	if (currentSize != size) {
		currentSize = size;
		// FT_Set_Char_Size uses 26.6 float
		if (const auto err = FT_Set_Char_Size(face, currentSize << 6, currentSize << 6, 96, 96))
			LIBV_LOG_UI_FT_ERROR("FT_Set_Char_Size failed: [%d]", err);
	}
}

void Font2D::bind() {
	texture.bind();
}

gl::BindGuard<Font2D> Font2D::bindGuard() {
	return gl::BindGuard<Font2D>(*this);
}

void Font2D::unbind() {
	texture.unbind();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv


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
