// File: string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/math/vec.hpp>
// std
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;

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

class String2D {
private:
	libv::glr::Mesh mesh_{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	std::shared_ptr<Font2D> font;

	FontSize fontSize = FontSize{12};
	bool dirty = true;
	AlignHorizontal align = AlignHorizontal::Center;
	libv::vec2f limit;

	std::string string;

public:
	String2D();

public:
	void setAlign(const AlignHorizontal align);
	void setFont(std::shared_ptr<Font2D> font);
	void setFont(std::shared_ptr<Font2D> font, const FontSize fontSize);
	void setSize(const FontSize fontSize);
	void setString(std::string string);
	void setLimit(const libv::vec2f limit);
	inline void setLimit(const float x, const float y) { setLimit({x, y}); }

public:
	void push_back(uint32_t unicode);
	void push_back(const std::string_view string_);
	void pop_back();

public:
	inline AlignHorizontal getAlign() const noexcept {
		return align;
	}

	inline const std::string& getString() const noexcept {
		return string;
	}

	inline const std::shared_ptr<Font2D>& getFont() const noexcept {
		return font;
	}

	inline FontSize getSize() const noexcept {
		return fontSize;
	}

	/// @param limit - The maximum available space that can be used to layout
	/// @note Negative \c limit values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	libv::vec2f getContent(libv::vec2f limit);

	/// @param x - The maximum available width that can be used to layout
	/// @param y - The maximum available height that can be used to layout
	/// @note Negative \c x and \c y values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	inline libv::vec2f getContent(const float x, const float y) {
		return getContent({x, y});
	}

public:
	const libv::glr::Mesh& mesh();

public:
	libv::vec2f getCharacterPosition();
	libv::vec2f getCharacterPosition(size_t characterIndex);
//	libv::vec2f getLinePosition();
//	libv::vec2f getLinePosition(size_t lineIndex);
//
//	size_t getClosestCharacterIndex(libv::vec2f position);
//	size_t getClosestLineIndex(libv::vec2f position);

private:
	void layout();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
