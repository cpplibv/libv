// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

#include <libv/ui/container/container.hpp>
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFlow : public Container {
public:
	PanelFlow() = default;
	virtual ~PanelFlow() = default;

public:
	inline void setAlign(LayoutAlign align) {
		set(Property::Align, align);
	}
	inline void setAnchor(LayoutAlign content) {
		set(Property::Anchor, content);
	}
	inline void setOrient(LayoutOrient orient) {
		set(Property::Orient, orient);
	}

private:
	virtual LayoutInfo doLayout(const LayoutInfo& parentLayout) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
