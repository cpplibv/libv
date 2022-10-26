// Project: libv.ui, File: src/libv/ui/component/layout/layout_text.cpp

// hpp
#include <libv/ui/component/layout/layout_text.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/enumerate.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/algo/slice.hpp>
#include <libv/range/view_uft8_codepoints.hpp>
#include <libv/utility/memory/observer_ref.hpp>
#include <libv/utility/utf8.hpp>
// std
#include <cmath>
// pro
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/resource/font_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

static constexpr uint32_t invalidCodepointReplacement = '_';

// -------------------------------------------------------------------------------------------------

void LayoutText::align_horizontal(const AlignHorizontal new_align) {
	if (align_horizontal_ == new_align)
		return;

	align_horizontal_ = new_align;
	dirty = true;
}

void LayoutText::align_vertical(const AlignVertical new_align) {
	if (align_vertical_ == new_align)
		return;

	align_vertical_ = new_align;
	dirty = true;
}

void LayoutText::font(std::shared_ptr<Font2D> new_font) {
	if (font_ == new_font)
		return;

	font_ = std::move(new_font);
	dirty = true;
}

void LayoutText::font(std::shared_ptr<Font2D> new_font, const FontSize new_size) {
	if (font_ == new_font && fontSize_ == new_size)
		return;

	font_ = std::move(new_font);
	fontSize_ = new_size;
	dirty = true;
}

void LayoutText::size(const FontSize new_size) {
	if (fontSize_ == new_size)
		return;

	fontSize_ = new_size;
	dirty = true;
}

void LayoutText::string(std::string value) {
	if (string_ == value)
		return;

	if (utf8::is_valid(value.begin(), value.end())) {
		string_ = std::move(value);

	} else {
		string_.clear();
		utf8::replace_invalid(value.begin(), value.end(), std::back_inserter(string_), '_');
		log_ui.error("Attempted to setString with an invalid string: \"{}\" substituting with {}", value, string_);
	}
	dirty = true;
}

void LayoutText::limit(const libv::vec2f new_limit) {
	if (limit_ == new_limit)
		return;

	limit_ = new_limit;
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

size_t LayoutText::length() const noexcept {
	return libv::distance_utf8_unchecked(string_);
}

// -------------------------------------------------------------------------------------------------

void LayoutText::insert(std::size_t position, uint32_t unicode) {
	if (!utf8::internal::is_code_point_valid(unicode)) {
		log_ui.error("Attempted to insert an invalid codepoint: {} at {} substituting with {}", unicode, position, invalidCodepointReplacement);
		unicode = invalidCodepointReplacement;
	}

	libv::insert_utf8_unchecked(string_, position, unicode);

	dirty = true;
}

size_t LayoutText::insert(std::size_t position, const std::string_view value) {
	std::size_t codepoints = 0;

	if (utf8::is_valid(value.begin(), value.end())) {
		libv::insert_utf8_unchecked(string_, position, value);
		codepoints = libv::distance_utf8_unchecked(value);

	} else {
		std::string fixedString;
		utf8::replace_invalid(value.begin(), value.end(), std::back_inserter(fixedString), '_');
		log_ui.error("Attempted to insert invalid utf8 string: \"{}\" at {} substituting with \"{}\"",
				value, position, fixedString);

		libv::insert_utf8_unchecked(string_, position, fixedString);
		codepoints = libv::distance_utf8_unchecked(fixedString);
	}

	dirty = true;
	return codepoints;
}

void LayoutText::erase(std::size_t position, std::size_t count) {
	auto it = string_.begin();

	if (!libv::advance_utf8_unchecked(it, string_.end(), position))
		return log_ui.error("Invalid position {}. Failed to erase {} character in string: \"{}\"", position, count, string_);

	const auto from = it;

	if (!libv::advance_utf8_unchecked(it, string_.end(), count))
		return log_ui.error("Invalid position {}. Failed to erase {} character in string: \"{}\"", position, count, string_);

	string_.erase(from, it);
	dirty = true;
}

void LayoutText::push_back(uint32_t unicode) {
	if (!utf8::internal::is_code_point_valid(unicode)) {
		log_ui.error("Attempted to push_back an invalid codepoint: {} substituting with {}", unicode, invalidCodepointReplacement);
		unicode = invalidCodepointReplacement;
	}

	utf8::unchecked::append(unicode, std::back_inserter(string_));

	dirty = true;
}

void LayoutText::push_back(const std::string_view value) {
	if (utf8::is_valid(value.begin(), value.end())) {
		string_.append(value);

	} else {
		utf8::replace_invalid(value.begin(), value.end(), std::back_inserter(string_), '_');
		log_ui.error("Attempted to push_back invalid utf8 string: \"{}\" substituting with \"{}\"",
				value,
				libv::slice_view(string_, -static_cast<int64_t>(value.size())));
	}
	dirty = true;
}

void LayoutText::pop_back() {
	if (string_.empty())
		return log_ui.warn("Attempted to pop_back an empty string");

	auto it = string_.end();
	utf8::unchecked::prior(it);
	string_.erase(it, string_.end());

	dirty = true;
}

void LayoutText::clear() {
	string_.clear();
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

const LayoutTextData& LayoutText::vertices_data() {
	if (dirty)
		layout();

	return static_cast<const LayoutTextData&>(*this);
}

// -------------------------------------------------------------------------------------------------

libv::vec2f LayoutText::getCharacterPosition() {
	if (dirty)
		layout();

	// NOTE: codepoint is \n as there is a hidden new-line glyph placed to the end to help these functions
	const auto codepoint = '\n';

	const auto& descender = font_->getDescender(fontSize_);
	const auto& glyph = font_->getCharacter(codepoint, fontSize_);
	const auto& position = positions[positions.size() - 4];

	return {position.x - glyph.pos[0].x, position.y - glyph.pos[0].y + descender};
}

libv::vec2f LayoutText::getCharacterPosition(std::size_t characterIndex) {
	auto it = string_.begin();

	if (!libv::advance_utf8_unchecked(it, string_.end(), characterIndex))
		log_ui.warn("Invalid character index {}. Failed to get character position. Returning last character's position of {}", characterIndex, length());

	if (it == string_.end())
		return getCharacterPosition();

	if (dirty)
		layout();

	const auto codepoint = utf8::unchecked::next(it);

	const auto& descender = font_->getDescender(fontSize_);
	const auto& glyph = font_->getCharacter(codepoint, fontSize_);
	const auto& position = positions[characterIndex * 4];
	// NOTE: characterIndex can over index into end as hidden new-line glyphs are placed to help these functions

	return {position.x - glyph.pos[0].x, position.y - glyph.pos[0].y + descender};
}

//std::size_t LayoutText::getLineOfCharacter() {
//	return {}; // Not implemented yet.
//}
//
//std::size_t LayoutText::getLineOfCharacter(std::size_t characterIndex) {
//	return {}; // Not implemented yet.
//}
//
//libv::vec2f LayoutText::getLinePosition() {
//	return {}; // Not implemented yet.
//}
//
//libv::vec2f LayoutText::getLinePosition(std::size_t lineIndex) {
//	return {}; // Not implemented yet.
//}

size_t LayoutText::getClosestCharacterIndex(libv::vec2f position) {
	if (dirty)
		layout();

	const auto lineHeight = font_->getLineHeight(fontSize_);

	float min_distance_sq = std::numeric_limits<float>::infinity();
	std::size_t min_index = 0; // In case of empty string, index 0 is valid
	std::size_t index = 0;

	const auto check = [&](uint32_t codepoint, std::size_t i) {
		const auto& descender = font_->getDescender(fontSize_);
		const auto& glyph = font_->getCharacter(codepoint, fontSize_);

		const auto& glyph_position = positions[i * 4];

		const auto pen = libv::vec2f{glyph_position.x - glyph.pos[0].x, glyph_position.y - glyph.pos[0].y + descender};
		const auto left_center = libv::vec2f{pen.x, pen.y + lineHeight * 0.5f};
		// As the caret is displayed on the left side of the glyph, calculations are based on the left center

		const auto distance_sq = (position - left_center).length_sq();

		if (distance_sq < min_distance_sq) {
			min_index = i;
			min_distance_sq = distance_sq;
		}
	};

	for (auto it = string_.begin(); it != string_.end();)
		check(utf8::unchecked::next(it), index++);
	check('\n', index); // NOTE: allowed to over index into end as hidden new-line glyphs are placed to help these functions

	return min_index;
}

size_t LayoutText::getClosestCharacterIndexInline(libv::vec2f position) {
	if (dirty)
		layout();

	const auto lineHeight = font_->getLineHeight(fontSize_);

	float min_distance_x = std::numeric_limits<float>::infinity();
	float min_distance_y = std::numeric_limits<float>::infinity();
	std::size_t min_index = 0; // In case of empty string, index 0 is valid
	std::size_t index = 0;

	const auto check = [&](uint32_t codepoint, std::size_t i) {
		const auto& descender = font_->getDescender(fontSize_);
		const auto& glyph = font_->getCharacter(codepoint, fontSize_);

		const auto& glyph_position = positions[i * 4];

		const auto pen = libv::vec2f{glyph_position.x - glyph.pos[0].x, glyph_position.y - glyph.pos[0].y + descender};
		const auto left_center = libv::vec2f{pen.x, pen.y + lineHeight * 0.5f};
		// As the caret is displayed on the left side of the glyph, calculations are based on the left center

		const auto distance = libv::vec::abs(position - left_center);

		if (distance.y < min_distance_y) {
			min_distance_y = distance.y;
			min_distance_x = std::numeric_limits<float>::infinity();
		}

		const auto is_minline = std::abs(distance.y - min_distance_y) < 0.001f;

		if (is_minline && distance.x < min_distance_x) {
			min_distance_x = distance.x;
			min_index = i;
		}
	};

	for (auto it = string_.begin(); it != string_.end();)
		check(utf8::unchecked::next(it), index++);
	check('\n', index); // NOTE: allowed to over index into end as hidden new-line glyphs are placed to help these functions

	return min_index;
}

//std::size_t LayoutText::getClosestLineIndex(libv::vec2f position) {
//	return {}; // Not implemented yet.
//}

// -------------------------------------------------------------------------------------------------

libv::vec2f LayoutText::content(libv::vec2f new_limit) {
	const auto lineAdvance = font_->getLineAdvance(fontSize_);

	if (string_.empty())
		return {0.f, lineAdvance};

	if (new_limit.x < 0.0f)
		new_limit.x = std::numeric_limits<float>::max();

	auto codepointPrevious = uint32_t{0};
	auto lineWidth = 0.0f;
	auto contentWidth = 0.0f;
	auto contentHeight = lineAdvance;

	for (const uint32_t codepointCurrent : string_ | libv::view::uft8_codepoints) {
		const auto& glyph = font_->getCharacter(codepointCurrent, fontSize_);
		const auto& kerning = font_->getKerning(codepointPrevious, codepointCurrent, fontSize_);

		const auto requestedLineWidth = lineWidth + kerning.x + glyph.advance.x;

		if (requestedLineWidth > new_limit.x || codepointCurrent == '\n') {
			contentWidth = std::max(contentWidth, lineWidth);
			contentHeight += lineAdvance;
			lineWidth = 0.0f;
		} else {
			lineWidth = requestedLineWidth;
		}

		codepointPrevious = codepointCurrent;
	}

	contentWidth = std::max(contentWidth, lineWidth);

	return {contentWidth, contentHeight};
}

// -------------------------------------------------------------------------------------------------

void LayoutText::layout() {
	if (font_ == nullptr) {
		log_ui.error("Attempted to layout a LayoutText without a valid font");
		assert(false && "Attempted to layout a LayoutText without a valid font");
		return;
	}

	// ---

	positions.clear();
	texture0s.clear();
	indices.clear();
	// NOTE: It would be nice to use resize and direct index into the data, but "virtual" new line glyphs might be inserted that cannot be calculated without the layout, also utf8 codepoint would require a shrinking resize at the end
	positions.reserve(string_.size() * 4);
	texture0s.reserve(string_.size() * 4);
	indices.reserve(string_.size() * 6);
	auto vi = 0; // Vertex Index

	const auto vertex = [&](auto pos, auto uv0) {
		positions.emplace_back(pos, 0);
		texture0s.emplace_back(uv0);
		vi++;
	};

	const auto index_quad = [&]() {
		indices.emplace_back(vi - 4 + 0);
		indices.emplace_back(vi - 4 + 1);
		indices.emplace_back(vi - 4 + 2);
		indices.emplace_back(vi - 4 + 2);
		indices.emplace_back(vi - 4 + 3);
		indices.emplace_back(vi - 4 + 0);
	};

	// ---

	struct Line {
		std::size_t begin = 0;
		std::size_t end = 0;
		float width = 0.0f;
		boost::container::small_vector<std::size_t, 32> wordEndings{};
	};

	const auto lineAdvance = font_->getLineAdvance(fontSize_);
	const auto descender = font_->getDescender(fontSize_);
	const auto hasLimitX = limit_.x > 0;
	const auto hasLimitY = limit_.y > 0;
	const auto heightAdjusment = std::round((hasLimitY ? limit_.y : 0.0f) - lineAdvance - descender);

	auto pen = libv::vec2f{0, heightAdjusment};
	auto previousCodepoint = uint32_t{'\n'};
	auto lines = boost::container::small_vector<Line, 8>(); // ~2240 Byte on the stack
	auto line = make_observer_ref(&lines.emplace_back());
	auto contentWidth = limit_.x;

	const auto finishLine = [&] {
		line->wordEndings.emplace_back(line->end);
		line->width = pen.x;
		if (!hasLimitX)
			contentWidth = std::max(line->width, contentWidth);
	};

	const auto newLine = [&] {
		finishLine();

		const auto lastEnd = line->end;
		line = make_observer_ref(&lines.emplace_back());

		line->begin = lastEnd;
		line->end = lastEnd;

		previousCodepoint = 0;
		pen.x = 0.0f;
		pen.y -= lineAdvance;
	};

	for (const uint32_t currentCodepoint : string_ | libv::view::uft8_codepoints) {
		const auto& glyph = font_->getCharacter(currentCodepoint, fontSize_);
		const auto& kerning = font_->getKerning(previousCodepoint, currentCodepoint, fontSize_);

		if (currentCodepoint != '\n' && hasLimitX && pen.x + glyph.advance.x + kerning.x > limit_.x)
			newLine();

		line->end += 4;
		pen.x += kerning.x;

		if (currentCodepoint == ' ')
			line->wordEndings.emplace_back(line->end);

		vertex(glyph.pos[0] + pen, glyph.tex[0]);
		vertex(glyph.pos[1] + pen, glyph.tex[1]);
		vertex(glyph.pos[2] + pen, glyph.tex[2]);
		vertex(glyph.pos[3] + pen, glyph.tex[3]);

		if (currentCodepoint == '\n') {
			newLine();
			continue;
		}

		// NOTE: At every line-feed character there is an extra not rendered new-line glyph placed, so the indices are skipped for those
		index_quad();

		pen.x += glyph.advance.x;
		previousCodepoint = currentCodepoint;
	}
	{
		// NOTE: At the very end there is an extra not rendered new-line glyph placed
		const auto currentCodepoint = '\n';
		const auto& glyph = font_->getCharacter(currentCodepoint, fontSize_);
		const auto& kerning = font_->getKerning(previousCodepoint, currentCodepoint, fontSize_);

		pen.x += kerning.x;
		line->end += 4;

		vertex(glyph.pos[0] + pen, glyph.tex[0]);
		vertex(glyph.pos[1] + pen, glyph.tex[1]);
		vertex(glyph.pos[2] + pen, glyph.tex[2]);
		vertex(glyph.pos[3] + pen, glyph.tex[3]);
	}

	finishLine();

	// --- Alignments ---
	const auto alignv = info(align_vertical_);
	const auto alignh = info(align_horizontal_);

	const auto num_lines_f = static_cast<float>(lines.size());
	const auto lines_height_sum = num_lines_f * lineAdvance;

	auto leftoverMinX = std::numeric_limits<float>::infinity(); // Used for content_bounding calculation

	const auto leftoverY = hasLimitY ? (limit_.y - lines_height_sum) : 0.f;
	const auto offsetY = -1.f * leftoverY * (1.f - alignv.rate());
	const auto isJustifiedY = alignv.justified();
	const auto justifyGapY = isJustifiedY ?
			-1.f * leftoverY / std::max(1.f, num_lines_f - 1.0f) : 0.0f;

	for (const auto& [line_index, line] : lines | ranges::view::enumerate) {
		const auto line_index_f = static_cast<float>(line_index);

		const auto leftoverX = contentWidth - line.width;
		const auto offsetX = leftoverX * alignh.rate();
		const auto isLastLine = static_cast<std::size_t>(line_index) == lines.size() - 1;
		const auto isJustifiedLine = align_horizontal_ == AlignHorizontal::justify_all || (align_horizontal_ == AlignHorizontal::justify && !isLastLine);
		const auto justifyGapX = isJustifiedLine ?
				leftoverX / std::max(1.f, static_cast<float>(line.wordEndings.size() - 1)) : 0.0f;

		const auto adjusmentY = std::round(offsetY + line_index_f * justifyGapY);

		leftoverMinX = std::min(leftoverMinX, isJustifiedLine && line.wordEndings.size() > 1 ? 0.0f : leftoverX); // If it's a isJustifiedLine with at least two word than there is no leftover at all

		std::size_t i = line.begin;
		for (const auto& [word_index, wordEnd] : line.wordEndings | ranges::view::enumerate) {
			const auto word_index_f = static_cast<float>(word_index);

			const auto adjusmentX = std::round(offsetX + word_index_f * justifyGapX);

			while (i < wordEnd) {
				positions[i].x += adjusmentX;
				positions[i].y += adjusmentY;
				i++;
			}
		}
	}

	content_bounding_pos_ = libv::vec::round(libv::vec2f{
			leftoverMinX * alignh.rate(),
			isJustifiedY && lines.size() > 1 ? 0.0f : (string_.empty() ? 0.0f : leftoverY) * alignv.rate()});
	content_bounding_size_ = libv::vec::round(libv::vec2f{
			contentWidth - leftoverMinX,
			string_.empty() ? 0.0f : lines_height_sum + (isJustifiedY && lines.size() > 1 ? leftoverY : 0.0f)});

	dirty = false;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
