// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/font_2D.hpp>
// ext
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
// libv
#include <libv/glr/queue.hpp>
#include <libv/math/vec.hpp>
// std
#include <atomic>
#include <mutex>
// pro
#include <libv/ui/log.hpp>


#define LIBV_UI_DEBUG_ASSERT(X) assert((X))

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

FT_Library freetype_lib = nullptr;
std::atomic<size_t> FT_LibUseCount{0};
std::mutex freetype_lib_m;

// - One can use a single `FT_Library' object across threads as long as a mutex lock is used around
//   `FT_New_Face' and `FT_Done_Face'.
// - Any calls to `FT_Load_Glyph' and similar API are safe and do not need the lock to be held
//   as long as the same `FT_Face' is not used from multiple threads at the same time

void _incFreetypeLibRef() {
	if (!freetype_lib)
		if (const auto err = FT_Init_FreeType(&freetype_lib))
			log_ui_ft.error("FT_Init_FreeType failed: {}", err);
	++FT_LibUseCount;
}

void _decFreetypeLibRef() {
	if (!--FT_LibUseCount)
		if (const auto err = FT_Done_FreeType(freetype_lib))
			log_ui_ft.error("FT_Done_FreeType failed: {}", err);
}

} // namespace

// -------------------------------------------------------------------------------------------------

Font2D::Font2D(std::string fontData) :
	fontData(std::move(fontData)) {

	{
		std::lock_guard lock(freetype_lib_m);
		_incFreetypeLibRef();
		if (const auto err = FT_New_Memory_Face(
				freetype_lib,
				reinterpret_cast<const FT_Byte*>(this->fontData.data()),
				static_cast<FT_Long>(this->fontData.size()),
				0,
				&face))
			log_ui_ft.error("FT_New_Memory_Face failed: {}", err);
		// TODO P5: FT error code to error message if possible.
		//			https://www.freetype.org/freetype2/docs/reference/ft2-error_code_values.html
	}

	texture_.storage(1, textureSize);
	texture_.set(gl::MagFilter::Nearest);
	texture_.set(gl::MinFilter::Nearest);

	_renderFallbackCharacter(); // pre-fetch default char
}

Font2D::~Font2D() {
	std::lock_guard lock(freetype_lib_m);
	FT_Done_Face(face);
	_decFreetypeLibRef();
}

// -------------------------------------------------------------------------------------------------

float Font2D::getLineAdvance(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	std::lock_guard lock(face_m);
	_changeSizeOnDemand(size);

	return static_cast<float>(face->size->metrics.height) / 64.0f; // 26.6 fixed point
}

float Font2D::getAscender(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	std::lock_guard lock(face_m);
	_changeSizeOnDemand(size);

	return static_cast<float>(face->size->metrics.ascender) / 64.0f; // 26.6 fixed point
}

float Font2D::getDescender(uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	std::lock_guard lock(face_m);
	_changeSizeOnDemand(size);

	return static_cast<float>(face->size->metrics.descender) / 64.0f; // 26.6 fixed point
}

vec2f Font2D::getKerning(uint32_t left, uint32_t right, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	std::lock_guard lock(face_m);
	_changeSizeOnDemand(size);

	FT_Vector kerning;
	kerning.x = 0;
	kerning.y = 0;

	if (FT_HAS_KERNING(face)) {
		int leftID = FT_Get_Char_Index(face, left);
		int rightID = FT_Get_Char_Index(face, right);

		if (const auto err = FT_Get_Kerning(face, leftID, rightID, FT_KERNING_DEFAULT, &kerning))
			log_ui_ft.error("FT_Get_Kerning failed: {}", err);
	}

	// 26.6 fixed point
	return {static_cast<float>(kerning.x) / 64.0f, static_cast<float>(kerning.y) / 64.0f};
}

const Font2D::Character& Font2D::getCharacter(uint32_t unicode, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);
	std::lock_guard lock(face_m);

	return _getCharacter(unicode, size);
}

const Font2D::Character& Font2D::_getCharacter(uint32_t unicode, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);

	const auto infoIndex = CharacterIndex{unicode, size};
	auto it = characterInfos.find(infoIndex);
	if (it == characterInfos.end())
		it = characterInfos.emplace(infoIndex, _renderCharacter(unicode, size)).first;
	return it->second;
}

Font2D::Character Font2D::_renderFallbackCharacter() {
	return _renderCharacter(0, 12);
}

