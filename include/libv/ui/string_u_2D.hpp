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

class StringU2D {
private:
	libv::glr::Mesh mesh_{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	std::shared_ptr<Font2D> font;

	FontSize fontSize = FontSize{12};
	bool dirty = true;
	AlignHorizontal align = AlignHorizontal::Center;
	libv::vec2f limit;

	std::vector<uint32_t> codepoints;
//	std::vector<libv::vec2f> positions;
//	std::vector<libv::vec2f> lines;
//	std::vector<uint32_t> lines;
//	std::vector<uint32_t> words;

public:
	StringU2D();

public:
	void setAlign(const AlignHorizontal align);
	void setFont(std::shared_ptr<Font2D> font, const FontSize fontSize);
	void setSize(const FontSize fontSize);
	void setString(std::string string);
	void setLimit(const libv::vec2f limit);
	inline void setLimit(const float x, const float y) { setLimit({x, y}); }

public:
	void push_back(const uint32_t unicode);
	void push_back(const std::string_view string_);
	void pop_back();

public:
//	inline const std::string& getString() const {
//		return string;
//	}

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
