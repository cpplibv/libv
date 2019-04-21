// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/string_2D.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/enumerate.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/glr/queue.hpp>
#include <libv/range/view_uft8_codepoints.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/slice.hpp>
// std
#include <algorithm>
#include <array>
#include <optional>
// pro
#include <libv/ui/context_render.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

//                                               Left, Center, Right, Justify
static constexpr std::array AlignHorizontalTable = {0.0f, 0.5f, 1.0f, 0.0f};

} // namespace

// -------------------------------------------------------------------------------------------------

String2D::String2D(std::string string, const AlignHorizontal align) :
	align(align) {
	setString(std::move(string));
}

// -------------------------------------------------------------------------------------------------

void String2D::setAlign(const AlignHorizontal align_) {
	if (align != align_) {
		align = align_;
		dirty = true;
	}
}

void String2D::setFont(std::shared_ptr<Font2D> font_) {
	if (font != font_) {
		font = std::move(font_);
		dirty = true;
	}
}

void String2D::setFont(std::shared_ptr<Font2D> font_, const FontSize fontSize_) {
	if (font != font_ || fontSize != fontSize_) {
		font = std::move(font_);
		fontSize = fontSize_;
		dirty = true;
	}
}

void String2D::setLimit(const libv::vec2f limit_) {
	if (limit != limit_) {
		limit = limit_;
		dirty = true;
	}
}

void String2D::setPosition(const libv::vec3f position_) {
	if (position != position_) {
		position = position_;
		dirty = true;
	}
}

void String2D::setSize(const FontSize fontSize_) {
	if (fontSize != fontSize_) {
		fontSize = fontSize_;
		dirty = true;
	}
}

void String2D::setString(std::string string_) {
	if (string != string_) {
		if (utf8::is_valid(string_.begin(), string_.end())) {
			string = std::move(string_);

		} else {
			string.clear();
			utf8::replace_invalid(string_.begin(), string_.end(), std::back_inserter(string), '_');
			log_ui.error("Invalid utf8 string: \"{}\" is converted to \"{}\"", string_, string);
		}
		dirty = true;
	}
}

void String2D::push_back(const std::string_view string_) {
	if (utf8::is_valid(string_.begin(), string_.end())) {
		string.append(string_);

	} else {
		utf8::replace_invalid(string_.begin(), string_.end(), std::back_inserter(string), '_');
		log_ui.error("Invalid utf8 string: \"{}\" is converted to \"{}\"",
				string_,
				libv::slice_view(string, -static_cast<int64_t>(string_.size())));
	}
	dirty = true;
}

void String2D::pop_back() {
	try {
		auto it = string.end();
		utf8::prior(it, string.begin());
		string.erase(it, string.end());
	} catch (const utf8::not_enough_room&) {
		log_ui.warn("String was already empty");
	} catch (const utf8::invalid_utf8&) {
		string.clear();
		log_ui.warn("String contained invalid utf8 sequence: string was cleared");
	}
	dirty = true;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f String2D::getContent(libv::vec2f limit_) {
	const auto lineHeight = static_cast<float>(font->getLineAdvance(fontSize));
	if (limit_.x <= 0.0f)
		limit_.x = std::numeric_limits<float>::max();

	auto previousCodepoint = uint32_t{0};
	auto lineWidth = 0.0f;
	auto width = 0.0f;
	auto height = lineHeight;

	for (const auto& currentCodepoint : string | libv::view::uft8_codepoints) {
		const auto& glyph = font->getCharacter(currentCodepoint, fontSize);
		const auto& kerning = font->getKerning(previousCodepoint, currentCodepoint, fontSize);

		const auto requestedLineWidth = lineWidth + kerning.x + glyph.advance.x;

		if (requestedLineWidth > limit_.x || currentCodepoint == '\n') {
			width = std::max(width, lineWidth);
			height += lineHeight;
			lineWidth = 0.0f;
		} else if (currentCodepoint != '\n') {
			lineWidth = requestedLineWidth;
		}

		previousCodepoint = currentCodepoint;
	}

	width = std::max(width, lineWidth);

	return {width, height};
}

// -------------------------------------------------------------------------------------------------

void String2D::render(libv::glr::Queue& gl) {
	if (dirty)
		layout();

	// bind font?
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

void String2D::layout() {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();
	mesh.reserve(string.size() * 4, string.size() * 6);

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
	auto line = observer_ref<Line>{lines.data()};
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

	for (const auto& currentCodepoint : string | libv::view::uft8_codepoints) {
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
		position(this->position + libv::vec3f{glyph.pos[0] + pen, 0});
		position(this->position + libv::vec3f{glyph.pos[1] + pen, 0});
		position(this->position + libv::vec3f{glyph.pos[2] + pen, 0});
		position(this->position + libv::vec3f{glyph.pos[3] + pen, 0});

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
		const auto justifyGap = (align == AlignHorizontal::Justify && !isLastLine) ?
				leftover / std::max(1.f, static_cast<float>(line.wordEndings.size() - 1)) :
				0.0f;

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