Font2D::Character Font2D::_renderCharacter(uint32_t unicode, uint32_t size) {
	LIBV_UI_DEBUG_ASSERT(face != nullptr);

	_changeSizeOnDemand(size);

	FT_Glyph glyph = nullptr;

	int charIndex = FT_Get_Char_Index(face, unicode);

	if (const auto err = FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT)) {
		log_ui_ft.error("FT_Load_Glyph failed: {} for: {} size: {}", err, unicode, size);
		return _renderFallbackCharacter();
	}
	if (const auto err = FT_Get_Glyph(face->glyph, &glyph)) {
		log_ui_ft.error("FT_Get_Glyph failed: {} for: {} size: {}", err, unicode, size);
		return _renderFallbackCharacter();
	}
	if (const auto err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_LCD, nullptr, true)) {
		log_ui_ft.error("FT_Glyph_To_Bitmap failed: {} for: {} size: {}", err, unicode, size);
		return _renderFallbackCharacter();
	}

	const auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
	const auto& bitmap = bitmapGlyph->bitmap;
	const auto bitmapWidth = static_cast<int32_t>(bitmap.width) / 3; // 3 component LCD
	const auto bitmapHeight = static_cast<int32_t>(bitmap.rows);

	if (texturePen.x + bitmapWidth > textureSize.x) // Warp to new line
		texturePen = vec2i(1, textureNextLine + 1); // Leaves 1 px gap between lines

	if (texturePen.y + bitmapHeight > textureSize.y) { // Detect 'full' texture
		log_ui_ft.error("Failed to render character: Not enough space in font texture: "
				"unicode: {} size: {} bitmap size: {} {}, pen pos: {}",
				unicode, currentSize, bitmapWidth, bitmapHeight, texturePen);
		FT_Done_Glyph(glyph);
		return _renderFallbackCharacter();
	}

	if (texturePen.y + bitmapHeight > textureNextLine) // Expand current line
		textureNextLine = texturePen.y + bitmapHeight;

	for (int32_t y = 0; y < bitmapHeight; y++) {
		// Freetypes bitmap is upside down, flipping during copy
		texture_.image(0,
				texturePen + libv::vec2i(0, bitmapHeight - y - 1),
				{bitmapWidth, 1},
				reinterpret_cast<const libv::vec3uc*>(bitmap.buffer + y * bitmap.pitch));
	}

	Character result;
	result.advance.x = static_cast<float>(glyph->advance.x) / 65536.0f; // 16.16 fixed point
	result.advance.y = static_cast<float>(glyph->advance.y) / 65536.0f; // 16.16 fixed point

	result.bearing_left = static_cast<float>(bitmapGlyph->left);
	result.bearing_right = result.advance.x - result.bearing_left - static_cast<float>(bitmapWidth);
	result.bearing_top = static_cast<float>(bitmapGlyph->top);
	result.bearing_bottom = result.advance.y - result.bearing_top - static_cast<float>(bitmapHeight);

	result.tex[0].x = result.tex[3].x = static_cast<float>(texturePen.x);
	result.tex[1].x = result.tex[2].x = static_cast<float>(texturePen.x + bitmapWidth);
	result.tex[0].y = result.tex[1].y = static_cast<float>(texturePen.y);
	result.tex[2].y = result.tex[3].y = static_cast<float>(texturePen.y + bitmapHeight);

	result.pos[0].x = result.pos[3].x = static_cast<float>(bitmapGlyph->left);
	result.pos[1].x = result.pos[2].x = static_cast<float>(bitmapGlyph->left + bitmapWidth);
	result.pos[0].y = result.pos[1].y = static_cast<float>(bitmapGlyph->top - bitmapHeight);
	result.pos[2].y = result.pos[3].y = static_cast<float>(bitmapGlyph->top);

	texturePen.x += bitmapWidth + 1; // Progress pen. Leaves 1 px gap between chars
	FT_Done_Glyph(glyph);

	return result;
}

void Font2D::_changeSizeOnDemand(uint32_t size) {
	if (currentSize != size) {
		// 26.6 fixed point
		if (const auto err = FT_Set_Char_Size(face, size << 6, size << 6, 96, 96))
			log_ui_ft.error("FT_Set_Char_Size failed: {}", err);
		else
			currentSize = size;
	}
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
//                    |          .                |
//                    |          .                |
// textureNextLine -> |          .                |
//                    |#  ## ## #+--+.  .  .  .  .| A
//                    |##### ## #|  |             | | bitmapSize.y
//    texturePen.y -> |##########+--+.  .  .  .  .| V
//                    |###   ########  #### ##  ##|
//                    |####  ############## ######|
//                    |###########################|
//                   -+---------------------------+-> X+
//                0,0 |          <-->
//                         bitmapSize.x





// Outline =========================================================================================

// https://github.com/rougier/freetype-gl/blob/master/demos/outline.c

//	// initialize stroker, so you can create outline font
//	FT_Stroker stroker;
//	FT_Stroker_New(library, &stroker);
//	//  2 * 64 result in 2px outline // 24.8 float
//	FT_Stroker_Set(stroker, 2 * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
//	...
//	// generation of an outline for single glyph:
//	FT_UInt glyphIndex = FT_Get_Char_Index(face, glyphId);
//	FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
//	FT_Glyph glyph;
//	FT_Get_Glyph(face->glyph, &glyph);
//	FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
//	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
//	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
//	// blit the glyph here on your target surface.
//	// For positioning use bitmapGlyph->left, bitmapGlyph->top
//	// For iteration over the glyph data use bitmapGlyph->bitmap.buffer, bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows, bitmapGlyph->bitmap.pitch.
//	// ...
//	FT_Stroker_Done(stroker);
