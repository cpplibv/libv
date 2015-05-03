// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

//pro
#include "vl/ui/component/component.hpp"
#include "vl/ui/render/default_renderer.hpp"

namespace vl {
namespace ui {

class Label : public Component {
private:
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
