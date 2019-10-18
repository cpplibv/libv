// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/input/event.hpp>
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
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/context_focus_travers.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_state.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Root : public PanelFull {
public:
	Root(ContextUI& context) : PanelFull(context) {}

	void setSize(libv::vec3f size_) {
		AccessRoot::size(*this) = size_;
		flagAuto(Flag::pendingLayout);
	}

	void setPosition(libv::vec3f position_) {
		AccessRoot::position(*this) = position_;
		flagAuto(Flag::pendingLayout);
	}
};

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

class ImplUI {
public:
	using UIEvent = std::variant<
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
	Root root{context};

	struct Stat {
		Histogram<100> attach1{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> event{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> attach2{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> styleScan{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> style{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> layout1{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> layout2{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> render{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};
		Histogram<100> detach{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};

		Histogram<100> frame{std::chrono::microseconds{0}, std::chrono::milliseconds{1}};

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
	} stat;
//	size_t i = 0;

	libv::Timer timer;
	libv::Timer timerFrame;
	std::vector<UIEvent> event_queue;
	std::mutex mutex;

public:
	ImplUI(UI& ui) :
		context(ui, context_state) { }

	ImplUI(UI& ui, const Settings& settings) :
		context(ui, context_state, settings) { }

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

		if (old_focus != nullptr)
			new_focus = AccessRoot::focusTravers(*old_focus, ctx);

		if (new_focus == nullptr)
			new_focus = AccessRoot::focusTravers(root, ctx);

		focus(old_focus, new_focus);
		return new_focus;
	}
};

// -------------------------------------------------------------------------------------------------

UI::UI() {
	self = std::make_unique<ImplUI>(*this);
}

UI::UI(const Settings& settings) {
	self = std::make_unique<ImplUI>(*this, settings);
}

UI::~UI() {
}

void UI::add(std::shared_ptr<BaseComponent> component) {
	self->root.add(std::move(component));
}

void UI::setSize(libv::vec3f size_) noexcept {
	self->root.setSize(size_);
}

void UI::setSize(float x, float y, float z) noexcept {
	self->root.setSize({x, y, z});
}

void UI::setPosition(libv::vec3f position_) noexcept {
	self->root.setPosition(position_);
}

void UI::setPosition(float x, float y, float z) noexcept {
	self->root.setPosition({x, y, z});
}

BaseComponent& UI::root() const noexcept {
	return self->root;
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
	self->timer.reset();
	self->timerFrame.reset();

	self->context_state.time_frame_ = clock::now();
	self->context_state.time_ = self->context_state.time_frame_ - self->context_state.time_ui_;

	try {
		// --- Attach ---
		AccessRoot::attach(self->root, self->root);
		self->stat.attach1.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during attach1 in UI: {}", ex.what());
	}

	try {
		// --- Event ---
		self->context.mouse.event_update(); // Internal "mouse" events don't require event queue lock (these are reactive events to layout and attach changes)

		std::unique_lock lock{self->mutex};
		for (const auto& mouseEvent : self->event_queue) {
			const auto visitor = libv::overload(
				[this](const libv::input::EventChar& event) {
					if (self->context_state.focus_ != nullptr)
						AccessRoot::eventChar(*self->context_state.focus_, event);
				},
				[this](const libv::input::EventKey& event) {
					if (self->context_state.focus_ != nullptr)
						AccessRoot::eventKey(*self->context_state.focus_, event);

					if (event.action != libv::input::Action::release) {
						self->context_state.pressed_keys.insert(event.key);
						self->context_state.pressed_scancodes.insert(event.scancode);
					} else {
						self->context_state.pressed_keys.erase(event.key);
						self->context_state.pressed_scancodes.erase(event.scancode);
					}
				},
				[this](const libv::input::EventMouseEnter& event) {
					if (event.entered) {
						self->context.mouse.event_enter();
						self->context_state.mouse_over_ = true;
					} else {
						self->context.mouse.event_leave();
						self->context_state.mouse_over_ = false;
					}
				},
				[this](const libv::input::EventMouseButton& event) {
					self->context.mouse.event_button(event.button, event.action);
					if (event.action != libv::input::Action::release)
						self->context_state.pressed_mouses.insert(event.button);
					else
						self->context_state.pressed_mouses.erase(event.button);
				},
				[this](const libv::input::EventMousePosition& event) {
					const auto position = libv::vec::cast<float>(event.position);
					self->context.mouse.event_position(position);
					self->context_state.mouse_position_ = position;
				},
				[this](const libv::input::EventMouseScroll& event) {
					const auto offset = libv::vec::cast<float>(event.offset);
					self->context.mouse.event_scroll(offset);
					self->context_state.scroll_position_ = offset;
				}
			);

			try {
				std::visit(visitor, mouseEvent);
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during event in UI: {}. Discarding event", ex.what());
			}
		}

		self->event_queue.clear();
		self->stat.event.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during event in UI: {}", ex.what());
	}

	try {
		// --- Attach ---
		AccessRoot::attach(self->root, self->root);
		self->stat.attach2.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during attach2 in UI: {}", ex.what());
	}

	try {
		// --- Update ---
		//AccessRoot::update(self->root);
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during update in UI: {}", ex.what());
	}

	try {
		// --- Attach ---
		//AccessRoot::attach(self->root, self->root);
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during attach3 in UI: {}", ex.what());
	}

	try {
		// --- Style ---
		if (self->context.isAnyStyleDirty()) {
			AccessRoot::styleScan(self->root);
			self->context.clearEveryStyleDirty();
			self->stat.styleScan.sample(self->timer.time_ns());
		} else {
			AccessRoot::style(self->root);
			self->stat.style.sample(self->timer.time_ns());
		}
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during style in UI: {}", ex.what());
	}

	try {
		// --- Layout ---
		AccessRoot::layout1(self->root, ContextLayout1{});
		self->stat.layout1.sample(self->timer.time_ns());
		AccessRoot::layout2(self->root, ContextLayout2{AccessRoot::position(self->root), AccessRoot::size(self->root), MouseOrder{0}});
		self->stat.layout2.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during layout in UI: {}", ex.what());
	}

	try {
		// --- Render ---
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

		gl.projection = libv::mat4f::ortho(xy(AccessRoot::position(self->root)), xy(AccessRoot::size(self->root)));
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		gl.viewport(
				libv::vec::cast<int32_t>(libv::vec::xy(AccessRoot::position(self->root))),
				libv::vec::cast<int32_t>(libv::vec::xy(AccessRoot::size(self->root)))
		);

		ContextRender context{gl, clock::now()};
//		AccessRoot::create(self->root, context);
		AccessRoot::render(self->root, context);
//		AccessRoot::destroy(self->root, context);

		self->stat.render.sample(self->timer.time_ns());
	} catch (const std::exception& ex) {
		log_ui.error("Exception occurred during render in UI: {}", ex.what());
	}

	try {
		// --- Detach ---
		AccessRoot::detach(self->root, self->root);
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
	self->root.markRemove();

	{
		// --- Render ---
		ContextRender context{gl, clock::now()};
		AccessRoot::render(self->root, context);
	} {
		// --- Detach ---
		AccessRoot::detach(self->root, self->root);
	}
}

// -------------------------------------------------------------------------------------------------

void UI::focus(BaseComponent& component) {
	self->focus(self->context_state.focus_, component);
	self->context_state.focus_ = component;
}

void UI::detachFocused(BaseComponent& component) {
	(void) component;

	assert(component == self->context_state.focus_ && "Attempted to detachFocused the not focused element");
	self->context_state.focus_ = self->focusTravers(self->context_state.focus_, Degrees<float>{315});
}

void UI::detachFocusLinked(BaseComponent& component) {
	(void) component;
	// TODO P5: implement focus link
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
