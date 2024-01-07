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
#include <libv/utility/unique_function.hpp>
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
// TODO P2: libv.ui: UI Debugger: Component stack on Hover (Not just mouse region based!) Go for full scan
// TODO P2: libv.ui: Record or report create/layout12/render/destroy statistics

struct EventGLCreate {
	libv::GL& gl;
};

struct EventGLDestroy {
	libv::GL& gl;
};

// -------------------------------------------------------------------------------------------------

struct UnhandledInputEventHandler {
	virtual ~UnhandledInputEventHandler() = default;
	virtual void event(const libv::input::EventChar& event, bool handled) = 0;
	virtual void event(const libv::input::EventKey& event, bool handled) = 0;
	virtual void event(const libv::input::EventMouseButton& event, bool handled) = 0;
	virtual void event(const libv::input::EventMouseEnter& event, bool handled) = 0;
	virtual void event(const libv::input::EventMousePosition& event, bool handled) = 0;
	virtual void event(const libv::input::EventMouseScroll& event, bool handled) = 0;
	virtual void event(const libv::input::EventGamepadAnalog& event, bool handled) = 0;
	virtual void event(const libv::input::EventGamepadButton& event, bool handled) = 0;
	virtual void event(const libv::input::EventJoystickButton& event, bool handled) = 0;
	virtual void event(const libv::input::EventJoystickAnalog& event, bool handled) = 0;
	virtual void update(libv::ui::time_duration deltaTime) = 0;
};

// -------------------------------------------------------------------------------------------------

class ImplUI;

class UI {
	friend ContextUI;

private:
	std::shared_ptr<ImplUI> self;

public:
	UI();
	explicit UI(Settings setting);
	UI(libv::Nexus nexus, Settings setting);
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
	void event(const libv::input::EventGamepadAnalog& event);
	void event(const libv::input::EventGamepadButton& event);
	void event(const libv::input::EventJoystickButton& event);
	void event(const libv::input::EventJoystickAnalog& event);

	void unhandledEventHandler(std::unique_ptr<UnhandledInputEventHandler> handler);

public:
	[[nodiscard]] EventHostGlobal<Component> event();

	void subscribe(libv::unique_function<void(const EventGLCreate&)>);
	void subscribe(libv::unique_function<void(const EventGLDestroy&)>);

public:
	[[nodiscard]] libv::GL& gl();
	[[nodiscard]] libv::Nexus& nexus();

	[[nodiscard]] ContextState& state();
	[[nodiscard]] ContextResource& resource();
	[[nodiscard]] ContextStyle& style();

	[[nodiscard]] ContextUI& context();

	template <typename Frame> void attach(Frame& frame);
	template <typename Controls> void attachControls(Controls& controls);

public:
	void create();
	void update();
	void destroy();
};

// -------------------------------------------------------------------------------------------------

template <typename Frame>
void UI::attach(Frame& frame) {
	// frame.onSize is not used, onFramebufferSize is more accurate
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

	frame.onGamepadAnalog.output([this](const auto& e) {
		this->event(e);
	});
	frame.onGamepadButton.output([this](const auto& e) {
		this->event(e);
	});
	frame.onJoystickAnalog.output([this](const auto& e) {
		this->event(e);
	});
	frame.onJoystickButton.output([this](const auto& e) {
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

template <typename Controls>
void UI::attachControls(Controls& controls) {
	struct Handler : UnhandledInputEventHandler {
		Controls& controls;

		explicit Handler(Controls& controls) : controls(controls) {}

		virtual void event(const libv::input::EventChar& event, bool handled) override {
			(void) event;
			(void) handled;
		}
		virtual void event(const libv::input::EventKey& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventMouseButton& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventMouseEnter& event, bool handled) override {
			(void) event;
			(void) handled;
		}
		virtual void event(const libv::input::EventMousePosition& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventMouseScroll& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventGamepadAnalog& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventGamepadButton& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventJoystickButton& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void event(const libv::input::EventJoystickAnalog& event, bool handled) override {
			controls.event(event, !handled);
		}
		virtual void update(libv::ui::time_duration deltaTime) override {
			controls.update(deltaTime);
		}
	};

	this->unhandledEventHandler(std::make_unique<Handler>(controls));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
