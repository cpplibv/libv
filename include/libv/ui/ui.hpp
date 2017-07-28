// File: pcui.hpp Author: Vader Created on 2017. augusztus 24., 22:32

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/timer.hpp>
// std
#include <memory>
#include <vector>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.ui: UI Option to disable Font2D LCD sub pixel render
// TODO P2: libv.ui: Resource debugger, font and texture view at first
// TODO P2: libv.ui: Component stack on Hover
// TODO P2: libv.ui: Record create/layout/render/destroy statistics
// TODO P3: Idea reminder: glr could be more light with signed objects, and this might be a completely different approve
//			<0 means look at some global glr store, and its only a promise, in the future there will be an object
//			=0 default ctor, noop
//			>0 actual opengl object index
// TODO P4: UI Event should not come from directly from frame but from open signals? (remove attach)
// TODO P5: Render ui into a separate frame buffer, or option set its target
// TODO P5: Create UIID and UIClass string "aliases" for components and use it to address string
//		ambiguity on constructors. Names could vary, but suggested to stay short
// TODO P5: (?) void setFocusPolicy(...);
// TODO P5: (?) std::vector<adaptive_ptr<Component>> findComponentAt(int, int);
// TODO P5: (?) std::vector<adaptive_ptr<Component>> getComponentAt(int, int);

class ComponentBase;
class ContextUI;
class Root;

struct UI {
private:
	std::unique_ptr<ContextUI> context_ui;
	std::unique_ptr<Root> root;
	libv::Timer timer;

public:
	UI();
	~UI();
	void add(std::shared_ptr<ComponentBase> component);
	void setSize(libv::vec3f size_) noexcept;
	void setSize(float x, float y, float z = 0.f) noexcept;
	void setPosition(libv::vec3f position_) noexcept;
	void setPosition(float x, float y, float z) noexcept;

public:
	ContextUI& context() {
		return *context_ui;
	}
	template <typename Frame> void attach(Frame& frame);

private:
	void attach();
	void layout();
	void render(libv::glr::Queue& gl);

public: // <<< P1: libv.ui: I disagree with this public, to resolve this I think UI has to store its glr::Remote?
	void create(libv::glr::Queue& gl);
	void update(libv::glr::Queue& gl);
	void destroy(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

template <typename Frame>
void UI::attach(Frame& frame) {
	(void) frame;
//		frame.onContextInitialization.output([this](const auto&) {
//			this->create();
//		});
//		frame.onContextRefresh.output([this](const auto&) {
//			this->update();
//		});
//		frame.onContextTerminate.output([this](const auto&) {
//			this->destroy();
//		});

	//	frame->onWindowSize.output([this](auto e) {
	//		this->setSize(e.size);
	//		this->content.set(Property::Size, px(e.size.x), px(e.size.y), 0);
	//	});
	//	frame->onClosed.output([this](auto /*frame*/) {
	//		this->destroy();
	//	});

	//	frame.onContextInitialization.output(create, this);
	//	frame.onContextRefresh.output(update, this);
	//	frame.onContextTerminate.output(destroy, this);

	//	frame.onKey();
	//	frame.onChar();
	//	frame.onMouse();
	//	frame.onMouseButton();
	//	frame.onMouseLeave();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
