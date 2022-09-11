// Project: libv.ui, File: src/libv/ui/component/button_core.hpp

#pragma once

// hpp
#include <libv/ui/component/button_core.hpp>
#include <libv/ui/component/toggle_button.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/text_layout.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class CoreBaseButton {
//
//};

class CoreToggleButton : public CoreButton {
//class CoreToggleButton : public CoreBaseButton {
public:
	friend class ToggleButton;
	[[nodiscard]] inline auto handler() { return ToggleButton{this}; }

public:
	template <typename T> static void access_properties(T& ctx);
	struct Properties {
		PropertyR<bool> selection;
	} property;

private:
//	TextLayout text_on;
//	TextLayoutProperties text_properties;


//	TextProperties text_properties;
//	TextBuilder text_builder;
//	TextFragment text_on;
//	TextFragment text_off;

	TextLayoutLite text_on;
	TextLayoutLite text_off;

//	void set_size(int size_value) {
//		text_builder.set_size(size_value);
//	}
//
//	void render() {
//		text_builder.check_dirt(text_on);
//		text_builder.check_dirt(text_off);
//	}

public:
//	using CoreBaseButton::CoreBaseButton;
	using CoreButton::CoreButton;

private:
	virtual void onSubmit() override;

public:
	virtual	libv::vec4f getInnerContentBounds() override;

protected:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
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
