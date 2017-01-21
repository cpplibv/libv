// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

// pro
#include <libv/ui/component/string_2D.hpp>
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
	const std::string& getText() const {
		return text.getText();
	}

// TODO P2: PropertyArchive, serialize_property
//	template <typename Archive, typename Descriptor>
//	void serialize_property(Archive& ar) {
//		ar & Descriptor(Property::Size, Mandatory, "Determines the maximum size of the text");
//		ar & Descriptor(Property::Boundary, Optional, "Determines the boundary policy");
//		ar & Descriptor(Property::Align, Default(LEFT), "Alignment of the text");
//		ar & Descriptor(Property::Orient, Default(RIGHT), "Orientation of the text");
//	}
protected:
	virtual LayoutInfo doLayout(const LayoutInfo& parentLayout) override;
	virtual void doBuild(Context& context) override;
	virtual void doDestroy(Context& context) override;
	virtual void doRender(Context& context) override;
};

} // namespace ui
} // namespace libv
