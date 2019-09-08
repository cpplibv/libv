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
#include <mutex>
#include <variant>
#include <vector>
// pro
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/context_focus_travers.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Root : public PanelFull {
public:
	Root() : PanelFull("") {}

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
	ContextUI context;
	Root root;
	libv::Timer timer;
	std::vector<UIEvent> event_queue;
	std::mutex mutex;

	libv::observer_ref<BaseComponent> focused = libv::make_observer_ref(root);

public:
	ImplUI(UI& ui) :
		context(ui) { }

public:
	void focus(BaseComponent& component) {
		if (libv::make_observer_ref(component) == focused)
			return;

		AccessRoot::focusChange(*focused, component);
		focused = libv::make_observer_ref(component);

		log_ui.debug("Focused: {}", focused->path());
	}

	void focusTravers(Degrees<float> direction) {
		ContextFocusTravers context{libv::make_observer_ref(*focused), direction};

		auto newFocus = AccessRoot::focusTravers(*focused, context, *focused);

		if (newFocus == nullptr) // Loop around
			newFocus = AccessRoot::focusTravers(root, context, root);

		if (newFocus == nullptr) // Not found anything, back to root
			newFocus = libv::make_observer(root);

		AccessRoot::focusChange(*focused, *newFocus);
		focused = libv::make_observer_ref(*newFocus);

		log_ui.debug("Focused: {}", focused->path());
	}
};

// -------------------------------------------------------------------------------------------------

UI::UI() {
	self = std::make_unique<ImplUI>(*this);
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

// -------------------------------------------------------------------------------------------------

void UI::update(libv::glr::Queue& gl) {
	self->timer.reset();
	AccessRoot::setContext(self->root, self->context);

	{
		// --- Attach ---
		AccessRoot::attach(self->root, self->root);
	} {
		// --- Event ---
		self->context.mouse.event_update(); // Internal "mouse" events don't require event queue lock

		std::unique_lock lock{self->mutex};
		for (const auto& mouseEvent : self->event_queue) {
			const auto visitor = libv::overload(
				[this](const libv::input::EventChar& event) {
					AccessRoot::eventChar(*self->focused, event);
				},
				[this](const libv::input::EventKey& event) {
					AccessRoot::eventKey(*self->focused, event);
				},
				[this](const libv::input::EventMouseEnter& event) {
					if (event.entered)
						self->context.mouse.event_enter();
					else
						self->context.mouse.event_leave();
				},
				[this](const libv::input::EventMouseButton& event) {
					self->context.mouse.event_button(event.button, event.action);
				},
				[this](const libv::input::EventMousePosition& event) {
					self->context.mouse.event_position(libv::vec::cast<float>(event.position));
				},
				[this](const libv::input::EventMouseScroll& event) {
					self->context.mouse.event_scroll(libv::vec::cast<float>(event.offset));
				}
			);

			std::visit(visitor, mouseEvent);
		}

		self->event_queue.clear();
	} {
		// --- Attach ---
		AccessRoot::attach(self->root, self->root);
	} {
		// --- Focus ---
//		self->focusTravers(Degrees<float>{135}); // backward
//		self->focusTravers(Degrees<float>{315}); // forward
	} {
//		// --- Update ---
//		AccessRoot::update(self->root);
//	} {
//		// --- Attach ---
//		AccessRoot::attach(self->root, self->root);
	} {
		// --- Style ---
		if (self->context.isAnyStyleDirty()) {
			AccessRoot::styleScan(self->root);
			self->context.clearEveryStyleDirty();
		} else {
			AccessRoot::style(self->root);
		}
	} {
		// --- Layout ---
		AccessRoot::layout1(self->root, ContextLayout1{});
		AccessRoot::layout2(self->root, ContextLayout2{AccessRoot::position(self->root), AccessRoot::size(self->root), MouseOrder{0}});
	} {
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

		gl.viewport(xy(AccessRoot::position(self->root)), xy(AccessRoot::size(self->root)));

		ContextRender context{gl};
//		AccessRoot::create(self->root, context);
		AccessRoot::render(self->root, context);
//		AccessRoot::destroy(self->root, context);
	} {
		// --- Detach ---
		AccessRoot::detach(self->root, self->root);
	}
}

void UI::destroy(libv::glr::Queue& gl) {
	self->root.markRemove();

	{
		// --- Render ---
		ContextRender context{gl};
		AccessRoot::render(self->root, context);
	} {
		// --- Detach ---
		AccessRoot::detach(self->root, self->root);
	}
}

// -------------------------------------------------------------------------------------------------

void UI::focus(BaseComponent& component) {
	self->focus(component);
}

void UI::detachFocused(BaseComponent& component) {
	(void) component;

	assert(&component == self->focused);
	self->focusTravers(Degrees<float>{315});
}

void UI::detachFocusLinked(BaseComponent& component) {
	(void) component;
	// TODO P5: implement focus link
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
