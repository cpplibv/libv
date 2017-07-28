// File:   ui.cpp Author: Vader Created on 2019. február 14., 4:55

// hpp
#include <libv/ui/ui.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/component_static.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Root : public ComponentStatic<Root> {
	friend class ComponentStaticAccess;

public:
	std::vector<std::shared_ptr<ComponentBase>> components;

	Root() :
		ComponentStatic<Root>("root") {}

private:
	template <typename T>
	void ui_access(T&& access) {
		for (const auto& component : components) {
			access(*component);
		}
	}
};

// -------------------------------------------------------------------------------------------------

UI::UI() {
	context_ui = std::make_unique<ContextUI>();
	root = std::make_unique<Root>();
}

UI::~UI() {
}

void UI::add(std::shared_ptr<ComponentBase> component) {
	auto& comp = root->components.emplace_back(std::move(component));
	comp->parent = libv::make_observer_ref(*root);
	root->invalidate(comp->flags);
}

void UI::setSize(libv::vec3f size_) noexcept {
	root->size = size_;
}

void UI::setSize(float x, float y, float z) noexcept {
	setSize({x, y, z});
}

void UI::setPosition(libv::vec3f position_) noexcept {
	root->position = position_;
}

void UI::setPosition(float x, float y, float z) noexcept {
	setPosition({x, y, z});
}

// -------------------------------------------------------------------------------------------------

void UI::attach() {
	root->attach(*context_ui);
}

void UI::layout() {
	root->layoutPass1(ContextLayoutPass1{});
	root->layoutPass2(ContextLayoutPass2{root->position, root->size});
}

void UI::render(libv::glr::Queue& gl) {
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

	// TODO P3: libv.ui: disableDepthTest_guard
	gl.state.disableDepthTest();
	gl.state.depthFunctionLess();

	gl.state.polygonModeFill();

	gl.projection = libv::mat4f::ortho(
			root->position.x, root->position.x + root->size.x,
			root->position.y, root->position.y + root->size.y);
	gl.view = libv::mat4f::identity();
	gl.model = libv::mat4f::identity();

	ContextRender context{gl};
	root->render(context);
}

// -------------------------------------------------------------------------------------------------

void UI::create(libv::glr::Queue& gl) {
	ContextRender context{gl};
	root->create(context);
}

void UI::destroy(libv::glr::Queue& gl) {
	ContextRender context{gl};
	root->destroy(context);
}

void UI::update(libv::glr::Queue& gl) {
	timer.reset();

	if (root->flags.match_any(Flag::invalidAttach))
		attach();

	if (root->flags.match_any(Flag::invalidLayout))
		layout();

//	if (root->flags.match_any(Flag::invalidRender))
//		render();
	render(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
