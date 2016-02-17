// File: panel.hpp, Created on 2015. április 8. 14:32, Author: Vader

#pragma once

#include <libv/ui/container/panel.hpp>

// TODO P4: Adopt NewLine property

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFlow : public BasePanel {
public:
	using Align = size_t;
	using Orient = size_t;

	static const Align ALIGN_BOTTOM_CENTER;
	static const Align ALIGN_BOTTOM_LEFT;
	static const Align ALIGN_BOTTOM_RIGHT;
	static const Align ALIGN_CENTER_CENTER;
	static const Align ALIGN_CENTER_LEFT;
	static const Align ALIGN_CENTER_RIGHT;
	static const Align ALIGN_TOP_CENTER;
	static const Align ALIGN_TOP_LEFT;
	static const Align ALIGN_TOP_RIGHT;

	static const Orient ORIENT_UP_LEFT;
	static const Orient ORIENT_UP_RIGHT;
	static const Orient ORIENT_DOWN_LEFT;
	static const Orient ORIENT_DOWN_RIGHT;
	static const Orient ORIENT_LEFT_UP;
	static const Orient ORIENT_LEFT_DOWN;
	static const Orient ORIENT_RIGHT_UP;
	static const Orient ORIENT_RIGHT_DOWN;

private:
	Align align = ALIGN_BOTTOM_LEFT;
	Align alignContent = ALIGN_TOP_LEFT;
	Orient orient = ORIENT_RIGHT_DOWN;

public:
	PanelFlow() = default;
	PanelFlow(Orient orientation, Align align, Align alignContent);
	virtual ~PanelFlow() = default;

public:
	void setOrient(Orient orient);
	void setAlign(Align align);
	void setAlignContent(Align alignContent);

private:
	virtual void doBuild(Renderer& renderer) override;
	void layoutImpl();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
