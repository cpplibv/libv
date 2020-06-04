// File: pcui.hpp Author: Vader Created on 2017. augusztus 24., 22:32

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
#include <libv/input/event_fwd.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.ui: Implement Setting to disable Font2D LCD sub pixel rendering
// TODO P2: libv.ui: Resource debugger, font and texture view at first
// TODO P2: libv.ui: UI Debugger: Component stack on Hover (needs std::vector<observer_ptr<BaseComponent>> getComponentAt(int, int);)
// TODO P2: libv.ui: Record create/layout/render/destroy statistics
// TODO P3: libv.ui: Idea reminder: glr could be more light with signed objects, and this might be a completely different approve
//			<0 means look at some global glr store, and its only a promise, in the future there will be an object
//			=0 default ctor, noop
//			>0 actual opengl object index
// TODO P5: libv.ui: Render ui into a separate frame buffer, or option set its target
// TODO P5: libv.ui: (?) void setFocusPolicy(...);

class BaseComponent;
class Component;
class ContextState;
class ContextUI;
class ImplUI;
class Settings;

class UI {
	friend class ContextUI;

private:
	std::unique_ptr<ImplUI> self;

public:
	UI();
	explicit UI(const Settings& setting);
	~UI();

public:
	void add(Component component);

	void setSize(libv::vec2i size_) noexcept;
	inline void setSize(int32_t x, int32_t y) noexcept {
		setSize({x, y});
	}

public:
	void event(const libv::input::EventChar& event);
	void event(const libv::input::EventKey& event);
	void event(const libv::input::EventMouseButton& event);
	void event(const libv::input::EventMouseEnter& event);
	void event(const libv::input::EventMousePosition& event);
	void event(const libv::input::EventMouseScroll& event);

public:
	ContextState& state();
	ContextUI& context();
	template <typename Frame> void attach(Frame& frame);

public:
	// TODO P1: libv.ui I disagree with this public, to resolve this I think UI has to store its glr::Remote?, which is reasonable, might need some alteration
	void update(libv::glr::Queue& gl);
	void destroy(libv::glr::Queue& gl);

private:
	void focus(BaseComponent& component);
	void detachFocused(BaseComponent& component);
	void detachFocusLinked(BaseComponent& component);
};

// -------------------------------------------------------------------------------------------------

template <typename Frame>
void UI::attach(Frame& frame) {
	frame.onSize.output([this](const auto& e) {
		this->setSize(e.size);
	});
//	frame.onFramebufferSize.output([this](const auto& e) {
//		this->setFramebufferSize(e.size);
//	});

	frame.onChar.output([this](const auto& e) {
		this->event(e);
	});
	frame.onKey.output([this](const auto& e) {
		this->event(e);
	});

	frame.onMouseEnter.output([this](const auto& e) {
		this->event(e);
	});
	frame.onMouseButton.output([this](const auto& e) {
		this->event(e);
	});
	frame.onMousePosition.output([this](const auto& e) {
		this->event(e);
	});
	frame.onMouseScroll.output([this](const auto& e) {
		this->event(e);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
