// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/check_box.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/core_component.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
//#include <libv/ui/font_2D.hpp>
#include <libv/ui/property_access_context.hpp>
//#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
//#include <libv/ui/string_2D.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class CoreRadioButton : public CoreComponent {
//	friend class RadioButton;
//	[[nodiscard]] inline auto handler() { return RadioButton{this}; }
//
//private:
//	template <typename T> static void access_properties(T& ctx);
//
//	struct Properties {
//		PropertyR<Color> bg_color;
//		PropertyL<Texture2D_view> bg_image;
//		PropertyR<ShaderImage_view> bg_shader;
//
//		PropertyR<Color> font_color;
//		PropertyR<ShaderFont_view> font_shader;
//
//		PropertyL<> align_horizontal;
//		PropertyL<> align_vertical;
//		PropertyL<> font;
//		PropertyL<> font_size;
//	} property;
//
//private:
//	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
//	String2D text_;
//
//public:
//	using CoreComponent::CoreComponent;
//
//private:
//	virtual void onFocus(const EventFocus& event) override;
//	virtual void onMouseRadioButton(const EventMouseRadioButton& event) override;
//	virtual void onMouseMovement(const EventMouseMovement& event) override;
//	virtual void onMouseScroll(const EventMouseScroll& event) override;
//
//private:
//	virtual void doAttach() override;
//	virtual void doStyle(ContextStyle& ctx) override;
//	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
//	virtual void doLayout2(const ContextLayout2& environment) override;
//	virtual void doRender(ContextRender& context) override;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//void CoreRadioButton::access_properties(T& ctx) {
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.bg_color; },
//			Color(1, 1, 1, 1),
//			pgr::appearance, pnm::bg_color,
//			"Background color"
//	);
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.bg_image; },
//			[](auto& u) { return u.fallbackTexture2D(); },
//			pgr::appearance, pnm::bg_image,
//			"Background image"
//	);
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.bg_shader; },
//			[](auto& u) { return u.shaderImage(); },
//			pgr::appearance, pnm::bg_shader,
//			"Background shader"
//	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.align_horizontal; },
//			[](auto& c, auto v) { c.text_.align_horizontal(v); },
//			[](const auto& c) { return c.text_.align_horizontal(); },
//			AlignHorizontal::Left,
//			pgr::appearance, pnm::align_horizontal,
//			"Horizontal alignment of the text"
//	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.align_vertical; },
//			[](auto& c, auto v) { c.text_.align_vertical(v); },
//			[](const auto& c) { return c.text_.align_vertical(); },
//			AlignVertical::Top,
//			pgr::appearance, pnm::align_vertical,
//			"Vertical alignment of the text"
//	);
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.font_color; },
//			Color(0, 0, 0, 1),
//			pgr::appearance, pnm::font_color,
//			"Font color"
//	);
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.font_shader; },
//			[](auto& u) { return u.shaderFont(); },
//			pgr::appearance, pnm::font_shader,
//			"Font shader"
//	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.font; },
//			[](auto& c, auto v) { c.text_.font(std::move(v)); },
//			[](const auto& c) { return c.text_.font(); },
//			[](auto& u) { return u.fallbackFont(); },
//			pgr::font, pnm::font,
//			"Font file"
//	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.font_size; },
//			[](auto& c, auto v) { c.text_.size(v); },
//			[](const auto& c) { return c.text_.size(); },
//			FontSize{12},
//			pgr::font, pnm::font_size,
//			"Font size in pixel"
//	);
//	ctx.synthetize(
//			[](auto& c, auto v) { c.handler().text(std::move(v)); },
//			[](const auto& c) { return c.handler().text(); },
//			pgr::behaviour, pnm::text,
//			"Displayed text"
//	);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void CoreRadioButton::onFocus(const EventFocus& event) {
//	(void) event;
//	// TODO P3: listen to hotkey event (ui.select)
//
//	if (event.loss())
//		{} // Set style to normal or disabled
//
//	if (event.gain())
//		{} // Set style to active if not disabled
//
//	flagAuto(Flag::pendingRender);
//}
//
//void CoreRadioButton::onMouseRadioButton(const EventMouseRadioButton& event) {
//	if (event.action == libv::input::Action::press)
//		focus();
//
//	fire(EventMouseRadioButton{event});
//
//	// TODO P3: use hotkey event (ui.select) (even for mouse)
//	if (event.action == libv::input::Action::press && event.button == libv::input::MouseRadioButton::Left)
//		fire(EventSubmit{});
//
//	event.stop_propagation();
//}
//
//void CoreRadioButton::onMouseMovement(const EventMouseMovement& event) {
//	if (event.enter)
//		set(property.bg_color, property.bg_color() + 0.2f);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
//
//	if (event.leave)
//		reset(property.bg_color);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
//
//	fire(EventMouseMovement{event});
//
//	event.stop_propagation();
//}
//
//void CoreRadioButton::onMouseScroll(const EventMouseScroll& event) {
//	fire(EventMouseScroll{event});
//
//	event.stop_propagation();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void CoreRadioButton::doAttach() {
//	watchFocus(true);
//	watchMouse(true);
//}
//
//void CoreRadioButton::doStyle(ContextStyle& ctx) {
//	PropertyAccessContext<CoreRadioButton> setter{*this, ctx.component, ctx.style, context()};
//	access_properties(setter);
//	CoreComponent::access_properties(setter);
//}
//
//libv::vec3f CoreRadioButton::doLayout1(const ContextLayout1& environment) {
//	const auto dynamic_size_text = text_.content(xy(environment.size.x));
//	const auto dynamic_size_image = property.bg_image()->size().cast<float>();
//
//	return {libv::vec::max(dynamic_size_text, dynamic_size_image), 0.f};
//}
//
//void CoreRadioButton::doLayout2(const ContextLayout2& environment) {
//	text_.limit(libv::vec::xy(environment.size));
//}
//
//void CoreRadioButton::doRender(ContextRender& ctx) {
//	if (ctx.changedSize) {
//		bg_mesh.clear();
//		auto pos = bg_mesh.attribute(attribute_position);
//		auto tex = bg_mesh.attribute(attribute_texture0);
//		auto index = bg_mesh.index();
//
//		pos(0, 0, 0);
//		pos(layout_size().x, 0, 0);
//		pos(layout_size().x, layout_size().y, 0);
//		pos(0, layout_size().y, 0);
//
//		tex(0, 0);
//		tex(1, 0);
//		tex(1, 1);
//		tex(0, 1);
//
//		index.quad(0, 1, 2, 3);
//	}
//
//	const auto guard_m = ctx.gl.model.push_guard();
// 	ctx.gl.model.translate(layout_position());
//
//	{
//		ctx.gl.program(*property.bg_shader());
//		ctx.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
//		ctx.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
//		ctx.gl.uniform(property.bg_shader()->uniform_MVPmat, ctx.gl.mvp());
//		ctx.gl.render(bg_mesh);
//	} {
//		const auto guard_s = ctx.gl.state.push_guard();
//		ctx.gl.state.blendSrc_Source1Color();
//		ctx.gl.state.blendDst_One_Minus_Source1Color();
//
//		ctx.gl.program(*property.font_shader());
//		ctx.gl.texture(text_.font()->texture(), property.font_shader()->textureChannel);
//		ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
//		ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
//		ctx.gl.render(text_.mesh());
//	}
//}
//
//// -------------------------------------------------------------------------------------------------
//
//RadioButton::RadioButton(std::string name) :
//	ComponentHandler<CoreRadioButton, EventHostSubmitable<RadioButton>>(std::move(name)) { }
//
//RadioButton::RadioButton(GenerateName_t gen, const std::string_view type) :
//	ComponentHandler<CoreRadioButton, EventHostSubmitable<RadioButton>>(gen, type) { }
//
//RadioButton::RadioButton(core_ptr core) noexcept :
//	ComponentHandler<CoreRadioButton, EventHostSubmitable<RadioButton>>(core) { }
//
//// -------------------------------------------------------------------------------------------------
//
//void RadioButton::color(Color value) {
//	AccessProperty::manual(self(), self().property.bg_color, value);
//}
//
//const Color& RadioButton::color() const noexcept {
//	return self().property.bg_color();
//}
//
//void RadioButton::image(Texture2D_view value) {
//	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
//}
//
//const Texture2D_view& RadioButton::image() const noexcept {
//	return self().property.bg_image();
//}
//
//void RadioButton::shader(ShaderImage_view value) {
//	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
//}
//
//const ShaderImage_view& RadioButton::shader() const noexcept {
//	return self().property.bg_shader();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void RadioButton::align_horizontal(AlignHorizontal value) {
//	AccessProperty::setter(self(), self().property.align_horizontal, PropertyDriver::manual, [&]() {
//		self().text_.align_horizontal(value);
//	});
//}
//
//AlignHorizontal RadioButton::align_horizontal() const noexcept {
//	return self().text_.align_horizontal();
//}
//
//void RadioButton::align_vertical(AlignVertical value) {
//	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
//		 self().text_.align_vertical(value);
//	});
//}
//
//AlignVertical RadioButton::align_vertical() const noexcept {
//	return self().text_.align_vertical();
//}
//
//void RadioButton::font(Font2D_view value) {
//	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
//		self().text_.font(std::move(value));
//	});
//}
//
//const Font2D_view& RadioButton::font() const noexcept {
//	return self().text_.font();
//}
//
//void RadioButton::font_size(FontSize value) {
//	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
//		self().text_.size(value);
//	});
//}
//
//FontSize RadioButton::font_size() const noexcept {
//	return self().text_.size();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void RadioButton::text(std::string value) {
//	self().text_.string(std::move(value));
//	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
//}
//
//const std::string& RadioButton::text() const noexcept {
//	return self().text_.string();
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
