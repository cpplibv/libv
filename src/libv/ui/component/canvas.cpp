// Project: libv.ui, File: src/libv/ui/component/canvas.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/canvas.hpp>
// pro
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreCanvasAdaptor : CoreComponent {
	friend class CanvasAdaptor;
	[[nodiscard]] inline auto handler() { return CanvasAdaptor{this}; }

private:
	Canvas* canvas = nullptr;

private:
	template <typename T> static void access_properties(T& ctx);

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doCreate(Renderer& r) override;
	virtual void doRender(Renderer& r) override;
	virtual void doDestroy(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreCanvasAdaptor::access_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CoreCanvasAdaptor::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreCanvasAdaptor> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreCanvasAdaptor::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	return {padding_size(), 0.f};
}

void CoreCanvasAdaptor::doCreate(Renderer& r) {
	if (canvas == nullptr)
		return;

	r.native([this](libv::glr::Queue& glr){
		canvas->create(glr);
	});
}

void CoreCanvasAdaptor::doRender(Renderer& r) {
	if (canvas == nullptr)
		return;

	canvas->update(context().state.time_delta());

	r.native([this](libv::glr::Queue& glr){
		canvas->render(glr);
	});
}

void CoreCanvasAdaptor::doDestroy(Renderer& r) {
	if (canvas == nullptr)
		return;

	r.native([this](libv::glr::Queue& glr){
		canvas->destroy(glr);
	});
}

// -------------------------------------------------------------------------------------------------

CanvasAdaptor::CanvasAdaptor(std::string name) :
	ComponentHandler<CoreCanvasAdaptor, EventHostGeneral<CanvasAdaptor>>(std::move(name)) { }

CanvasAdaptor::CanvasAdaptor(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreCanvasAdaptor, EventHostGeneral<CanvasAdaptor>>(gen, type) { }

CanvasAdaptor::CanvasAdaptor(core_ptr core) noexcept :
	ComponentHandler<CoreCanvasAdaptor, EventHostGeneral<CanvasAdaptor>>(core) { }

// -------------------------------------------------------------------------------------------------

void CanvasAdaptor::adopt(Canvas* canvas) {
	self().canvas = canvas;
	self().flagAuto(Flag::pendingRender);
}

void CanvasAdaptor::clear() {
	self().canvas = nullptr;
	self().flagAuto(Flag::pendingRender);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
