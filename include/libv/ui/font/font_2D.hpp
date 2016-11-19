// File: Font2D.hpp, Created on 2014. november 30. 14:32, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/gl/guard.hpp>
#include <libv/gl/texture.hpp>
#include <libv/vec.hpp>
// std
#include <array>
#include <string>

// Freetype2 forward declarations ------------------------------------------------------------------

struct FT_FaceRec_;
using FT_Face = FT_FaceRec_*;

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// Terminology:
// font family: arial
// font face: arial italic
// advance: distance between origin points
// kering: difference in advance depending on the next/previous char

// TODO P3: Font2D formated version which is btw more than font, its String2D
// TODO P3: Font size by dpi and pt (only set size and indexing is the problem)
// TODO P3: Allow access of font faces other than zero (currently burnt in to FT_New_Memory_Face)
// TODO P4: enum class Weight { Bold, Normal };
// TODO P4: Vertical lines
// TODO P4: Freetype error handling (Check return locations) for memory leaks (?)
// TODO P5: Slant slant = Slant::Normal;
// TODO P5: Improved texture usage algorithm (and there is a way to reallocate a new bigger texture)
//		either use pixel space texture coords end set uniforms for shader and in the shader recalc tx
//		or observing dependents and invalidating them... first method preferred?
// TODO P5: Ctor with std::array_view
// TODO P5: default font texture size into nice constexpr variable
// TODO PLog: Log group libv-ui-freetype for freetype err msgs

constexpr const size_t DEFAULT_FONT2D_TEXTURE_WIDTH = 512;
constexpr const size_t DEFAULT_FONT2D_TEXTURE_HEIGHT = 512;

// -------------------------------------------------------------------------------------------------

//class ShaderFont2D {
//	libv::gl::ShaderNC fs;
//	libv::gl::ShaderNC vs;
//	libv::gl::ProgramNC program;
//
//	libv::gl::Uniform<glm::mat4> MVPmat;
//	// Uniform MVPmat
//	// Uniform Color
//};

// -------------------------------------------------------------------------------------------------

/**
 * Font2D is a primitive wrapper.
 * Loading, unloading and updating has multiple steps, manual and thread sensitive.
 * Font2D is responsibly to providing char information, glyph texture (and divisor in the future).
 * Font2D is NOT responsibly to rendering, aligning, layouting or shading.
 * @context GL / ANY
 * @threadsafety single threaded
 */
class Font2D {
public:
	struct Character {
		vec2 vertexCoord[4];
		vec2 textureCoord[4];
		vec2 advance; // The pen offset for the next character position
	};

private:
	struct CharacterIndex {
		uint32_t unicode;
		uint32_t pxSize;
		bool operator<(const CharacterIndex& rhs) const {
			return unicode < rhs.unicode || (unicode == rhs.unicode && pxSize < rhs.pxSize);
		}
	};

private:
	bool dirty = true;
	gl::Texture2D texture;

private:
	uint32_t currentSize;
	std::string fontData;
	FT_Face face = nullptr;

private:
	uivec2 textureSize{DEFAULT_FONT2D_TEXTURE_WIDTH, DEFAULT_FONT2D_TEXTURE_HEIGHT};
	uivec2 texturePen{0, 0};
	uint32_t textureNextLine = 0;
	std::array<GLubyte, DEFAULT_FONT2D_TEXTURE_WIDTH * DEFAULT_FONT2D_TEXTURE_HEIGHT> textureData;

private:
	boost::container::flat_map<CharacterIndex, Character> characterInfos;

public:
	/**
	 * @context ANY
	 */
	Font2D();
	/**
	 * @context ANY - Can only be called from ANY context when the object is unloaded.
	 */
	~Font2D();

	/**
	 * @param data beginning of the font file's content
	 * @param size size of the font file
	 * @note the data will be copied into an internal storage,
	 * deallocation of data is possible after the function return
	 * @context ANY
	 */
	void loadFont(const void* data, const size_t size);
	/**
	 * @context ANY
	 */
	void unloadFont();

	/**
	 * @context GL
	 */
	void loadGL();
	/**
	 * @context GL
	 */
	void updateGL();
	/**
	 * @context GL
	 */
	void unloadGL();

	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	int getLineAdvance(uint32_t size);
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	int getDescender(uint32_t size);
	/**
	 * @context ANY
	 */
	ivec2 getKerning(uint32_t left, uint32_t right, uint32_t size);
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	const Character& getCharacter(uint32_t unicode, uint32_t size);

	/** Bind the font's texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	void bind();
	/** Bind the font's texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	gl::BindGuard<Font2D> bindGuard();
	/** Unbind the font's shader and texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	void unbind();

private:
	Character renderCharacter(uint32_t unicode, uint32_t size);
	void updateTexture();
	void changeSizeOnDemand(uint32_t size);
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv