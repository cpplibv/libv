// Project: libv.ui, File: src/libv/ui/component/toggle_button_core.hpp

#pragma once

// hpp
#include <libv/ui/component/button_core.hpp>
#include <libv/ui/component/toggle_button.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_text.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreToggleButton : CoreButton {
	using base_type = CoreButton;
	using base_type::base_type;

	// TODO P5: Remove handler()
	[[nodiscard]] inline auto handler() { return ToggleButton{this}; }

public:
	struct Properties {
		PropertyR<bool> selection;
	} property;

	template <typename T> static void access_properties(T& ctx);

public:
//	LayoutText text_on;
//	LayoutTextProperties text_properties;

//	TextProperties text_properties;
//	TextBuilder text_builder;
//	TextFragment text_on;
//	TextFragment text_off;

	LayoutTextLite text_on;
	LayoutTextLite text_off;

//	void set_size(int size_value) {
//		text_builder.set_size(size_value);
//	}
//
//	void render() {
//		text_builder.check_dirt(text_on);
//		text_builder.check_dirt(text_off);
//	}

public:
	virtual void onSubmit() override;

public:
	virtual	libv::vec4f getInnerContentBounds() override;

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual libv::vec2f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreToggleButton::access_properties(T& ctx) {
	ctx.synthesize(
			T::handler_setter(&ToggleButton::text_on),
			T::handler_getter(&ToggleButton::text_on),
			pgr::behaviour, pnm::text_on,
			"Displayed text when on"
	);
	ctx.synthesize(
			T::handler_setter(&ToggleButton::text_off),
			T::handler_getter(&ToggleButton::text_off),
			pgr::behaviour, pnm::text_off,
			"Displayed text when off (default state)"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.selection; },
			false,
			pgr::behaviour, pnm::selection, "Selection state"
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
