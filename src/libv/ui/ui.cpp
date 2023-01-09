// Project: libv.ui, File: src/libv/ui/ui.cpp

// hpp
#include <libv/ui/ui.hpp>
// libv
#include <libv/fsw/watcher.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/input/event.hpp>
#include <libv/lua/lua.hpp>
#include <libv/math/remap.hpp>
#include <libv/utility/histogram.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
#include <libv/utility/memory/observer_ref.hpp>
#include <libv/utility/overload.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <mutex>
#include <optional>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/overlay/overlay_zoom.hxx>
// #include <libv/ui/component/panel_full.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event_system/event_hub.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/lua/script_style.hpp>
#include <libv/ui/settings.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using Root = PanelAnchor;

// -------------------------------------------------------------------------------------------------

class ImplUI {
	struct Stat {
		static constexpr std::chrono::milliseconds t_min{0};
		static constexpr std::chrono::milliseconds t_max{20};
		static constexpr std::size_t resolution = 100;

		libv::Histogram<resolution> attach1{t_min, t_max};
		libv::Histogram<resolution> event{t_min, t_max};
		libv::Histogram<resolution> loopTask{t_min, t_max};
		libv::Histogram<resolution> attach2{t_min, t_max};
		libv::Histogram<resolution> styleScan{t_min, t_max};
		libv::Histogram<resolution> style{t_min, t_max};
		libv::Histogram<resolution> layout{t_min, t_max};
		libv::Histogram<resolution> mouse{t_min, t_max};
		libv::Histogram<resolution> render{t_min, t_max};
		libv::Histogram<resolution> renderExe{t_min, t_max};
		libv::Histogram<resolution> detach{t_min, t_max};

		libv::Histogram<resolution> frame{t_min, t_max};

