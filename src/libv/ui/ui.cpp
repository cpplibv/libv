// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/input/event.hpp>
#include <libv/math/remap.hpp>
#include <libv/utility/observer_ptr.hpp>
#include <libv/utility/observer_ref.hpp>
#include <libv/utility/overload.hpp>
#include <libv/utility/timer.hpp>
// std
#include <array>
#include <mutex>
#include <variant>
#include <vector>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/context_focus_travers.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_mouse.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_state.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/context_ui_link.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/overlay_zoom.lpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <size_t N>
struct Histogram {
	std::array<size_t, N> data;

	std::chrono::nanoseconds min;
	std::chrono::nanoseconds max;

	Histogram(std::chrono::nanoseconds min, std::chrono::nanoseconds max) : min(min), max(max) {
		data.fill(0);
	}

	void sample(std::chrono::nanoseconds value) {
		if (value > max)
			data[N - 1]++;
		else if (value < min)
			data[0]++;
		else
			data[(value - min).count() / ((max - min).count() / N)]++;
	}

	friend std::ostream& operator<<(std::ostream& os, const Histogram& var) {
		for (size_t i = 0; i < N; ++i)
			os << var.data[i] << ' ';
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

using Root = PanelFull;

//class Root : public PanelFull {
//public:
//	using PanelFull::PanelFull;
//
//public:
//	void setSize(libv::vec3f size_) {
//		AccessRoot::size(base()) = size_;
//		AccessRoot::flagAuto(base(), Flag::pendingLayout);
//	}
//};

// -------------------------------------------------------------------------------------------------

class ImplUI {
	struct Stat {
		static constexpr std::chrono::microseconds t_min{0};
		static constexpr std::chrono::milliseconds t_max{1};

		Histogram<100> attach1{t_min, t_max};
		Histogram<100> event{t_min, t_max};
		Histogram<100> attach2{t_min, t_max};
		Histogram<100> styleScan{t_min, t_max};
		Histogram<100> style{t_min, t_max};
		Histogram<100> layout1{t_min, t_max};
		Histogram<100> layout2{t_min, t_max};
		Histogram<100> render{t_min, t_max};
		Histogram<100> detach{t_min, t_max};

		Histogram<100> frame{t_min, t_max};

		friend std::ostream& operator<<(std::ostream& os, const Stat& var) {
			os << "attach1:   " << var.attach1;
			os << "\nevent:     " << var.event;
			os << "\nattach2:   " << var.attach2;
			os << "\nstyleScan: " << var.styleScan;
			os << "\nstyle:     " << var.style;
			os << "\nlayout1:   " << var.layout1;
			os << "\nlayout2:   " << var.layout2;
			os << "\nrender:    " << var.render;
			os << "\ndetach:    " << var.detach;
			os << "\n-------------------------------------------------------------------------------------";
			os << "\nframe:     " << var.frame;
			os << '\n';
			return os;
		}
	};

public:
	using EventVariant = std::variant<
			libv::input::EventChar,
			libv::input::EventKey,
			libv::input::EventMouseButton,
			libv::input::EventMouseEnter,
			libv::input::EventMousePosition,
			libv::input::EventMouseScroll
	>;

public:
	ContextState context_state;
	ContextUI context;
	Root root;

	Stat stat;
	libv::Timer timer;
	libv::Timer timerFrame;

	std::vector<EventVariant> event_queue;
	std::mutex mutex;

public:
	OverlayZoomMode overlayZoomMode = OverlayZoomMode::disabled;
	OverlayZoom overlayZoom;
	// TODO P1: make sure zoom is px aligned and the lines are pixel perfect

//	bool overlayCursorEnable = false;
//	std::shared_ptr<OverlayCursor> overlayCursor = std::make_shared<OverlayCursor>(root);

//	std::shared_ptr<OverlayPref> overlayPref = std::make_shared<OverlayPref>(root);
//	std::shared_ptr<OverlayStack> overlayStack = std::make_shared<OverlayStack>(root);

public:
	ImplUI(UI& ui) :
		context(ui, context_state),
		root((current_thread_context(context), "")) { }

	ImplUI(UI& ui, const Settings& settings) :
		context(ui, context_state, settings),
		root((current_thread_context(context), "")) { }

public:
	void focus(libv::observer_ptr<BaseComponent> old_focus, libv::observer_ptr<BaseComponent> new_focus) {
		if (new_focus == old_focus)
			return;

		if (old_focus == nullptr) {
			log_ui.trace("Focus set to: {}", new_focus->path());
			AccessRoot::focusGain(*new_focus);

		} else if (new_focus == nullptr) {
			log_ui.trace("Focus clear from: {}", old_focus->path());
			AccessRoot::focusLoss(*old_focus);

		} else {
			log_ui.trace("Focus set from: {} to: {}", old_focus->path(), new_focus->path());
			AccessRoot::focusLoss(*old_focus);
			AccessRoot::focusGain(*new_focus);
		}
	}

	libv::observer_ptr<BaseComponent> focusTravers(libv::observer_ptr<BaseComponent> old_focus, Degrees<float> direction) {
		ContextFocusTravers ctx{direction};

		libv::observer_ptr<BaseComponent> new_focus = nullptr;

		if (old_focus != nullptr) // Traverse to next
			new_focus = AccessRoot::focusTravers(*old_focus, ctx);

		if (new_focus == nullptr) // End reached, Loop around
			new_focus = AccessRoot::focusTravers(root.base(), ctx);

		focus(old_focus, new_focus);
		return new_focus;
	}

public:
	void event(const libv::input::EventChar& event) {
		if (context_state.focus_ == nullptr)
			return;

		AccessRoot::eventChar(*context_state.focus_, EventChar(event));
	}

	void event(const libv::input::EventKey& event) {
//		if (event.key == libv::input::Key::F12 && (event.mods & libv::input::KeyModifier::shift) != libv::input::KeyModifier::none) {
//		parse_hotkey_or_throw("F12")
//		parse_hotkey_or_throw("Shift + F12")

		if (event.keycode == libv::input::Keycode::F12 && event.action == libv::input::Action::press) {
			if (overlayZoomMode == OverlayZoomMode::disabled) {
				log_ui.info("Switch overlay mode: {} to {}", "zoom", "control");
				overlayZoomMode = OverlayZoomMode::control;
				root.add(overlayZoom);
				overlayZoom.control();

			} else if (overlayZoomMode == OverlayZoomMode::control) {
				log_ui.info("Switch overlay mode: {} to {}", "zoom", "view");
				overlayZoomMode = OverlayZoomMode::view;
				overlayZoom.view();

			} else if (overlayZoomMode == OverlayZoomMode::view) {
				log_ui.info("Switch overlay mode: {} to {}", "zoom", "disabled");
				overlayZoomMode = OverlayZoomMode::disabled;
				overlayZoom.disable();
				root.remove(overlayZoom);
			}
		}

//		if (event.key == libv::input::Key::F11 && event.action == libv::input::Action::press) {
//			overlayCursorEnable = !overlayCursorEnable;
//
//			if (overlayCursorEnable)
////				root.add(overlayZoom);
//			else
////				root.remove(overlayZoom);
//
//			log_ui.info("Switch overlay mode: {} to {}", "cursor", overlayCursorEnable ? "on" : "off");
//		}

		if (context_state.focus_ != nullptr)
			AccessRoot::eventKey(*context_state.focus_, EventKey(event));

		if (event.action != libv::input::Action::release) {
			context_state.pressed_keys.insert(event.keycode);
			context_state.pressed_scancodes.insert(event.scancode);
		} else {
			context_state.pressed_keys.erase(event.keycode);
			context_state.pressed_scancodes.erase(event.scancode);
		}
	}

	void event(const libv::input::EventMouseButton& event) {
		context.mouse.event_button(event.button, event.action);
		if (event.action != libv::input::Action::release)
			context_state.pressed_mouses.insert(event.button);
		else
			context_state.pressed_mouses.erase(event.button);
	}

	void event(const libv::input::EventMouseEnter& event) {
		if (event.entered) {
			context.mouse.event_enter();
			context_state.mouse_over_ = true;
		} else {
			context.mouse.event_leave();
			context_state.mouse_over_ = false;
		}
	}

	void event(const libv::input::EventMousePosition& event) {
		auto position = libv::vec::cast<float>(event.position);

		if (overlayZoomMode == OverlayZoomMode::view)
			// Floor is used as mouse position are in pixel center coordinates
			position = libv::vec::floor(libv::remap(position, libv::vec2f(), root.size2(), overlayZoom.screen_BL(), overlayZoom.screen_TR() + 1.0f));

		context.mouse.event_position(position);
		context_state.mouse_position_ = position;
	}

	void event(const libv::input::EventMouseScroll& event) {
		const auto offset = libv::vec::cast<float>(event.offset);
		context.mouse.event_scroll(offset);
		context_state.scroll_position_ = offset;
	}
};

// -------------------------------------------------------------------------------------------------

UI::UI() {
	self = std::make_unique<ImplUI>(*this);
	current_thread_context(context());
}

UI::UI(const Settings& settings) {
	self = std::make_unique<ImplUI>(*this, settings);
	current_thread_context(context());
}

UI::~UI() {
	clear_current_thread_context();
}

void UI::add(Component component) {
	self->root.add(std::move(component));
}

void UI::setSize(libv::vec2i size_) noexcept {
	std::unique_lock lock{self->mutex};
	AccessRoot::size(self->root.base()) = libv::vec3f{libv::vec::cast<float>(size_), 0};
	AccessRoot::flagAuto(self->root.base(), Flag::pendingLayout);
}

// -------------------------------------------------------------------------------------------------

void UI::event(const libv::input::EventChar& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventKey& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseButton& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseEnter& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMousePosition& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseScroll& event) {
	std::unique_lock lock{self->mutex};
	self->event_queue.emplace_back(event);
}

// -------------------------------------------------------------------------------------------------

ContextUI& UI::context() {
	return self->context;
}

ContextState& UI::state() {
	return self->context_state;
}

// -------------------------------------------------------------------------------------------------

void UI::update(libv::glr::Queue& gl) {
	current_thread_context(context());

	self->timer.reset();
	self->timerFrame.reset();

	self->context_state.frame_count_++;
	const auto now = clock::now();
	self->context_state.time_delta_ = now - self->context_state.time_frame_;
	self->context_state.time_frame_ = now;
	self->context_state.time_ = self->context_state.time_frame_ - self->context_state.time_ui_;

	// --- Attach ---
	try {
		AccessRoot::attach(self->root.base(), self->root.base());
		self->stat.attach1.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during attach1 in UI: {}", ex.what());
	}

	// --- Event ---
	try {
		self->context.mouse.event_update();
		// NOTE: Internal "mouse" events don't require event queue lock (these are reactive events to layout and attach changes)

		std::unique_lock lock{self->mutex};

		for (const auto& inputEvent : self->event_queue) {
			try {
				std::visit([this](const auto& event) { self->event(event); }, inputEvent);
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during event in UI: {}. Discarding event", ex.what());
			}
		}

		self->event_queue.clear();
		self->stat.event.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during event in UI: {}", ex.what());
	}

	// --- Attach ---
	try {
		AccessRoot::attach(self->root.base(), self->root.base());
		self->stat.attach2.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during attach2 in UI: {}", ex.what());
	}

//	// --- Update ---
//	try {
//		//AccessRoot::update(self->root);
//	} catch (const std::exception& ex) {
//		log_ui.error("Exception occurred during update in UI: {}", ex.what());
//	}
//
//	// --- Attach ---
//	try {
//		//AccessRoot::attach(self->root, self->root);
//	} catch (const std::exception& ex) {
//		log_ui.error("Exception occurred during attach3 in UI: {}", ex.what());
//	}

	// --- Style ---
	try {
		if (self->context.isAnyStyleDirty()) {
			AccessRoot::styleScan(self->root.base());
			self->context.clearEveryStyleDirty();
			self->stat.styleScan.sample(self->timer.time_ns());
		} else {
			AccessRoot::style(self->root.base());
			self->stat.style.sample(self->timer.time_ns());
		}
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during style in UI: {}", ex.what());
	}

	// --- Layout ---
	try {
		AccessRoot::layout1(self->root.base(), ContextLayout1{});
		self->stat.layout1.sample(self->timer.time_ns());
		AccessRoot::layout2(self->root.base(), ContextLayout2{self->root.position(), self->root.size(), MouseOrder{0}});
		self->stat.layout2.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during layout in UI: {}", ex.what());
	}

	// --- Render ---
	try {
		const auto guard_s = gl.state.push_guard();
		const auto guard_m = gl.model.push_guard();
		const auto guard_v = gl.view.push_guard();
		const auto guard_p = gl.projection.push_guard();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();

		gl.state.enableCullFace();
		gl.state.cullBackFace();
		gl.state.frontFaceCCW();

		gl.state.disableDepthTest();
		gl.state.depthFunctionLess();

		gl.state.polygonModeFill();

		gl.projection = libv::mat4f::ortho(self->root.position2(), self->root.size2());
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		gl.viewport(
				libv::vec::cast<int32_t>(self->root.position2()),
				libv::vec::cast<int32_t>(self->root.size2())
		);

		ContextRender context{gl, clock::now()};
//		AccessRoot::create(self->root, context);
		AccessRoot::render(self->root.base(), context);
//		AccessRoot::destroy(self->root, context);

		self->stat.render.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during render in UI: {}", ex.what());
	}

	// --- Detach ---
	try {
		AccessRoot::detach(self->root.base(), self->root.base());
		self->stat.detach.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during detach in UI: {}", ex.what());
	}

	self->stat.frame.sample(self->timerFrame.time_ns());

//	if (++self->i == 1200) {
//		self->i = 0;
//		std::cout << '\n' << self->stat << std::endl;
//	}
}

void UI::destroy(libv::glr::Queue& gl) {
	current_thread_context(context());

	self->root.markRemove();

	// --- Render ---
	{
		ContextRender context{gl, clock::now()};
		AccessRoot::render(self->root.base(), context);
	}

	// --- Detach ---
	{
		AccessRoot::detach(self->root.base(), self->root.base());
	}
}

// -------------------------------------------------------------------------------------------------

void UI::focus(BaseComponent& component) {
	current_thread_context(context());

	self->focus(self->context_state.focus_, component);
	self->context_state.focus_ = component;
}

void UI::detachFocused(BaseComponent& component) {
	current_thread_context(context());

	(void) component;

	assert(component == self->context_state.focus_ && "Attempted to detachFocused the not focused element");
	self->context_state.focus_ = self->focusTravers(self->context_state.focus_, Degrees<float>{315});
}

void UI::detachFocusLinked(BaseComponent& component) {
	current_thread_context(context());

	(void) component;
	assert(false && "Not yet implemented"); // TODO P5: implement focus link
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
