// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/utility/overload.hpp>
#include <libv/utility/timer.hpp>
// std
#include <mutex>
#include <variant>
#include <vector>
// pro
#include <libv/ui/component/panel_full.hpp>
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
	struct UIMouseEnter {
		bool entered;
	};

	struct UIMouseButton {
		libv::input::Mouse button;
		libv::input::Action action;
	};

	struct UIMousePosition {
		libv::vec2d position;
	};

	struct UIMouseScroll {
		libv::vec2d offset;
	};

	using UIMouseEvent = std::variant<UIMouseEnter, UIMouseButton, UIMousePosition, UIMouseScroll>;

public:
	ContextUI context;
	Root root;
	libv::Timer timer;
	std::vector<UIMouseEvent> mouseEvents;
	std::mutex mutex;
};

// -------------------------------------------------------------------------------------------------

UI::UI() {
	self = std::make_unique<ImplUI>();
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

void UI::eventMouseEnter(bool entered) {
	std::unique_lock lock{self->mutex};
	self->mouseEvents.emplace_back(ImplUI::UIMouseEnter{entered});
}

void UI::eventMouseButton(libv::input::Mouse button, libv::input::Action action) {
	std::unique_lock lock{self->mutex};
	self->mouseEvents.emplace_back(ImplUI::UIMouseButton{button, action});
}

void UI::eventMousePosition(libv::vec2d position) {
	std::unique_lock lock{self->mutex};
	self->mouseEvents.emplace_back(ImplUI::UIMousePosition{position});
}

void UI::eventMouseScroll(libv::vec2d offset) {
	std::unique_lock lock{self->mutex};
	self->mouseEvents.emplace_back(ImplUI::UIMouseScroll{offset});
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
		for (const auto& mouseEvent : self->mouseEvents) {
			const auto visitor = libv::overload(
				[this](const ImplUI::UIMouseEnter& event) {
					if (event.entered)
						self->context.mouse.event_enter();
					else
						self->context.mouse.event_leave();
				},
				[this](const ImplUI::UIMouseButton& event) {
					self->context.mouse.event_button(event.button, event.action);
				},
				[this](const ImplUI::UIMousePosition& event) {
					self->context.mouse.event_position(libv::vec::cast<float>(event.position));
				},
				[this](const ImplUI::UIMouseScroll& event) {
					self->context.mouse.event_scroll(libv::vec::cast<float>(event.offset));
				}
			);

			std::visit(visitor, mouseEvent);
		}

		self->mouseEvents.clear();
	} {
		// --- Attach ---
		AccessRoot::attach(self->root, self->root);
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

} // namespace ui
} // namespace libv
