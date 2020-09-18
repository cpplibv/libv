// Project: libv.ui, File: src/libv/ui/text_2D.cpp, Author: Császár Mátyás [Vader]
//
//// hpp
//#include <libv/ui/component/string_2D.hpp>
//// ext
//#include <GL/glew.h>
//#include <boost/filesystem/path.hpp>
//#include <utf8cpp/utf8.h>
//// libv
//#include <libv/gl/gl.hpp>
//#include <libv/gl/log.hpp>
//#include <libv/parse/color.hpp>
//#include <libv/parse/text_tag.hpp>
//#include <libv/range/utf8.hpp>
//#include <libv/ui/log.hpp>
//// std
//#include <sstream>
//// pro
//#include <libv/ui/render/context.hpp>
//#include <libv/ui/resource/font.hpp>
//
//
//namespace libv {
//namespace ui {
//
//// -------------------------------------------------------------------------------------------------
//
//String2D::String2D(const std::string& text, Anchor halign, Anchor valign) :
//	halign(halign),
//	valign(valign),
//	rawText(text) { }
//
//
//// -------------------------------------------------------------------------------------------------
//
//void String2D::setText(const std::string& text) {
//	if (!utf8::is_valid(text.begin(), text.end())) {
//		rawText.clear();
//		utf8::replace_invalid(text.begin(), text.end(), std::back_inserter(rawText), ' ');
//
//		LIBV_LOG_UI_COMPONENT_ERROR("Invalid utf8 text: [%s] value will be used as [%s]", text, rawText);
//	} else
//		rawText = text;
//	renderChanged = true;
//}
//
//void String2D::setSize(vec2i size) {
//	maxSize = size;
//	renderChanged = true;
//}
//
//// -------------------------------------------------------------------------------------------------
//
//vec2i String2D::layout() {
//	return vec2i();
//}
//
//void String2D::build(Context& context) {
//	if (renderChanged) {
//		buildImpl(context);
//		renderChanged = false;
//	}
//	LIBV_GL_DEBUG_CHECK();
//}
//
//void String2D::destroy(Context&) {
//	segments.clear();
//	vao.destroy();
//	vbo.destroy();
//	LIBV_GL_DEBUG_CHECK();
//}
//
//void String2D::render(Context& context) {
//	{
//		auto dDisableGuard = context.gl.disableGuard(gl::Capability::DepthTest);
//		auto mStackGuard = context.gl.model.pushGuard();
//
//		context.gl.model.translate(libv::vec3f(0, +layoutedSize.y, 0));
//
//		for (const auto& segment : segments) {
//			segment.program->use();
//			segment.program->uniformMVPmat = context.gl.mvp();
//			segment.program->uniformTextureSampler = gl::TextureChannel::diffuse;
//			segment.program->uniformColor = segment.color;
//
//			context.gl.activeTexture(gl::TextureChannel::diffuse);
//			auto txGuard = segment.font->bindGuard();
//
//			context.gl.drawArrays(vao, gl::Primitive::Triangles, segment.vertexCount, segment.vertexStart);
//		}
//	}
//	glUseProgram(0);
//
//	LIBV_GL_DEBUG_CHECK();
//}
//
//// -------------------------------------------------------------------------------------------------
//struct String2D::Builder {
//	struct Line {
//		uint32_t start = 0;
//		uint32_t last = 0;
//		vec2i size;
//	};
//
//	// TODO P2: Base font by property
//	// TODO P2: Base font program by property
//	// TODO P2: Base font color by property
//	// TODO P2: Base font size by property
//
//	// TODO P2: Hide members, use swapXYwith() member functions
//	// TODO P2: String 2D could provide resources so only one ref / resource instead of usage in string
//	//			Requires scoping and common base to references.
//	// TODO P2: Finalize function
//
//	Context& context;
//
//	std::stack<UIFont2D_view> stackFont;
//	std::stack<UIProgram_view> stackProgram;
//	std::stack<vec4f> stackColor;
//	std::stack<uint32_t> stackSize;
//
//	std::vector<String2D::Segment> segments;
//	std::vector<String2D::Character> characters;
//
//	std::vector<Line> lines;
//
//	uint32_t previousChar = 0;
//	uint32_t currentChar = 0;
//
//	int32_t pen = 0;
//	vec2i boundingBox;
//	vec2i layoutSize;
//
//private:
//	void outputText(const std::string& text) {
//		auto& currentFont = stackFont.top();
//		const auto& currentSize = stackSize.top();
//
//		const auto outputNewLine = [&] {
//			currentChar = 0;
//			pen = 0;
//			lines.back().last = characters.size();
//
//			lines.emplace_back();
//
//			lines.back().start = characters.size();
//			lines.back().size = vec2i(0, 0);
//		};
//
//		for (const auto& codePoint : text | view::uft8_codepoints) {
//			if (codePoint == '\n') {
//				outputNewLine();
//				continue;
//			}
//
//			previousChar = currentChar;
//			currentChar = codePoint;
//
//			const auto& currentGlyph = currentFont->getCharacter(currentChar, currentSize);
//			const auto& kerning = currentFont->getKerning(previousChar, currentChar, currentSize).x;
//
//			pen += kerning;
//
//			if (boundingBox.x > 0 && (pen + currentGlyph.advance.x) > boundingBox.x)
//				outputNewLine();
//
//			characters.emplace_back(vec2i(pen, 0), currentGlyph);
//			pen += currentGlyph.advance.x;
//
//			lines.back().size.x = pen;
//			lines.back().size.y = std::max(lines.back().size.y, currentFont->getLineAdvance(currentSize));
//		}
//	}
//
//	void outputTag() {
//		segments.back().vertexCount = characters.size() * 6 - segments.back().vertexStart;
//		segments.emplace_back(characters.size() * 6,
//				stackFont.top(), stackProgram.top(), stackColor.top(), stackSize.top());
//	}
//
//public:
//
//	void process(const std::string& raw) {
//		const auto tags = parse::parseTextTag(raw);
//
//		auto safe_pop = [](auto& stack) {
//			if (stack.size() > 1)
//				stack.pop();
//		};
//
//		lines.emplace_back();
//		lines.back().start = 0;
//		lines.back().size = vec2i(0, 0);
//
//		for (const auto& tag : tags) {
//			if (tag.name.empty()) {
//				outputText(tag.value);
//				continue;
//			}
//
//			if (tag.close) {
//				if (tag.name == "program")
//					safe_pop(stackProgram);
//				else if (tag.name == "font")
//					safe_pop(stackFont);
//				else if (tag.name == "size")
//					safe_pop(stackSize);
//				else if (tag.name == "color")
//					safe_pop(stackColor);
//			} else {
//				if (tag.name == "program")
//					stackProgram.emplace(context.resource.program(tag.value));
//				else if (tag.name == "font")
//					stackFont.emplace(context.resource.font(boost::filesystem::path(tag.value)));
//				else if (tag.name == "size")
//					stackSize.emplace(std::stoi(tag.value));
//				else if (tag.name == "color")
//					stackColor.emplace(parse::to_color(tag.value));
//			}
//
//			outputTag();
//		}
//
//		lines.back().last = characters.size();
//
//		for (const auto& line : lines) {
//			layoutSize.x = std::max(layoutSize.x, line.size.x);
//			layoutSize.y += line.size.y;
//
//			for (size_t i = line.start; i < line.last; ++i)
//				for (auto& vertex : characters[i].vertices)
//					vertex.position.y -= layoutSize.y;
//		}
//
//		segments.back().vertexCount = characters.size() * 6 - segments.back().vertexStart;
//	}
//
//	Builder(Context& context, vec2i maxSize) :
//		context(context),
//		stackFont({context.resource.font("res/font/default.ttf")}),
//		stackProgram({context.resource.program("res/shader/font")}),
//		stackColor({context.config.defaultFontColor}),
//		stackSize({context.config.defaultFontSize}),
//		boundingBox(maxSize) {
//		segments.emplace_back(0, stackFont.top(), stackProgram.top(), stackColor.top(), stackSize.top());
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//void String2D::buildImpl(Context& context) {
//	LIBV_LOG_UI_COMPONENT_TRACE("Build string2D [%s]", rawText);
//	Builder builder(context, maxSize);
//
//	builder.process(rawText);
//
//	layoutedSize = builder.layoutSize;
//	segments.swap(builder.segments);
//	characters.swap(builder.characters);
//
//	if (!vbo) {
//		vbo.create();
//		vao.create();
//
//		vao.bindAttribute(vbo, gl::Attribute<libv::vec2f>(0), sizeof (Character::Vertex), offsetof(Character::Vertex, position), false);
//		vao.bindAttribute(vbo, gl::Attribute<libv::vec2f>(8), sizeof (Character::Vertex), offsetof(Character::Vertex, texture0), false);
//	}
//
//	vbo.data(characters.data(), characters.size() * sizeof (characters[0]), gl::BufferUsage::StaticDraw);
//
//	// TODO P3: bind attribute based on member pointer
//	//	vao.bindAttribute(vbo, 0, &Character::Vertex::vertexCoord, false);
//	//	vao.bindAttribute(vbo, 8, &Character::Vertex::textureCoord, false);
//
//	LIBV_GL_DEBUG_CHECK();
//	LIBV_LOG_UI_COMPONENT_TRACE("Built string2D [%s]", rawText);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv
