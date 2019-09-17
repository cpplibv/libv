// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/string_u_2D.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/enumerate.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/utility/observer_ref.hpp>
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

//                                              Left, Center, Right, Justify, JustifyAll
static constexpr std::array AlignHorizontalTable = {0.0f, 0.5f, 1.0f, 0.0f, 0.0f};

} // namespace

// -------------------------------------------------------------------------------------------------

StringU2D::StringU2D() { }

// -------------------------------------------------------------------------------------------------

void StringU2D::setAlign(const AlignHorizontal align_) {
	if (align == align_)
		return;

	align = align_;
	dirty = true;
}

void StringU2D::setFont(std::shared_ptr<Font2D> font_, const FontSize fontSize_) {
	if (font == font_ && fontSize == fontSize_)
		return;

	font = std::move(font_);
	fontSize = fontSize_;
	dirty = true;
}

void StringU2D::setSize(const FontSize fontSize_) {
	if (fontSize == fontSize_)
		return;

	fontSize = fontSize_;
	dirty = true;
}

void StringU2D::setString(std::string string_) {
	codepoints.clear();
	push_back(string_);
}

void StringU2D::setLimit(const libv::vec2f limit_) {
	if (limit == limit_)
		return;

	limit = limit_;
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

void StringU2D::push_back(const uint32_t unicode) {
	codepoints.push_back(unicode);

	dirty = true;
}

void StringU2D::push_back(const std::string_view string_) {
	for (auto it = string_.begin(); it != string_.end();) {
		constexpr auto invalid_utf8_character_replacement = '_';

		auto& codepoint = codepoints.emplace_back(invalid_utf8_character_replacement);
		const auto ec = utf8::internal::validate_next(it, string_.end(), codepoint);

		if (ec == utf8::internal::UTF8_OK) {
			// Everything is ok, codepoint already set
		} else if (ec == utf8::internal::NOT_ENOUGH_ROOM) {
			log_ui.error("Invalid utf8 string NOT_ENOUGH_ROOM at index {} in: \"{}\"", string_, std::distance(string_.begin(), it));
		} else if (ec == utf8::internal::INVALID_LEAD) {
			log_ui.error("Invalid utf8 string INVALID_LEAD at index {} in: \"{}\"", string_, std::distance(string_.begin(), it));
		} else if (ec == utf8::internal::INCOMPLETE_SEQUENCE) {
			log_ui.error("Invalid utf8 string INCOMPLETE_SEQUENCE at index {} in: \"{}\"", string_, std::distance(string_.begin(), it));
		} else if (ec == utf8::internal::OVERLONG_SEQUENCE) {
			log_ui.error("Invalid utf8 string OVERLONG_SEQUENCE at index {} in: \"{}\"", string_, std::distance(string_.begin(), it));
		} else if (ec == utf8::internal::INVALID_CODE_POINT) {
			log_ui.error("Invalid utf8 string INVALID_CODE_POINT at index {} in: \"{}\"", string_, std::distance(string_.begin(), it));
		}
	}

	dirty = true;
}

void StringU2D::pop_back() {
	codepoints.pop_back();

	dirty = true;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f StringU2D::getContent(libv::vec2f limit_) {
	const auto lineHeight = static_cast<float>(font->getLineAdvance(fontSize));
	if (limit_.x <= 0.0f)
		limit_.x = std::numeric_limits<float>::max();

	auto codepointPrevious = uint32_t{0};
	auto lineWidth = 0.0f;
	auto contentWidth = 0.0f;
	auto contentHeight = lineHeight;

	for (const uint32_t codepointCurrent : codepoints) {
		const auto& glyph = font->getCharacter(codepointCurrent, fontSize);
		const auto& kerning = font->getKerning(codepointPrevious, codepointCurrent, fontSize);

		const auto requestedLineWidth = lineWidth + kerning.x + glyph.advance.x;

		if (requestedLineWidth > limit_.x || codepointCurrent == '\n') {
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

const libv::glr::Mesh& StringU2D::mesh() {
	if (dirty)
		layout();

	return mesh_;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f StringU2D::getCharacterPosition() {
	if (codepoints.empty())
		return {};

	if (dirty)
		layout();

	const auto& descender = font->getDescender(fontSize);
	const auto& glyph = font->getCharacter(codepoints.back(), fontSize);
	const auto& kerning = font->getKerning(codepoints.back(), 0, fontSize);

	auto positions = mesh_.attribute(attribute_position);
	const auto& position = positions[positions.size() - 4];

	return libv::vec2f{position.x - glyph.pos[0].x + kerning.x + glyph.advance.x, position.y - glyph.pos[0].y + descender};
}

libv::vec2f StringU2D::getCharacterPosition(size_t characterIndex) {
	if (characterIndex >= codepoints.size())
		return getCharacterPosition();

	if (codepoints.empty())
		return {};

	if (dirty)
		layout();

	const auto& descender = font->getDescender(fontSize);
	const auto& glyph = font->getCharacter(codepoints[characterIndex], fontSize);

	auto positions = mesh_.attribute(attribute_position);
	const auto& position = positions[characterIndex * 4];

	return libv::vec2f{position.x - glyph.pos[0].x, position.y - glyph.pos[0].y + descender};
}

//libv::vec2f StringU2D::getLinePosition() {
//	return {}; // Not implemented yet.
//}
//
//libv::vec2f StringU2D::getLinePosition(size_t lineIndex) {
//	return {}; // Not implemented yet.
//}
//
//size_t StringU2D::getClosestCharacterIndex(libv::vec2f position) {
//	return {}; // Not implemented yet.
//}
//
//size_t StringU2D::getClosestLineIndex(libv::vec2f position) {
//	return {}; // Not implemented yet.
//}

// -------------------------------------------------------------------------------------------------

void StringU2D::layout() {
	mesh_.clear();
	auto position = mesh_.attribute(attribute_position);
	auto texture0 = mesh_.attribute(attribute_texture0);
	auto index = mesh_.index();
	mesh_.reserve(codepoints.size() * 4, codepoints.size() * 6);

	struct Line {
		size_t begin = 0;
		size_t end = 0;
		float width = 0.0f;
		boost::container::small_vector<size_t, 32> wordEndings{};
	};

	const auto lineHeight = font->getLineAdvance(fontSize);
	const auto descender = font->getDescender(fontSize);
	const auto hasLimitX = limit.x > 0;
	const auto hasLimitY = limit.y > 0;
	const auto heightAdjusment = hasLimitY ? std::round(limit.y - lineHeight - descender) : 0.0f;
	auto pen = libv::vec2f{0, heightAdjusment};
	auto previousCodepoint = uint32_t{'\n'};
	auto lines = boost::container::small_vector<Line, 1>{1};
	auto line = libv::observer_ref<Line>{lines.data()};
	auto contentWidth = limit.x;

	const auto finishLine = [&] {
		line->wordEndings.emplace_back(line->end);
		line->width = pen.x;
		if (!hasLimitX)
			contentWidth = std::max(line->width, contentWidth);
	};

	const auto newLine = [&] {
		finishLine();

		const auto lastEnd = line->end;
		line = make_observer_ref(lines.emplace_back());
		line->begin = lastEnd;
		line->end = lastEnd;

		previousCodepoint = 0;
		pen.x = 0.0f;
		pen.y -= lineHeight;
	};

	for (const uint32_t currentCodepoint : codepoints) {
		if (currentCodepoint == '\n') {
			newLine();
			continue;
		}

		const auto& glyph = font->getCharacter(currentCodepoint, fontSize);
		const auto& kerning = font->getKerning(previousCodepoint, currentCodepoint, fontSize);

		if (hasLimitX && pen.x + glyph.advance.x + kerning.x > limit.x)
			newLine();

		if (currentCodepoint == ' ')
			line->wordEndings.emplace_back(line->end);

		pen.x += kerning.x;

		const auto vertex_base = static_cast<libv::glr::VertexIndex>(position.size());
		position(libv::vec3f{glyph.pos[0] + pen, 0});
		position(libv::vec3f{glyph.pos[1] + pen, 0});
		position(libv::vec3f{glyph.pos[2] + pen, 0});
		position(libv::vec3f{glyph.pos[3] + pen, 0});

		texture0(glyph.tex[0]);
		texture0(glyph.tex[1]);
		texture0(glyph.tex[2]);
		texture0(glyph.tex[3]);

		index.quad(vertex_base + 0, vertex_base + 1, vertex_base + 2, vertex_base + 3);

		line->end += 4;
		pen.x += glyph.advance.x;

		previousCodepoint = currentCodepoint;
	}
	finishLine();

	for (const auto& [index, line] : lines | ranges::view::enumerate) {
		const auto leftover = contentWidth - line.width;

		const auto offset = std::round(leftover * AlignHorizontalTable[to_value(align)]);
		const auto isLastLine = static_cast<size_t>(index) == lines.size() - 1;
		const auto isJustifiedLine = align == AlignHorizontal::JustifyAll || (align == AlignHorizontal::Justify && !isLastLine);
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
