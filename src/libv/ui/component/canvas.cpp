// Project: libv.ui, File: src/libv/ui/component/canvas.cpp

// hpp
#include <libv/ui/component/canvas.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreCanvasAdaptor : CoreComponent {
	using base_type = CoreComponent;
	CoreCanvasAdaptor(std::string name, std::unique_ptr<CanvasBase>&& canvas_object);

public:
	std::unique_ptr<CanvasBase> canvas_object;

public:
	virtual void onMouseButton(const EventMouseButton& event) override;
//	virtual void onMouseMovement(const EventMouseMovement& event) override;

	virtual void doAttach() override;
	virtual void doUpdate() override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doCreate(Renderer& r) override;
	virtual void doRender(Renderer& r) override;
	virtual void doDestroy(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

CoreCanvasAdaptor::CoreCanvasAdaptor(std::string name, std::unique_ptr<CanvasBase>&& canvas_object) :
	CoreComponent(std::move(name)), canvas_object(std::move(canvas_object)) {
	this->canvas_object->core = libv::make_observer_ptr(this);

	// TODO P1: Optionally watch mouse/focus: opt-in? opt-out?
	watchFocus(true);
	watchMouse(true);
}

void CoreCanvasAdaptor::onMouseButton(const EventMouseButton& event) {
	event.stop_propagation();
	focus();
}

//void CoreCanvasAdaptor::onMouseMovement(const EventMouseMovement& event) {
//	event.pass_through();
//}

void CoreCanvasAdaptor::doAttach() {
	canvas_object->attach();
}

void CoreCanvasAdaptor::doUpdate() {
	if (not canvas_object)
		return;

	canvas_object->update(ui().state.time_delta());
}

void CoreCanvasAdaptor::doLayout2(const ContextLayout2& environment) {
	if (not canvas_object)
		return;

	// NOTE: canvas_position is used for view porting so it has to account for global position
	const auto global_pos = ui().mouse.get_global_position(*this);

	canvas_object->canvas_position = global_pos + padding_LB();
	canvas_object->canvas_size = xy(environment.size) - padding_size();
}

void CoreCanvasAdaptor::doCreate(Renderer& r) {
	if (not canvas_object)
		return;

	r.native([this](libv::glr::Queue& glr) {
		auto handler = CanvasAdaptor{this};
		auto events = handler.event();
		events.before_create.fire(glr.out_of_order_gl());
		canvas_object->create(glr);
		events.after_create.fire(glr.out_of_order_gl());
	});
}

void CoreCanvasAdaptor::doDestroy(Renderer& r) {
	if (not canvas_object)
		return;

	r.native([this](libv::glr::Queue& glr) {
		auto handler = CanvasAdaptor{this};
		auto events = handler.event();
		events.before_destroy.fire(glr.out_of_order_gl());
		canvas_object->destroy(glr);
		events.after_destroy.fire(glr.out_of_order_gl());
	});
}

void CoreCanvasAdaptor::doRender(Renderer& r) {
	if (not canvas_object)
		return;

	r.native([this](libv::glr::Queue& glr) {
		// TODO P1: Does not account for clipping
		//				this would require shader support in canvas
		// 				or use a framebuffer for this and render with UI texture shader
		//				(Matrix stacks reset view and discards the UI's current view, fine for now, but with clipping support it will break)

		auto handler = CanvasAdaptor{this};
		auto events = handler.event();
		events.before_render.fire(glr.out_of_order_gl());

		const auto prev_view_pos = glr.viewport_position();
		const auto prev_view_size = glr.viewport_size();

		glr.viewport(
				cast<int>(round(canvas_object->canvas_position)),
				cast<int>(round(canvas_object->canvas_size))
		);

		canvas_object->render(glr);

		glr.viewport(prev_view_pos, prev_view_size);

		events.after_render.fire(glr.out_of_order_gl());
	});
}

// =================================================================================================

libv::vec2f CanvasBase::calculate_local_mouse_coord() const noexcept {
	return core->calculate_local_mouse_coord();
}

libv::ui::ContextUI& CanvasBase::ui() const noexcept {
	return core->ui();
}

// -------------------------------------------------------------------------------------------------

void CanvasBase::focus() {
	core->focus();
}

// -------------------------------------------------------------------------------------------------

core_ptr CanvasAdaptor::create_core(std::string name, std::unique_ptr<CanvasBase>&& canvas_object) {
	return create_core_ptr<CoreType>(std::move(name), std::move(canvas_object));
}

CanvasBase& CanvasAdaptor::object_base() noexcept {
	return *self().canvas_object;
}

const CanvasBase& CanvasAdaptor::object_base() const noexcept {
	return *self().canvas_object;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
