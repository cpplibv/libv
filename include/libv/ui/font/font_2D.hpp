// File: Font2D.hpp, Created on 2014. november 30. 14:32, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/gl/shader.hpp>
#include <libv/gl/types.hpp>
#include <libv/gl/uniform.hpp>
#include <libv/vec.hpp>
// std
#include <array>
#include <string>

// Freetype2 forward declarations ------------------------------------------------------------------

struct FT_FaceRec_;
using FT_Face = FT_FaceRec_*;
struct FT_SizeRec_;
using FT_Size = FT_SizeRec_*;

// Boost forward declarations ------------------------------------------------------------------

namespace boost {
namespace filesystem {
class path;
} //namespace filesystem
} //namespace boost

// -------------------------------------------------------------------------------------------------

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

class ShaderFont2D {
	libv::gl::ShaderProgram shaderProgram;
	libv::gl::Uniform<glm::mat4> MVPmat;
	// Shader
	// Uniform MVPmat
	// Uniform Color
};

// -------------------------------------------------------------------------------------------------

/**
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
		unsigned int pxSize;
		bool operator<(const CharacterIndex& rhs) const {
			return unicode < rhs.unicode || (unicode == rhs.unicode && pxSize < rhs.pxSize);
		}
	};
private:
	GLuint textureID = 0;
	unsigned int activeFontSize = 12; // in px
	bool dirty = true;

private:
	FT_Face face = nullptr;
	std::string fileContent;
	boost::container::flat_map<int, FT_Size> sizes;

private:
	boost::container::flat_map<CharacterIndex, Character> characterInfos;
	Character defaultCharacter;
	uivec2 textureSize{DEFAULT_FONT2D_TEXTURE_WIDTH, DEFAULT_FONT2D_TEXTURE_HEIGHT};
	uivec2 texturePen{0, 0};
	unsigned int textureNextLine = 0;
	std::array<GLubyte, DEFAULT_FONT2D_TEXTURE_WIDTH * DEFAULT_FONT2D_TEXTURE_HEIGHT> textureData;

public:
	/**
	 * @context ANY
	 */
	Font2D();
	/**
	 * @context GL
	 */
	Font2D(const boost::filesystem::path& filePath);
	/**
	 * @param data begining of the font file's content
	 * @param size size of the font file
	 * @note the data will be copied into an internal storage,
	 * deallocation of data is possible after the function return
	 * @context GL
	 */
	Font2D(const void* data, const size_t size);
	/**
	 * @context GL / ANY - Can only be called from ANY context when the object is not loaded.
	 * (For example after a successful unload member function call)
	 */
	~Font2D();

	/**
	 * @context ANY
	 */
	void load(const boost::filesystem::path& filePath);
	/**
	 * @param data begining of the font file's content
	 * @param size size of the font file
	 * @note the data will be copied into an internal storage,
	 * deallocation of data is possible after the function return
	 * @context ANY
	 */
	void load(const void* data, const size_t size);

	/**
	 * @context GL
	 */
	void unload();

	bool isLoaded() const;
	/** Set the active font size
	 * @param px size in pixel
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	void setSize(int px);
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	int getLineAdvance() const;
	ivec2 getKerning(uint32_t left, uint32_t right) const;
	/**
	 * @state This operation requires loaded state
	 * @context GL
	 */
	const Character& getCharacter(uint32_t unicode);
	/** Bind the font's shader and texture to the openGL state machine
	 * @prerequirment GL_TEXTURE_2D has to be enabled
	 * @state This operation requires loaded state
	 * @context GL
	 */
	void bind();
	/** Unbind the font's shader and texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	void unbind();

private:
	void loadFace();
	Character renderCharacter(uint32_t unicode);
	void uploadTexture();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv