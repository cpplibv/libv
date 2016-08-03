// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/font/string_2D.hpp>
// ext
#include <boost/filesystem/path.hpp>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>
// pro
#include <libv/ui/font/font_2D.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

String2D::String2D(const std::string& text, Anchor halign, Anchor valign) :
	halign(halign),
	valign(valign),
	rawText(text) { }


// -------------------------------------------------------------------------------------------------

void String2D::setText(const std::string& text) {
	rawText = text;
}

void String2D::setSize(ivec2 size) {
	this->maxSize = size;
}

// -------------------------------------------------------------------------------------------------

void String2D::build() {
	if (!deafultFont.isLoaded()) {
		LIBV_UI_COMPONENT_TRACE("Load Font");
		deafultFont.setSize(12);
		deafultFont.load("res/font/cour.ttf");
		checkGL();
	}

	if (!defaultShader) {
		LIBV_UI_COMPONENT_TRACE("Load Shader");

		auto vs = std::make_shared<gl::ShaderVertex>("res/shader/font.vs");
		auto fs = std::make_shared<gl::ShaderFragment>("res/shader/font.fs");
		defaultShader = std::make_shared<gl::ShaderProgram>("Default Font Shader", vs, fs);

		uniformTextureSampler.assign(*defaultShader, "textureSampler");
		uniformMVPmat.assign(*defaultShader, "MVPmat");
		checkGL();
	}

	buildImpl();
	checkGL();
}

void String2D::destroy() {
	if (deafultFont.isLoaded()) {
		LIBV_UI_COMPONENT_TRACE("Unload Font");
		deafultFont.unload();
	}
	defaultShader.reset();
	checkGL();
}

void String2D::render(Renderer& gl) {
	gl.pushMatrixModel();
	gl.matrixModel() *= glm::translate(glm::vec3(0, -deafultFont.getDescender(), 0));

	defaultShader->use();
	deafultFont.bind();
	checkGL();

	uniformMVPmat = gl.matrixMVP();
	uniformTextureSampler = gl::TextureType::diffuse;
	checkGL();

	glBegin(GL_TRIANGLES);
	for (const auto& ch : data) {
		for (int i = 0; i < 6; i++) {
			glTexCoord2f(ch.textureCoord[i].x, ch.textureCoord[i].y);
			glVertex3f(ch.vertexCoord[i].x, ch.vertexCoord[i].y, 0);
		}
	}
	glEnd();
	checkGL();

	deafultFont.unbind();
	glUseProgram(0);
	checkGL();
	gl.popMatrixModel();
}

// -------------------------------------------------------------------------------------------------

void String2D::buildImpl() {
	data.clear();

	auto iter = rawText.begin();
	auto end = rawText.end();
	ivec2 pen;
	uint32_t prev = 0;

	size_t lineCount = 1;
	int width = 0;

	const auto newLine = [&]{
		width = std::max(width, pen.x);
		++lineCount;
		pen.x = 0;
		pen.y += deafultFont.getLineAdvance();
	};

	while (iter != end) {
		auto current = utf8::next(iter, end);
		// TODO P2: handle 'invalid' utf8 exceptions (handle should be on set!)

		if (current == '\n') {
			newLine();
			prev = current;
			continue;
		}

		auto fontCharInfo = deafultFont.getCharacter(current);
		pen += deafultFont.getKerning(prev, current);

		Character charInfo;
		charInfo.textureCoord[0] = fontCharInfo.textureCoord[0];
		charInfo.textureCoord[1] = fontCharInfo.textureCoord[1];
		charInfo.textureCoord[2] = fontCharInfo.textureCoord[2];
		charInfo.textureCoord[3] = fontCharInfo.textureCoord[2];
		charInfo.textureCoord[4] = fontCharInfo.textureCoord[3];
		charInfo.textureCoord[5] = fontCharInfo.textureCoord[0];

		charInfo.vertexCoord[0] = pen + fontCharInfo.vertexCoord[0];
		charInfo.vertexCoord[1] = pen + fontCharInfo.vertexCoord[1];
		charInfo.vertexCoord[2] = pen + fontCharInfo.vertexCoord[2];
		charInfo.vertexCoord[3] = pen + fontCharInfo.vertexCoord[2];
		charInfo.vertexCoord[4] = pen + fontCharInfo.vertexCoord[3];
		charInfo.vertexCoord[5] = pen + fontCharInfo.vertexCoord[0];

		pen.x += fontCharInfo.advance.x;
		if (maxSize.x > 0 && pen.x > maxSize.x)
			newLine();

		data.push_back(charInfo);
		prev = current;
	}
	width = std::max(width, pen.x);

	layoutedSize = ivec2(width, lineCount * deafultFont.getLineAdvance()) ;
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv


//	glBegin(GL_QUADS);
//	glColor3f(1, 1, 1);
//	glTexCoord2f(0, 0);
//	glVertex2f(-1.0f, -1.0f);
//	glTexCoord2f(1, 0);
//	glVertex2f(+1.0f, -1.0f);
//	glTexCoord2f(1, 1);
//	glVertex2f(+1.0f, +1.0f);
//	glTexCoord2f(0, 1);
//	glVertex2f(-1.0f, +1.0f);
//	glEnd();


//	glBegin(GL_QUADS);
//	glColor3f(1, 1, 1);
//	glTexCoord2f(0, 0);
//	glVertex2f(0, 0);
//	glTexCoord2f(1, 0);
//	glVertex2f(512, 0);
//	glTexCoord2f(1, 1);
//	glVertex2f(512, 512);
//	glTexCoord2f(0, 1);
//	glVertex2f(0, 512);
//	glEnd();