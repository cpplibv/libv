// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

// vl
#include <ui/font/font_2D.hpp>
//pro
#include <vl/ui/component/component.hpp>

namespace vl {
namespace ui {

class Label : public Component {
private:
	Font2D deafultFont;
	std::string text;
	//Font2D font;
public:
	void setText(const std::string& text);
public:
	virtual void build(Renderer& renderer) override;
	virtual void destroy(Renderer& renderer) override;
	virtual void render(Renderer& renderer) override;
};

} //namespace ui
} //namespace vl
