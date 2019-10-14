// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/string_2D.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/enumerate.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/algorithm/slice.hpp>
#include <libv/range/view_uft8_codepoints.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/utf8.hpp>
// std
#include <array>
#include <cmath>
// pro
#include <libv/ui/context_render.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

static constexpr std::array AlignHorizontalTable = {
	0.0f, // Left
	0.5f, // Center
	1.0f, // Right
	0.0f, // Justify
	0.0f, // JustifyAll
};

static constexpr uint32_t invalidCodepointReplacement = '_';

} // namespace

// -------------------------------------------------------------------------------------------------

void String2D::align(const AlignHorizontal new_align) {
	if (align_ == new_align)
		return;

	align_ = new_align;
	dirty = true;
}

void String2D::font(std::shared_ptr<Font2D> new_font) {
	if (font_ == new_font)
		return;

	font_ = std::move(new_font);
	dirty = true;
}

void String2D::font(std::shared_ptr<Font2D> new_font, const FontSize new_size) {
	if (font_ == new_font && fontSize_ == new_size)
		return;

	font_ = std::move(new_font);
	fontSize_ = new_size;
	dirty = true;
}

void String2D::size(const FontSize new_size) {
	if (fontSize_ == new_size)
		return;

	fontSize_ = new_size;
	dirty = true;
}

