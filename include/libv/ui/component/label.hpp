// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

// libv
#include <libv/ui/font/font_2D.hpp>
//pro
#include <libv/ui/component/component.hpp>

namespace libv {
namespace ui {

class Label : public Component {
private:
	Font2D deafultFont;
	// String2D text;
	std::string text;
	// Font2D font;
public:
	void setText(const std::string& text);
protected:
	virtual void doBuild(Renderer& renderer) override;
	virtual void doDestroy(Renderer& renderer) override;
	virtual void doRender(Renderer& renderer) override;
};

} //namespace ui
} //namespace libv
