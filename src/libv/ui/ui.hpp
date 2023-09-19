// Project: libv.ui, File: src/libv/ui/ui.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/gl/fwd.hpp>
#include <libv/input/event_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <functional>
#include <memory>
#include <string>
// pro
#include <libv/ui/event_system/event_host.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.ui: Implement global Setting and local property to disable Font2D LCD sub pixel rendering (both are required to be true to enable it)
// TODO P2: libv.ui: Resource debugger: font and texture view at first
// TODO P2: libv.ui: UI Debugger: Component stack on Hover (needs std::vector<observer_ptr<CoreComponent>> getComponentAt(int, int);) | Not just mouse region based!
// TODO P2: libv.ui: Record or report create/layout/render/destroy statistics
// TODO P3: libv.ui: Idea reminder: glr could be more light with signed objects, and this might be a completely different approve
//			<0 means look at some global glr store, and its only a promise, in the future there will be an object
//			=0 default ctor, noop
//			>0 actual opengl object index
// TODO P5: libv.ui: Render ui into a separate frame buffer, or option set its target
// TODO P5: libv.ui: (?) void setFocusPolicy(...);

class ImplUI;

class UI {
	friend ContextUI;

private:
	std::shared_ptr<ImplUI> self;

public:
	UI();
	explicit UI(Settings setting);
	~UI();

public:
	void add(Component component);
	void remove(Component component);

	void setSize(libv::vec2i size_) noexcept;
	inline void setSize(int32_t x, int32_t y) noexcept {
		setSize({x, y});
	}

public:
	/// This is the only thread-safe operation on UI, every other member function can only be called from the main UI thread
	void execute_in_ui_loop(std::function<void()> func);

	void foreach_recursive_component(libv::function_ref<void(const Component&)> func);

public:
	/// Loads a lua style script file
	void load_style_script_file(std::string path);

public:
	void event(const libv::input::EventChar& event);
	void event(const libv::input::EventKey& event);
	void event(const libv::input::EventMouseButton& event);
	void event(const libv::input::EventMouseEnter& event);
	void event(const libv::input::EventMousePosition& event);
	void event(const libv::input::EventMouseScroll& event);

public:
	[[nodiscard]] EventHostGlobal<Component> event();
	[[nodiscard]] EventHub event_hub();

public:
	[[nodiscard]] libv::GL& gl();
	[[nodiscard]] libv::Nexus& nexus();

	[[nodiscard]] ContextState& state();
	[[nodiscard]] ContextResource& resource();
	[[nodiscard]] ContextStyle& style();

	[[nodiscard]] ContextUI& context();

	template <typename Frame> void attach(Frame& frame);

public:
	void create();
	void update();
	void destroy();

private:
	void focus(CoreComponent& component);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);
};

// -------------------------------------------------------------------------------------------------

template <typename Frame>
void UI::attach(Frame& frame) {
//	frame.onSize.output([this](const auto& e) {
//		this->setSize(e.size);
//	});
	frame.onFramebufferSize.output([this](const auto& e) {
		this->setSize(e.size);
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

	frame.onContextCreate.output([&](const auto&) {
		this->create();
	});
	frame.onContextUpdate.output([&](const auto&) {
		this->update();
	});
	frame.onContextDestroy.output([&](const auto&) {
		this->destroy();
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