void String2D::string(std::string value) {
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

void String2D::limit(const libv::vec2f new_limit) {
	if (limit_ == new_limit)
		return;

	limit_ = new_limit;
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

size_t String2D::length() const noexcept {
	return libv::distance_utf8_unchecked(string_);
}

// -------------------------------------------------------------------------------------------------

void String2D::insert(size_t position, uint32_t unicode) {
	if (!utf8::internal::is_code_point_valid(unicode)) {
		log_ui.error("Attempted to insert an invalid codepoint: {} at {} substituting with {}", unicode, position, invalidCodepointReplacement);
		unicode = invalidCodepointReplacement;
	}

	libv::insert_utf8_unchecked(string_, position, unicode);

	dirty = true;
}

size_t String2D::insert(size_t position, const std::string_view value) {
	size_t codepoints = 0;

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

void String2D::erase(size_t position, size_t count) {
	auto it = string_.begin();

	if (!libv::advance_utf8_unchecked(it, string_.end(), position))
		return log_ui.error("Invalid position {}. Failed to erase {} character in string_ {}: \"{}\"", position, count, string_);

	const auto from = it;

	if (!libv::advance_utf8_unchecked(it, string_.end(), count))
		return log_ui.error("Invalid position {}. Failed to erase {} character in string_ {}: \"{}\"", position, count, string_);

	string_.erase(from, it);
	dirty = true;
}

void String2D::push_back(uint32_t unicode) {
	if (!utf8::internal::is_code_point_valid(unicode)) {
		log_ui.error("Attempted to push_back an invalid codepoint: {} substituting with {}", unicode, invalidCodepointReplacement);
		unicode = invalidCodepointReplacement;
	}

	utf8::unchecked::append(unicode, std::back_inserter(string_));

	dirty = true;
}

void String2D::push_back(const std::string_view value) {
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

void String2D::pop_back() {
	if (string_.empty())
		return log_ui.warn("Attempted to pop_back an empty string");

	auto it = string_.end();
	utf8::unchecked::prior(it);
	string_.erase(it, string_.end());

	dirty = true;
}

void String2D::clear() {
	string_.clear();
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

const libv::glr::Mesh& String2D::mesh() {
	if (dirty)
		layout();

	return mesh_;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f String2D::getCharacterPosition() {
	if (dirty)
		layout();

	// Note: codepoint is \n as there is a hidden new-line glyph placed to the end to help these functions
	const auto codepoint = '\n';

	const auto& descender = font_->getDescender(fontSize_);
	const auto& glyph = font_->getCharacter(codepoint, fontSize_);

	auto positions = mesh_.attribute(attribute_position);
	const auto& position = positions[positions.size() - 4];

	return {position.x - glyph.pos[0].x, position.y - glyph.pos[0].y + descender};
}

libv::vec2f String2D::getCharacterPosition(size_t characterIndex) {
	auto it = string_.begin();

	if (!libv::advance_utf8_unchecked(it, string_.end(), characterIndex))
		log_ui.warn("Invalid character index {}. Failed to get character position. Returning last character position of {}", characterIndex, length());

	if (it == string_.end())
		return getCharacterPosition();

	if (dirty)
		layout();

	const auto codepoint = utf8::unchecked::next(it);

	const auto& descender = font_->getDescender(fontSize_);
	const auto& glyph = font_->getCharacter(codepoint, fontSize_);

	auto positions = mesh_.attribute(attribute_position);
	const auto& position = positions[characterIndex * 4];
	// Note: characterIndex can over index into end as hidden new-line glyphs are placed to help these functions

	return {position.x - glyph.pos[0].x, position.y - glyph.pos[0].y + descender};
}

//libv::vec2f String2D::getLinePosition() {
//	return {}; // Not implemented yet.
//}
//
//libv::vec2f String2D::getLinePosition(size_t lineIndex) {
//	return {}; // Not implemented yet.
//}
//
//size_t String2D::getClosestCharacterIndex(libv::vec2f position) {
//	return {}; // Not implemented yet.
//}
//
//size_t String2D::getClosestLineIndex(libv::vec2f position) {
//	return {}; // Not implemented yet.
//}

// -------------------------------------------------------------------------------------------------

libv::vec2f String2D::content(libv::vec2f new_limit) {
	const auto lineHeight = static_cast<float>(font_->getLineAdvance(fontSize_));
	if (new_limit.x <= 0.0f)
		new_limit.x = std::numeric_limits<float>::max();

	auto codepointPrevious = uint32_t{0};
	auto lineWidth = 0.0f;
	auto contentWidth = 0.0f;
	auto contentHeight = lineHeight;

	for (const uint32_t codepointCurrent : string_ | libv::view::uft8_codepoints) {
		const auto& glyph = font_->getCharacter(codepointCurrent, fontSize_);
		const auto& kerning = font_->getKerning(codepointPrevious, codepointCurrent, fontSize_);

		const auto requestedLineWidth = lineWidth + kerning.x + glyph.advance.x;

		if (requestedLineWidth > new_limit.x || codepointCurrent == '\n') {
			contentWidth = std::max(contentWidth, lineWidth);
			contentHeight += lineHeight;
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

void String2D::layout() {
	if (font_ == nullptr) {
		log_ui.fatal("Attempted to layout a String2D without a valid font");
		assert(false && "Attempted to layout a String2D without a valid font");
	}

	mesh_.clear();
	auto position = mesh_.attribute(attribute_position);
	auto texture0 = mesh_.attribute(attribute_texture0);
	auto index = mesh_.index();
	mesh_.reserve(string_.size() * 4, string_.size() * 6);

	struct Line {
		size_t begin = 0;
		size_t end = 0;
		float width = 0.0f;
		boost::container::small_vector<size_t, 32> wordEndings{};
	};

	const auto lineHeight = font_->getLineAdvance(fontSize_);
	const auto descender = font_->getDescender(fontSize_);
	const auto hasLimitX = limit_.x > 0;
	const auto hasLimitY = limit_.y > 0;
	const auto heightAdjusment = hasLimitY ? std::round(limit_.y - lineHeight - descender) : 0.0f;
	auto pen = libv::vec2f{0, heightAdjusment};
	auto previousCodepoint = uint32_t{'\n'};
	auto lines = boost::container::small_vector<Line, 1>{1};
	auto line = libv::observer_ref<Line>{lines.data()};
	auto contentWidth = limit_.x;

	const auto finishLine = [&] {
		line->wordEndings.emplace_back(line->end);
		line->width = pen.x;
		if (!hasLimitX)
			contentWidth = std::max(line->width, contentWidth);
	};

	const auto newLine = [&]() {
		finishLine();

		const auto lastEnd = line->end;
		line = make_observer_ref(lines.emplace_back());
		line->begin = lastEnd;
		line->end = lastEnd;

		previousCodepoint = 0;
		pen.x = 0.0f;
		pen.y -= lineHeight;
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

		const auto vertex_base = static_cast<libv::glr::VertexIndex>(position.size());

		position(libv::vec3f{glyph.pos[0] + pen, 0});
		position(libv::vec3f{glyph.pos[1] + pen, 0});
		position(libv::vec3f{glyph.pos[2] + pen, 0});
		position(libv::vec3f{glyph.pos[3] + pen, 0});

		texture0(glyph.tex[0]);
		texture0(glyph.tex[1]);
		texture0(glyph.tex[2]);
		texture0(glyph.tex[3]);

		if (currentCodepoint == '\n') {
			newLine();
			// NOTE: At every line-feed character there is an extra not rendered new-line glyph placed
			continue;
		}

		index.quad(vertex_base + 0, vertex_base + 1, vertex_base + 2, vertex_base + 3);
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

		position(libv::vec3f{glyph.pos[0] + pen, 0});
		position(libv::vec3f{glyph.pos[1] + pen, 0});
		position(libv::vec3f{glyph.pos[2] + pen, 0});
		position(libv::vec3f{glyph.pos[3] + pen, 0});

		texture0(glyph.tex[0]);
		texture0(glyph.tex[1]);
		texture0(glyph.tex[2]);
		texture0(glyph.tex[3]);
	}
	finishLine();

	for (const auto& [index, line] : lines | ranges::view::enumerate) {
		const auto leftover = contentWidth - line.width;

		const auto offset = std::round(leftover * AlignHorizontalTable[libv::to_value(align_)]);
		const auto isLastLine = static_cast<size_t>(index) == lines.size() - 1;
		const auto isJustifiedLine = align_ == AlignHorizontal::JustifyAll || (align_ == AlignHorizontal::Justify && !isLastLine);
		const auto justifyGap = isJustifiedLine ?
				leftover / std::max(1.f, static_cast<float>(line.wordEndings.size() - 1)) : 0.0f;

		size_t i = line.begin;
		auto justifyOffset = 0.0f;
		for (const auto& wordEnd : line.wordEndings) {
			while (i < wordEnd) {
				position[i++].x += offset + justifyOffset;
			}
			justifyOffset += justifyGap;
		}
	}

	dirty = false;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
