// Project: libv.ui, File: src/libv/ui/component/button_core.hpp

#pragma once

// hpp
#include <libv/ui/component/button.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/text_layout.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//struct TextProperties {
//	PropertyL1L2LP<std::shared_ptr<Font2D>> font;
//	PropertyL1L2LP<FontSize> font_size;
//	PropertyL1L2<AlignHorizontal> align_horizontal;
//	PropertyL1L2<AlignVertical> align_vertical;
//};

class CoreButton : public CoreComponent {
public:
	friend class Button;
	[[nodiscard]] inline auto handler() { return Button{this}; }

public:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Background> background;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		// LP what does LP means again ???, do font font_size need it?
		PropertyL1L2LP<std::shared_ptr<Font2D>> font;
		PropertyL1L2LP<FontSize> font_size;
		PropertyL2<AlignHorizontal> align_horizontal;
		PropertyL2<AlignVertical> align_vertical;

//		TextProperties text_properties;

//		PropertyL2<> align_horizontal;
//		PropertyL2<> align_vertical;
//		PropertyL1L2<> font;
//		PropertyL1L2<> font_size;
	} property;

private:
	TextLayoutLite text_;

public:
	using CoreComponent::CoreComponent;

protected:
	void routineSubmit();
	virtual void onSubmit() {}

private:
	virtual void onFocus(const EventFocus& event) override;
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

public:
	virtual	libv::vec4f getInnerContentBounds() override;

protected:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreButton::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::color({0.9f, 1.f, 0.9f, 1.f}),
			pgr::appearance, pnm::background, "Background"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(0, 0, 0, 1),
			pgr::appearance, pnm::font_color, "Font color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_shader; },
			[](auto& u) { return u.shaderFont(); },
			pgr::appearance, pnm::font_shader, "Font shader"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font; },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font, "Font file"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_size; },
			FontSize{12},
			pgr::font, pnm::font_size, "Font size in pixel"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			AlignHorizontal::left,
			pgr::appearance, pnm::align_horizontal, "Horizontal alignment of the text"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			AlignVertical::top,
			pgr::appearance, pnm::align_vertical, "Vertical alignment of the text"
	);
	ctx.synthesize(
			T::handler_setter(&Button::text),
			T::handler_getter(&Button::text),
			pgr::behaviour, pnm::text, "Displayed text"
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
