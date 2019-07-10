// File:   font.hpp Author: Vader Created on 2019. január 27., 4:25

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/texture.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// std
#include <array>
#include <mutex>
#include <string>


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

// TODO P3: Dynamic textureSize and resize, TextureRect might be useful to avoid uniforms
// TODO P3: Allow access of font faces other than zero (currently burnt in to FT_New_Memory_Face)
// TODO P4: enum class Weight { Bold, Normal };
// TODO P4: Freetype error handling (Check return locations) for memory leaks (?)
//			use unique_ptr with custom deleter
// TODO P5: Slant slant = Slant::Normal;
// TODO P5: Improved texture usage algorithm (and there is a way to reallocate a new bigger texture)
//			use pixel space texture coords end set uniforms for vertex shader
// TODO P5: Gamma 1.8 + stem darkening in freetype: https://www.freetype.org/freetype2/docs/text-rendering-general.html

// -------------------------------------------------------------------------------------------------

/// Font2D is responsibly to providing char information, glyph texture (and divisor in the future).
/// Font2D is NOT responsibly to rendering, aligning, laying out or shading.
/// @context ANY
/// @threadsafety single threaded
class Font2D {
public:
	struct Character {
		libv::vec2f pos[4];
		libv::vec2f tex[4];
		libv::vec2f advance; // The pen offset for the next character position
		float bearing_left = 0;
		float bearing_right = 0;
		float bearing_top = 0;
		float bearing_bottom = 0;
	};

private:
	struct CharacterIndex {
		uint32_t unicode;
		uint32_t pxSize;
		bool operator<(const CharacterIndex& rhs) const {
			return std::tie(unicode, pxSize) < std::tie(rhs.unicode, rhs.pxSize);
		}
	};

private:
	libv::glr::Texture2D::R8_G8_B8 texture;

private:
	std::mutex face_m;
	FT_Face face = nullptr;
	uint32_t currentSize = 0;
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
	Font2D(std::string fontData);
	~Font2D();

public:
	void bind(libv::glr::Queue& gl, libv::gl::TextureChannel channel);

public:
	float getLineAdvance(uint32_t size);
	float getAscender(uint32_t size);
	float getDescender(uint32_t size);
	vec2f getKerning(uint32_t left, uint32_t right, uint32_t size);
	const Character& getCharacter(uint32_t unicode, uint32_t size);

private:
	Character _renderCharacter(uint32_t unicode, uint32_t size);
	void _changeSizeOnDemand(uint32_t size);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
