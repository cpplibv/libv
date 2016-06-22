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

// TODO P2: PropertyArchive, serialize_property
//	template <typename PropertyArchive>
//	void serialize_property(PropertyArchive& ar) {
//		ar & PropertyDescriptor(Property::Size, Mandatory, "Determines the maximum size of the text");
//		ar & PropertyDescriptor(Property::Boundary, Optional, "Determines the boundary policy");
//		ar & PropertyDescriptor(Property::Align, Default(LEFT), "Alignment of the text");
//		ar & PropertyDescriptor(Property::Orient, Default(RIGHT), "Orientation of the text");
//	}
protected:
	virtual LayoutInfo doLayout(const LayoutInfo& parentLayout) override;
	virtual void doBuild(Renderer& renderer) override;
	virtual void doDestroy(Renderer& renderer) override;
	virtual void doRender(Renderer& renderer) override;
};

} //namespace ui
} //namespace libv
