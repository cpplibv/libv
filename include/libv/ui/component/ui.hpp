// File:   ui.hpp Author: Vader Created on 2016. január 7., 6:35

#pragma once

// libv
#include <libv/memory.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/container/panel_flow.hpp>
#include <libv/ui/render/context.hpp>

// TODO P3: Create UIID and UIClass string "aliases" for components and use it to address string
//		ambiguity on constructors. Name could vary, but suggested to stay short
// TODO P4: UI Event should not come from directly from frame but from open signals? (remove attach)
// TODO P4: Render ui into a separate frame buffer, or option set its target

namespace libv {
namespace frame {
class Frame;
} // namespace frame
using Frame = frame::Frame;

namespace ui {

// -------------------------------------------------------------------------------------------------

// UI process orders:
//
// events
// IO tasks
// GL tasks
// build
// layout
// render
// destroy

// -------------------------------------------------------------------------------------------------

class Component;

class UI {
private:
	Context context; //Layout layout;
	PanelFlow content; // TODO P5: Change to a more primitive panel

	vec2i position;
	vec2i size;

	bool valid = false;

public:
	Context& getContext();
	vec2i getSize() const;
	void setSize(vec2i size);

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

} // namespace ui
} // namespace libv