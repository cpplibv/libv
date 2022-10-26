// Project: libv.ui, File: src/libv/ui/component/layout/layout_text.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
//#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;

// -------------------------------------------------------------------------------------------------

struct LayoutTextData {
	std::vector<libv::vec3f> positions;
	std::vector<libv::vec2f> texture0s;
	std::vector<uint32_t> indices;
};

// -------------------------------------------------------------------------------------------------

class LayoutText : LayoutTextData {
private:
	std::shared_ptr<Font2D> font_;

protected:
	bool dirty = true;
private:
	FontSize fontSize_ = FontSize{12};
	AlignHorizontal align_horizontal_ = AlignHorizontal::center;
	AlignVertical align_vertical_ = AlignVertical::center;
	libv::vec2f limit_;

	std::string string_;

	libv::vec2f content_bounding_pos_;
	libv::vec2f content_bounding_size_;

public:
	LayoutText() = default;
	LayoutText(std::shared_ptr<Font2D> font, FontSize fontSize, AlignHorizontal alignH, AlignVertical alignV, std::string string) :
		font_(std::move(font)), fontSize_(fontSize), align_horizontal_(alignH), align_vertical_(alignV), string_(std::move(string)) {}

public:
	void align_horizontal(const AlignHorizontal align);
	void align_vertical(const AlignVertical align);
	void font(std::shared_ptr<Font2D> font);
	void font(std::shared_ptr<Font2D> font, const FontSize fontSize);
	void size(const FontSize fontSize);
	void string(std::string string);
	void limit(const libv::vec2f limit);
	inline void limit(const float x, const float y) { limit(libv::vec2f(x, y)); }

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
	[[nodiscard]] std::size_t length() const noexcept;

public:
	void insert(std::size_t position, uint32_t unicode);
	/// @return the number of codepoints inserted
	std::size_t insert(std::size_t position, const std::string_view string_);
	void erase(std::size_t position, std::size_t count = 1);

	void push_back(uint32_t unicode);
	void push_back(const std::string_view string_);
	void pop_back();

	void clear();

public:
	[[nodiscard]] const LayoutTextData& vertices_data();

public:
	[[nodiscard]] libv::vec2f getCharacterPosition();
	[[nodiscard]] libv::vec2f getCharacterPosition(std::size_t characterIndex);
//	std::size_t getLineOfCharacter();
//	std::size_t getLineOfCharacter(std::size_t characterIndex);
//	libv::vec2f getLinePosition();
//	libv::vec2f getLinePosition(std::size_t lineIndex);

	[[nodiscard]] std::size_t getClosestCharacterIndex(libv::vec2f position);
	[[nodiscard]] std::size_t getClosestCharacterIndexInline(libv::vec2f position);
//	std::size_t getClosestLineIndex(libv::vec2f position);

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

	[[nodiscard]] inline libv::vec2f content_bounding_pos() {
		if (dirty)
			layout();
		return content_bounding_pos_;
	}
	[[nodiscard]] inline libv::vec2f content_bounding_size() {
		if (dirty)
			layout();
		return content_bounding_size_;
	}

private:
	void layout();
};

// -------------------------------------------------------------------------------------------------

// =================================================================================================
// =================================================================================================
// =================================================================================================

struct LayoutTextLite : LayoutText {
private:
	using LayoutText::content;
	using LayoutText::limit;

public:
	void invalidateLayout2(const libv::vec2f limit_) {
		dirty = true;
		limit(limit_);
	}

	[[nodiscard]] const libv::ui::LayoutTextData& vertices_data(
			const std::shared_ptr<Font2D>& font_,
			FontSize size_,
			AlignHorizontal align_horizontal_,
			AlignVertical align_vertical_) {

		font(font_);
		size(size_);
		align_horizontal(align_horizontal_);
		align_vertical(align_vertical_);

		return LayoutText::vertices_data();
	}

	[[nodiscard]] libv::vec2f measure_content_size(
			libv::vec2f probe_limit,
			const std::shared_ptr<Font2D>& font_,
			FontSize size_) {

		font(font_);
		size(size_);

		return LayoutText::content(probe_limit);
	}
};

// =================================================================================================
// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
