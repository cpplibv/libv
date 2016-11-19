// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

// libv
#include <libv/ui/font/string_2D.hpp>
//pro
#include <libv/ui/component/component.hpp>

namespace libv {
namespace ui {

class Label : public Component {
private:
	String2D text;
public:
	Label() = default;
	Label(const std::string& componentID, const std::string& componentClass) :
		Component(componentID, componentClass) { }

public:
	void setText(const std::string& text);
	std::string getText() const {
		return text.getText();
	}

protected:
	virtual void doBuild(Renderer& renderer) override;
	virtual void doDestroy(Renderer& renderer) override;
	virtual void doRender(Renderer& renderer) override;
};

} //namespace ui
} //namespace libv
