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
// TODO P3: Idea reminder: glr could be more light with signed objects, and this might be a completely different approve
//			<0 means look at some global glr store, and its only a promise, in the future there will be an object
//			=0 default ctor, noop
//			>0 actual opengl object index
// TODO P5: Render ui into a separate frame buffer, or option set its target
// TODO P5: (?) void setFocusPolicy(...);

class BaseComponent;
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
	void add(std::shared_ptr<BaseComponent> component);
	void setSize(libv::vec3f size_) noexcept;
	void setSize(float x, float y, float z = 0.f) noexcept;
	void setPosition(libv::vec3f position_) noexcept;
	void setPosition(float x, float y, float z) noexcept;
	BaseComponent& root() const noexcept; // TODO P5: I would really like to not give access to the root

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
		this->setSize(static_cast<float>(e.size.x), static_cast<float>(e.size.y));
	});

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
