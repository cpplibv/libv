// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/component/string_2D.hpp>
// ext
#include <GL/glew.h>
//#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/path.hpp>
#include <glm/gtx/transform.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>
// std
#include <sstream>
// pro
#include <libv/ui/resource/font.hpp>
#include <libv/ui/render/context.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

String2D::String2D(const std::string& text, Anchor halign, Anchor valign) :
	halign(halign),
	valign(valign),
	rawText(text) { }


// -------------------------------------------------------------------------------------------------

void String2D::setText(const std::string& text) {
	if (!utf8::is_valid(text.begin(), text.end())) {
		rawText.clear();
		utf8::replace_invalid(text.begin(), text.end(), std::back_inserter(rawText), ' ');

		LIBV_LOG_UI_COMPONENT_ERROR("Invalid utf8 text: [%s] value will be used as [%s]", text, rawText);
	} else
		rawText = text;
	renderChanged = true;
}

void String2D::setSize(ivec2 size) {
	maxSize = size;
	renderChanged = true;
}

// -------------------------------------------------------------------------------------------------

ivec2 String2D::layout() {
	return ivec2();
}

void String2D::build(Context& context) {
	if (renderChanged) {
		buildImpl(context);
		renderChanged = false;
	}
	LIBV_GL_DEBUG_CHECK_GL();
}

void String2D::destroy(Context&) {
	segments.clear();
	vao.destroy();
	vbo.destroy();
	LIBV_GL_DEBUG_CHECK_GL();
}

void String2D::render(Context& context) {
	{
		auto dDisableGuard = context.gl.disableGuard(gl::Capability::DepthTest);
		auto mStackGuard = context.gl.model.pushGuard();

//		context.gl.model.translate(0, -defaultFont.getDescender(), 0); // full text size except first line
//		context.gl.model.translate(0, -150, 0); // full text size except first line
//		context.gl.model.translate(150, 0, 0); // full text size except first line

		for (const auto& segment : segments) {
			segment.program->use();
			segment.program->uniformMVPmat = context.gl.mvp();
			segment.program->uniformTextureSampler = gl::TextureChannel::diffuse;
			segment.program->uniformColor = segment.color;

			context.gl.activeTexture(gl::TextureChannel::diffuse);
			auto txGuard = segment.font->bindGuard();

			context.gl.drawArrays(vao, gl::Primitive::Triangles, segment.vertexCount, segment.vertexStart);
		}
	}
	glUseProgram(0);

	LIBV_GL_DEBUG_CHECK_GL();
}

// -------------------------------------------------------------------------------------------------
struct String2D::Builder {
	// TODO P2: Base font by property
	// TODO P2: Base font program by property
	// TODO P2: Base font color by property
	// TODO P2: Base font size by property

	// TODO P2: Hide members, use swapXYwith() member functions
	// TODO P2: String 2D could provide resources so only one ref / resource instead of usage in string
	//			Requires scoping and common base to references.
	// TODO P2: Finalize function

	Context& context;

	std::stack<UIFont2D_view> stackFont;
	std::stack<UIProgram_view> stackProgram;
	std::stack<fvec4> stackColor;
	std::stack<uint32_t> stackSize;

	std::vector<String2D::Segment> segments;
	std::vector<String2D::Character> characters;

	uint32_t previousChar = 0;
	uint32_t currentChar = 0;

	uint32_t lineMaxFontSize = 0;

	ivec2 pen;
	ivec2 lineSize;
	ivec2 layoutSize;

	ivec2 maxSize;

	std::vector<uint32_t> currentTag;
	std::vector<uint32_t> currentTagValue;

private:
	bool isTag(const char* tag) {
		return std::equal(currentTag.begin(), currentTag.end(), tag);
	}

	std::string tagValueAsString() {
		std::string temp;
		for (const auto& item : currentTagValue)
			utf8::append(item, std::back_inserter(temp));
		return temp;
	}

	int32_t tagValueAsInt() {
		auto str = tagValueAsString();
		return std::atoi(str.c_str());
	}

	fvec4 tagValueAsVec4() {
		fvec4 result;
		std::istringstream ss(tagValueAsString());
		ss >> result.r;
		ss >> result.g;
		ss >> result.b;
		ss >> result.a;
		return result;
	}

private:
	enum class State {
		Char,
		Escape,
		Tag,
		TagValue,
	} state = State::Char;

	void outputNewLine() {
		currentChar = 0;
		lineMaxFontSize = 0;

		layoutSize.x = std::max(layoutSize.x, lineSize.x);
		layoutSize.y = layoutSize.y + lineSize.y;
		pen.x = 0;
		pen.y = pen.y - lineSize.y;
		lineSize.x = 0;
		lineSize.y = 0;
	}

	void outputChar(uint32_t character) {
		previousChar = currentChar;
		currentChar = character;

		auto& currentFont = stackFont.top();
		const auto& currentSize = stackSize.top();
		const auto& currentGlyph = currentFont->getCharacter(currentChar, currentSize);

		const auto& kerning = currentFont->getKerning(previousChar, currentChar, currentSize).x;

		pen.x += kerning;
		lineMaxFontSize = std::max(lineMaxFontSize, currentSize);

		if (maxSize.x > 0 && (pen.x + currentGlyph.advance.x) > maxSize.x)
			outputNewLine();

		lineSize.x = lineSize.x + currentGlyph.advance.x;
		lineSize.y = std::max(lineSize.y, currentFont->getLineAdvance(lineMaxFontSize));

		characters.emplace_back(pen, currentGlyph);
		pen.x += currentGlyph.advance.x;
	}

	void outputTag() {
		auto safePop = [](auto& stack) {
			if (!stack.empty())
				stack.pop();
		};

		if (isTag("program")) {
			stackProgram.push(context.resource.program(tagValueAsString()));
		} else if (isTag("/program")) {
			safePop(stackProgram);
		} else if (isTag("font")) {
			auto path = boost::filesystem::path(tagValueAsString());
			stackFont.push(context.resource.font(path));
		} else if (isTag("/font")) {
			safePop(stackFont);
		} else if (isTag("size")) {
			stackSize.push(tagValueAsInt());
		} else if (isTag("/size")) {
			safePop(stackSize);
		} else if (isTag("color")) {
			stackColor.push(tagValueAsVec4());
		} else if (isTag("/color")) {
			safePop(stackColor);
		} else {
			currentTag.clear();
			currentTagValue.clear();
			return;
		}

		segments.back().vertexCount = characters.size() * 6 - segments.back().vertexStart;
		if (segments.back().vertexCount == 0) // Drop empty segments
			segments.pop_back();

		segments.emplace_back(characters.size() * 6,
				stackFont.top(),
				stackProgram.top(),
				stackColor.top(),
				stackSize.top());

		currentTag.clear();
		currentTagValue.clear();
	}

	// TODO P5: implement a proper state machine with framework
	//  STATE   |    INPUT    |    OUTPUT    |    NEXTSTATE
	// ---------+-------------+--------------+------------------
	// Char     | \           |              | Escape
	// Char     | [           |              | Tag
	// Char     | \n          | newline      | Char
	// Char     | --ANYTING-- | char(in)     | Char
	// Escape   | \           | char(\)      | Char
	// Escape   | n           | newline      | Char
	// Escape   | [           | char([)      | Char
	// Escape   | --ANYTING-- |              | Char
	// Tag      | ]           | tag()        | Char
	// Tag      | =           |              | TagValue
	// Tag      | --ANYTING-- | tag+=in      | Tag
	// TagValue | ]           | tag(value)   | Char
	// TagValue | --ANYTING-- | value+=in    | Tag

public:

	void process(uint32_t character) {
		switch (state) {
		case State::Char:
			if (character == '\\') {
				state = State::Escape;
			} else if (character == '[') {
				state = State::Tag;
			} else if (character == '\n') {
				outputNewLine();
			} else {
				outputChar(character);
			}
			break;
		case State::Escape:
			if (character == '\\') {
				outputChar('\\');
				state = State::Char;
			} else if (character == 'n') {
				outputNewLine();
				state = State::Char;
			} else if (character == '[') {
				outputChar('[');
				state = State::Char;
			} else {
				state = State::Char;
			}
			break;
		case State::Tag:
			if (character == ']') {
				outputTag();
				state = State::Char;
			} else if (character == '=') {
				state = State::TagValue;
			} else {
				currentTag.emplace_back(character);
			}
			break;
		case State::TagValue:
			if (character == ']') {
				outputTag();
				state = State::Char;
			} else {
				currentTagValue.emplace_back(character);
			}
			break;
		}
	}

	void finalize() {
		outputNewLine();
		segments.back().vertexCount = characters.size() * 6 - segments.back().vertexStart;
	}

	Builder(Context& context, ivec2 maxSize) :
		context(context),
		stackFont({context.resource.font("res/font/default.ttf")}),
	stackProgram({context.resource.program("res/shader/font")}),
	stackColor({context.config.defaultFontColor}),
	stackSize({context.config.defaultFontSize}),
	maxSize(maxSize) {
		segments.emplace_back(0, stackFont.top(), stackProgram.top(), stackColor.top(), stackSize.top());
	}
};

// -------------------------------------------------------------------------------------------------

//void foo() {
//	if (isTag("program")) {
//		stackProgram.push(context.resource.program(tagValueAsString()));
//	} else if (isTag("/program")) {
//		safePop(stackProgram);
//	} else if (isTag("font")) {
//		auto path = boost::filesystem::path(tagValueAsString());
//		stackFont.push(context.resource.font(path));
//	} else if (isTag("/font")) {
//		safePop(stackFont);
//	} else if (isTag("size")) {
//		stackSize.push(tagValueAsInt());
//	} else if (isTag("/size")) {
//		safePop(stackSize);
//	} else if (isTag("color")) {
//		stackColor.push(tagValueAsVec4());
//	} else if (isTag("/color")) {
//		safePop(stackColor);
//	}
//}

void String2D::buildImpl(Context& context) {
	LIBV_LOG_UI_COMPONENT_TRACE("Build string2D [%s]", rawText);
	Builder builder(context, maxSize);

	auto iter = rawText.begin();
	auto end = rawText.end();

	//	bool escape = false;
	//	bool startNewSegment = false;
	//
	//	layoutSize.y = std::max(lineSize.y, pen.x);
	//	pen.x = 0;
	//	pen.y += currentSegment.font->getLineAdvance();
	//	continue;

	while (iter != end)
		builder.process(utf8::next(iter, end));
	builder.finalize();

	layoutedSize = builder.layoutSize;
	segments.swap(builder.segments);
	characters.swap(builder.characters);

	for (const auto& segment : segments) {
		LIBV_LOG_UI_COMPONENT_TRACE("Segment: start=%3d, count=%3d, size=%d, color=%0.2f %0.2f %0.2f %0.2f",
				segment.vertexStart / 6, segment.vertexCount / 6, segment.size, segment.color.x, segment.color.y, segment.color.z, segment.color.w);
	}

	if (!vbo) {
		vbo.create();
		vao.create();
	}

	vbo.data(characters.data(), characters.size() * sizeof(characters[0]), gl::BufferUsage::StaticDraw);

	// TODO P3: bind attribute based on member pointer
	//	vao.bindAttribute(vbo, 0, &Character::vertexCoord, false);
	//	vao.bindAttribute(vbo, 8, &Character::textureCoord, false);

	vao.bindAttribute(vbo, gl::Attribute<glm::vec2>(0), sizeof (Character::Vertex), offsetof(Character::Vertex, position), false);
	vao.bindAttribute(vbo, gl::Attribute<glm::vec2>(8), sizeof (Character::Vertex), offsetof(Character::Vertex, texture0), false);

	LIBV_GL_DEBUG_CHECK_GL();
	LIBV_LOG_UI_COMPONENT_TRACE("Built string2D [%s]", rawText);
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