		friend std::ostream& operator<<(std::ostream& os, const Stat& var) {
			for (std::size_t i = 0; i <= resolution; ++i)
				os << time_duration{t_max - t_min}.count() * static_cast<double>(i) / resolution << " ";

			os << "\nattach1:   " << var.attach1;
			os << "\nevent:     " << var.event;
			os << "\nloopTask:  " << var.loopTask;
			os << "\nattach2:   " << var.attach2;
			os << "\nstyleScan: " << var.styleScan;
			os << "\nstyle:     " << var.style;
			os << "\nlayout:    " << var.layout;
			os << "\nmouse:     " << var.mouse;
			os << "\nrender:    " << var.render;
			os << "\nrenderExe: " << var.renderExe;
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
	libv::glr::Remote remote;

public:
	UI& ui;
	Settings settings;

	ContextEvent context_event;
	ContextMouse context_mouse;
	ContextState context_state;
	ContextResource context_resource{settings};
	ContextRender context_render;
	ContextStyle context_style;

	ContextUI context_ui{ui, settings, context_event, context_mouse, context_state, context_resource, context_style};

	bool context_render_created = false;

public:
	std::vector<EventVariant> event_queue;
	std::mutex event_queue_m;

public:
	std::vector<std::function<void()>> loop_tasks;
	std::mutex loop_tasks_m;

public:
	libv::fsw::Watcher fsw;

public:
	Root root;

public:
	Stat stat;
	libv::Timer timer;
	libv::Timer timerFrame;

public:
	OverlayZoomMode overlayZoomMode = OverlayZoomMode::disabled;
	OverlayZoom overlayZoom;

//	bool overlayCursorEnable = false;
//	std::shared_ptr<OverlayCursor> overlayCursor = std::make_shared<OverlayCursor>(root);

//	std::shared_ptr<OverlayPref> overlayPref = std::make_shared<OverlayPref>(root);
//	std::shared_ptr<OverlayStack> overlayStack = std::make_shared<OverlayStack>(root);

public:
	ImplUI(UI& ui, Settings settings) :
		ui(ui),
		settings(std::move(settings)),
		// NOTE: operator, utilized to set current_thread_context to prepare for root constructor
		root((current_thread_context(context_ui), "")) { }

	~ImplUI() {
		clear_current_thread_context();
	}

public:
	void focus(libv::observer_ptr<CoreComponent> old_focus, libv::observer_ptr<CoreComponent> new_focus) {
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

	libv::observer_ptr<CoreComponent> focusTraverse(libv::observer_ptr<CoreComponent> old_focus, degrees<float> direction) {
		ContextFocusTraverse ctx{direction};

		libv::observer_ptr<CoreComponent> new_focus = nullptr;

		if (old_focus != nullptr) // Traversee to next
			new_focus = AccessRoot::focusTraverse(*old_focus, ctx);

		if (new_focus == nullptr) // End reached, Loop around
			new_focus = AccessRoot::focusTraverse(root.core(), ctx);

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
		context_mouse.event_button(event.button, event.action);
		if (event.action != libv::input::Action::release)
			context_state.pressed_mouses.insert(event.button);
		else
			context_state.pressed_mouses.erase(event.button);
	}

	void event(const libv::input::EventMouseEnter& event) {
		if (event.entered) {
			context_mouse.event_enter();
			context_state.mouse_over_ = true;
		} else {
			context_mouse.event_leave();
			context_state.mouse_over_ = false;
		}
	}

	void event(const libv::input::EventMousePosition& event) {
		auto position = libv::vec::cast<float>(event.position);

		if (overlayZoomMode == OverlayZoomMode::view)
			// Floor is used as mouse position are in pixel center coordinates
			position = libv::vec::floor(libv::remap(position, libv::vec2f(), root.layout_size2(), overlayZoom.screen_BL(), overlayZoom.screen_TR() + 1.0f));

		context_mouse.event_position(position);
		context_state.mouse_position_ = position;
	}

	void event(const libv::input::EventMouseScroll& event) {
		const auto offset = libv::vec::cast<float>(event.offset);
		context_mouse.event_scroll(offset);
		context_state.scroll_position_ = offset;
	}

	void create() {
		remote.create();
		remote.enableDebug();
	}

	void update() {
		// UI Loop:
		// - Frame state roll
		// - Attach #1
		// - Event
		// - Update
		// - UI loop tasks
		// - Attach #2
		// - Style
		// - Layout
		// - Mouse update layout // Comes after layout: Mouse needs the updated mouse region bounds to produce the events
		// - Render
		// - Detach

		current_thread_context(context_ui);

		auto glr = remote.queue();

		auto events = EventHostGlobal<Component>{root};

		{
			// --- Frame state roll ---
			timer.reset();
			timerFrame.reset();

			context_state.frame_count_++;
			const auto now = clock::now();
			context_state.time_delta_ = now - context_state.time_frame_;
			context_state.time_frame_ = now;
			context_state.time_ = context_state.time_frame_ - context_state.time_ui_;

			// --- Attach #1 ---
			try {
				AccessRoot::attach(root.core(), root.core());
				stat.attach1.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during attach #1 in UI: {}", ex.what());
			}

			// --- Event ---
			{
				// Usual pattern to lift the entry out of locking scope, candidate for generalization
				std::size_t i = 0;
				std::optional<EventVariant> current_event;
				while (true) {
					{
						std::unique_lock lock{event_queue_m};
						if (i != event_queue.size()) {
							current_event = std::move(event_queue[i]);
							i++;
						} else {
							event_queue.clear();
							break;
						}
					}

					std::visit([this](const auto& event) {
						try {
							this->event(event);
						} catch (const std::exception& ex) {
							log_ui.error("Exception occurred during event in UI: {}. Discarding event {}", ex.what(), event.toPrettyString());
						}
					}, *current_event);
				}

				stat.event.sample(timer.time_ns());
			}

			// --- Update ---
			try {
				events.global_before_update.fire(context_state.time_frame(), context_state.time_delta());
				AccessRoot::update(root.core());
				events.global_after_update.fire(context_state.time_frame(), context_state.time_delta());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during update in UI: {}", ex.what());
			}

			// --- UI loop tasks ---
			{
				// TODO P5: libv.mt: Usual pattern to lift the entry out of locking scope, candidate for generalization
				//				libv.mt: queue
				std::size_t i = 0;
				std::function<void()> current_task;
				while (true) {
					{
						std::unique_lock lock{loop_tasks_m};
						if (i != loop_tasks.size()) {
							current_task = std::move(loop_tasks[i]);
							i++;
						} else {
							loop_tasks.clear();
							break;
						}
					}

					try {
						current_task();
					} catch (const std::exception& ex) {
						log_ui.error("Exception occurred during UI loop task execution in UI: {}", ex.what());
					}
				}

				stat.loopTask.sample(timer.time_ns());
			}

			// --- Attach #2 ---
			try {
				AccessRoot::attach(root.core(), root.core());
				stat.attach2.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during attach #2 in UI: {}", ex.what());
			}

			// --- Style ---
			try {
				if (context_style.isAnyStyleDirty()) {
					AccessRoot::styleScanAll(root.core());
					context_style.clearEveryStyleDirty();
					stat.styleScan.sample(timer.time_ns());
				} else {
					AccessRoot::styleScan(root.core());
					stat.style.sample(timer.time_ns());
				}
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during style in UI: {}", ex.what());
			}

			// --- Layout ---
			try {
				AccessRoot::layout2(root.core(), ContextLayout2{root.layout_position(), root.layout_size()});
				stat.layout.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during layout in UI: {}", ex.what());
			}

			// --- Mouse Update Layout ---
			try {
				context_mouse.event_update_layout();
				stat.mouse.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during virtual events in UI: {}. Discarding rest of the virtual events", ex.what());
			}

			// --- Render ---
			try {
				events.global_before_render.fire(glr.out_of_order_gl());

				glr.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
				glr.clearColor();
				glr.clearDepth();

				const auto guard_s = glr.state.push_guard();
				const auto guard_m = glr.model.push_guard();
				const auto guard_v = glr.view.push_guard();
				const auto guard_p = glr.projection.push_guard();

				//AccessRoot::create(root.core(), context);
				auto r = context_render.root_renderer(
						root,
						glr,
						context_state.time_frame(),
						context_state.time(),
						context_state.mouse_position(),
						root.layout_size2()
				);

				AccessRoot::render(root.core(), r);
				context_render.execute_render(glr);
				//AccessRoot::destroy(root.core(), context);

				if (overlayZoomMode != OverlayZoomMode::disabled)
					overlayZoom.postRender(glr);

				stat.render.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during render in UI: {}", ex.what());
			}

			// --- Execute Render Queue ---
			remote.queue(std::move(glr));
			remote.execute();
			stat.renderExe.sample(timer.time_ns());

			// --- Detach ---
			try {
				AccessRoot::detachScan(root.core());
				stat.detach.sample(timer.time_ns());
			} catch (const std::exception& ex) {
				log_ui.error("Exception occurred during detach in UI: {}", ex.what());
			}

			stat.frame.sample(timerFrame.time_ns());
		}

//		static int i = 0;
//		if (++i == 1200) {
//			i = 0;
//			log_ui.info("UI Statistics:\n{}", stat);
//		}
	}

	void destroy() {
		current_thread_context(context_ui);

		{
			auto glr = remote.queue();

			root.markRemove();

			// --- Render ---
			{
				auto r = context_render.root_renderer(
						root,
						glr,
						context_state.time_frame(),
						context_state.time(),
						context_state.mouse_position(),
						root.layout_size2()
				);
				AccessRoot::render(root.core(), r);
				context_render.execute_render(glr);
			}

			// --- Execute Destroy Queue ---
			remote.queue(std::move(glr));
			remote.execute();

			// --- Detach ---
			{
				AccessRoot::detachScan(root.core());
			}
		}

		remote.destroy();
	}
};

// -------------------------------------------------------------------------------------------------

UI::UI() : UI(Settings{}) {
}

UI::UI(Settings settings) {
	self = std::make_shared<ImplUI>(*this, std::move(settings));
	nexus().object_view_set<libv::ui::UI>(this);
}

UI::~UI() {
	nexus().object_view_remove<libv::ui::UI>();
}

void UI::add(Component component) {
	self->root.add(std::move(component));
}

void UI::remove(Component component) {
	self->root.remove(component);
}

void UI::setSize(libv::vec2i size_) noexcept {
	AccessRoot::layout_size(self->root.core()) = libv::vec3f{libv::vec::cast<float>(size_), 0};
	AccessRoot::flagAuto(self->root.core(), Flag::pendingLayout);
}

// -------------------------------------------------------------------------------------------------

void UI::execute_in_ui_loop(std::function<void()> func) {
	std::unique_lock lock{self->loop_tasks_m};
	self->loop_tasks.emplace_back(std::move(func));
}

void UI::foreach_recursive_component(libv::function_ref<void(const Component&)> func) {
	func(self->root);
	self->root.foreach_recursive_children(func);
}

// -------------------------------------------------------------------------------------------------

//void UI::load_style_script(std::string_view script) {
void UI::load_style_script_file(std::string path) {

	execute_in_ui_loop([this, path] {
		auto script = libv::read_file_str_ec(path);
		if (script.ec)
			return log_ui.error("Failed to reload style script {}: {}: {}", path, fmt::streamed(script.ec), script.ec.message());

		// TODO P4: Persist/reuse lua state, make sure sandboxing happens
		auto lua = libv::lua::create_state(libv::lua::lualib::base);
		script_style(*this, lua, script.data);
	});

	if (context().settings.track_style_scripts) {
		// TODO P1: fsw token and/or kill fsw before dtor reaches it, so handle late events during termination
		self->fsw.subscribe_file(path, [this, path](const auto&) {
			// TODO P4: Warmup / Cooldown for tracking reload
			execute_in_ui_loop([this, path] {
				auto script = libv::read_file_str_ec(path);
				if (script.ec)
					return log_ui.error("Failed to reload style script {}: {}: {}", path, fmt::streamed(script.ec), script.ec.message());

				// TODO P4: Persist/reuse lua state, make sure sandboxing happens
				auto lua = libv::lua::create_state(libv::lua::lualib::base);
				script_style(*this, lua, script.data);
			});
		});
	}
}

// -------------------------------------------------------------------------------------------------

void UI::event(const libv::input::EventChar& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventKey& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseButton& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseEnter& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMousePosition& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

void UI::event(const libv::input::EventMouseScroll& event) {
	std::unique_lock lock{self->event_queue_m};
	self->event_queue.emplace_back(event);
}

// -------------------------------------------------------------------------------------------------

EventHostGlobal<Component> UI::event() {
	return EventHostGlobal<Component>{self->root};
}

EventHub UI::event_hub() {
	auto sp = std::shared_ptr<ContextEvent>(self, &self->context_event);
	return EventHub(&self->context_ui, std::weak_ptr<ContextEvent>(sp));
}

// -------------------------------------------------------------------------------------------------

libv::gl::GL& UI::gl() {
	return self->remote.gl();
}

[[nodiscard]] libv::Nexus& UI::nexus() {
	return self->context_event.nexus;
}

ContextUI& UI::context() {
	return self->context_ui;
}

ContextState& UI::state() {
	return self->context_state;
}

ContextResource& UI::resource() {
	return self->context_resource;
}

ContextStyle& UI::style() {
	return self->context_style;
}

// -------------------------------------------------------------------------------------------------

void UI::create() {
	self->create();
}

void UI::update() {
	self->update();
}

void UI::destroy() {
	self->destroy();
}

// -------------------------------------------------------------------------------------------------

void UI::focus(CoreComponent& component) {
	current_thread_context(context());

	self->focus(self->context_state.focus_, libv::make_observer_ptr(&component));
	self->context_state.focus_ = libv::make_observer_ptr(&component);
}

void UI::detachFocused(CoreComponent& component) {
	current_thread_context(context());

	(void) component;
	assert(libv::make_observer_ptr(&component) == self->context_state.focus_ && "Attempted to detachFocused the not focused element");
	self->context_state.focus_ = self->focusTraverse(self->context_state.focus_, degrees<float>{315});
}

void UI::detachFocusLinked(CoreComponent& component) {
	current_thread_context(context());

	(void) component;
	assert(false && "Not yet implemented"); // TODO P5: implement focus link
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
