// File:   ui.hpp Author: Vader Created on 2016. január 7., 6:35

#pragma once

// vl
#include <libv/memory.hpp>
#include <libv/vec.hpp>
// pro
//#include <libv/ui/container/panel.hpp>
#include <libv/ui/container/panel_flow.hpp>
#include <libv/ui/render/renderer.hpp>

// TODO P3: Create UIID and UIClass string "aliases" for components and use it to address string
//		ambiguity on constructors. Name could vary, but suggested to stay short
// TODO P4: UI Event should not come from directly from frame but from open signals? (remove attach)
// TODO P4: Render ui into a separate frame buffer, or option set its target

// Random statements about UI:
// Not knowing your parent is fun
// Not remembering your roots is kind of fun too
// Container is not a component
// Merging layout management with container should improve our system

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Frame;
class Component;

class UI {
private:
	Renderer gl; //<<< Layout layout;
	PanelFlow content; //TODO P5: Change to a more primitive panel

	ivec2 position;
	ivec2 size;

	bool valid = false;

private:
	bool isInvalid() const;
	void invalidate();
	void validate();

public:
	ivec2 getPosition() const;
	void setPosition(ivec2 position);
	ivec2 getSize() const;
	void setSize(ivec2 size);

private:
	void build();
	void destroy();
	void render();

public:
	void attach(observer_ptr<Frame> frame);
	void refresh();

	void add(const observer_ptr<Component>& component);
	void add(const shared_ptr<Component>& component);

public:
	UI();
	virtual ~UI();
};


// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv