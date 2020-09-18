// File: string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/math/vec.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;

class String2D {
private:
	libv::glr::Mesh mesh_{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	std::shared_ptr<Font2D> font_;

	FontSize fontSize_ = FontSize{12};
	bool dirty = true;
	AlignHorizontal align_horizontal_ = AlignHorizontal::center;
	AlignVertical align_vertical_ = AlignVertical::center;
	libv::vec2f limit_;

	std::string string_;

public:
	void align_horizontal(const AlignHorizontal align);
	void align_vertical(const AlignVertical align);
	void font(std::shared_ptr<Font2D> font);
	void font(std::shared_ptr<Font2D> font, const FontSize fontSize);
	void size(const FontSize fontSize);
	void string(std::string string);
	void limit(const libv::vec2f limit);
	inline void limit(const float x, const float y) { limit({x, y}); }

public:
	[[nodiscard]] inline AlignHorizontal align_horizontal() const noexcept {
		return align_horizontal_;
	}
	[[nodiscard]] inline AlignVertical align_vertical() const noexcept {
		return align_vertical_;
	}
	[[nodiscard]] inline const std::string& string() const noexcept {
		return string_;
	}
	[[nodiscard]] inline const std::shared_ptr<Font2D>& font() const noexcept {
		return font_;
	}
	[[nodiscard]] inline FontSize size() const noexcept {
		return fontSize_;
	}

public:
	[[nodiscard]] size_t length() const noexcept;

public:
	void insert(size_t position, uint32_t unicode);
	/// @return the number of codepoints inserted
	size_t insert(size_t position, const std::string_view string_);
	void erase(size_t position, size_t count = 1);

	void push_back(uint32_t unicode);
	void push_back(const std::string_view string_);
	void pop_back();

	void clear();

public:
	[[nodiscard]] const libv::glr::Mesh& mesh();

public:
	[[nodiscard]] libv::vec2f getCharacterPosition();
	[[nodiscard]] libv::vec2f getCharacterPosition(size_t characterIndex);
//	size_t getLineOfCharacter();
//	size_t getLineOfCharacter(size_t characterIndex);
//	libv::vec2f getLinePosition();
//	libv::vec2f getLinePosition(size_t lineIndex);

	[[nodiscard]] size_t getClosestCharacterIndex(libv::vec2f position);
	[[nodiscard]] size_t getClosestCharacterIndexInline(libv::vec2f position);
//	size_t getClosestLineIndex(libv::vec2f position);

	/// @param limit - The maximum available space that can be used to layout
	/// @note Negative \c limit values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	[[nodiscard]] libv::vec2f content(libv::vec2f limit);

	/// @param x - The maximum available width that can be used to layout
	/// @param y - The maximum available height that can be used to layout
	/// @note Negative \c x and \c y values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	[[nodiscard]] inline libv::vec2f content(const float x, const float y) {
		return content({x, y});
	}

private:
	void layout();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
