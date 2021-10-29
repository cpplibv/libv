// Project: libv.ui, File: src/libv/ui/font_2D.hpp

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/glr/texture.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
//#include <libv/glr/fwd.hpp>
// std
#include <array>
#include <mutex>
#include <string>
// pro
#include <libv/ui/property/font_size.hpp>


// -------------------------------------------------------------------------------------------------

struct FT_FaceRec_;
using FT_Face = FT_FaceRec_*;

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// Terminology:
// font family: arial
// font face: arial italic
// advance: distance between origin points
// kering: difference in advance depending on the current/previous char

// TODO P2: Freetype error handling (Check return locations) for memory leaks (?)
//			use unique_ptr with custom deleter
// TODO P3: Allow access of font faces other than zero (currently burnt in to FT_New_Memory_Face)
// TODO P4: enum class Weight { Bold, Normal };
// TODO P5: Slant slant = Slant::Normal;
// TODO P5: Improved texture usage algorithm (texture size reallocation is now possible) (Question: could it be stored as a one dimensional data? yes could mean no wasted space)
// TODO P5: Gamma 1.8 + stem darkening in freetype: https://www.freetype.org/freetype2/docs/text-rendering-general.html

// -------------------------------------------------------------------------------------------------

//	struct Font {
//		FontEffect effect;   /// normal / outline / underline / strike-through ...
//		FontFamily family;   /// font-family: Consolas
//		FontSize size = 12;  /// font-size/line-height
//		FontStretch stretch; /// font-stretch - normal / ultra-condensed / extra-condensed / condensed / semi-condensed / semi-expanded / expanded / extra-expanded / ultra-expanded
//		FontStyle style;     /// font-style - normal / italic / oblique
//		FontVariant variant; /// font-variant - normal / small-caps
//		FontWeight weight;   /// font-weight - normal / bold / bolder / lighter / %number%
//
//		FontFamily family;   /// font-family: Consolas
//		uint64_t underline:1;
//		uint64_t strike_through:1;
//		uint64_t small_caps:1;
//		uint64_t italic:1;
//		uint64_t stretch:8; // 4.4 fixed point
//		uint64_t outline:12; // 8.4 fixed point
//		uint64_t weight:16; // 8.8 fixed point
//		uint64_t size:8;
//	};

// -------------------------------------------------------------------------------------------------

/// Font2D is responsibly to providing character information, glyph texture
/// Font2D is NOT responsibly to rendering, aligning, laying out or shading.
/// @context ANY
/// @threadsafety single threaded
class Font2D {
public:
	struct Character {
		libv::vec2f pos[4]; /// Order: bottom-left, bottom-right, top-right, top-left
		libv::vec2f tex[4]; /// Texture coordinates in TEXEL-SPACE (and not in normalized texture space)
		libv::vec2f advance; /// The pen offset for the next character position
		float bearing_left = 0;
		float bearing_right = 0;
		float bearing_top = 0;
		float bearing_bottom = 0;
	};

private:
	struct CharacterIndex {
		uint32_t unicode;
		FontSize pxSize;
		bool operator<(const CharacterIndex& rhs) const {
			return std::tie(unicode, pxSize) < std::tie(rhs.unicode, rhs.pxSize);
		}
	};

private:
	libv::glr::Texture2D::R8_G8_B8 texture_;

private:
	std::mutex face_m;
	FT_Face face = nullptr;
	FontSize currentSize = FontSize{0};
	std::string fontData;
	// TODO P2: unique_ptr for FT_Face

private:
	libv::vec2i textureSize{1024, 1024};
	libv::vec2i texturePen{1, 1}; // 1 px gap from border
	int32_t textureNextLine = 0;

private:
	boost::container::flat_map<CharacterIndex, Character> characterInfos;

public:
	/// @param fontData the content of the font file
	explicit Font2D(std::string fontData);
	~Font2D();

public:
	const libv::glr::Texture2D::R8_G8_B8& texture() const {
		return texture_;
	}

public:
	float getLineAdvance(FontSize size); /// Offset between lines, Yields the offset to the next lines first pixel
	float getLineHeight(FontSize size); /// Height of a single line, Yields the offset to the current lines last pixel
	float getAscender(FontSize size);
	float getDescender(FontSize size);
	vec2f getKerning(uint32_t left, uint32_t right, FontSize size);
	const Character& getCharacter(uint32_t unicode, FontSize size);

private:
	const Character& _getCharacter(uint32_t unicode, FontSize size);
	Character _getFallbackCharacter();
	Character _renderCharacter(uint32_t unicode, FontSize size);
	void _changeSizeOnDemand(FontSize size);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
