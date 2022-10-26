// Project: libv.ui, File: src/libv/ui/zzz_pastebin/fragments.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// std
#include <type_traits>
// pro
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/property/align_horizontal.hpp>
#include <libv/ui/property/align_vertical.hpp>
#include <libv/ui/property/background.hpp>
#include <libv/ui/property/font_2D.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property_system/property.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseFragment {
//	template <typename Self>
//	LIBV_FORCE_INLINE void fragmentOperation(Self& self, const EventFocus& event) {
//		(void) event;
//	}

	LIBV_FORCE_INLINE void fragmentFocus(const EventFocus& event) {
		(void) event;
	}

	LIBV_FORCE_INLINE void fragmentOnMouseButton(const EventMouseButton& event) {
		(void) event;
	}

	LIBV_FORCE_INLINE void fragmentOnMouseMovement(const EventMouseMovement& event) {
		(void) event;
	}

	LIBV_FORCE_INLINE void fragmentOnMouseScroll(const EventMouseScroll& event) {
		(void) event;
	}

	LIBV_FORCE_INLINE void fragmentOnKeyboard(const EventKey& event) {
		(void) event;
	}

	LIBV_FORCE_INLINE libv::vec3f fragmentLayout1(const ContextLayout1& environment) {
		(void) environment;
		return libv::vec3f::zero();
	}

	LIBV_FORCE_INLINE void fragmentLayout2(const ContextLayout2& environment) {
		(void) environment;
	}

	LIBV_FORCE_INLINE void fragmentRender(Renderer& r) {
		(void) r;
	}

	// Any other fragment function...
};

static_assert(std::is_empty_v<BaseFragment>); // NOTE: BaseFragment has to be empty as it will be derived from multiple times for each fragment compositions

// -------------------------------------------------------------------------------------------------

template <typename Content>
struct FragmentPadding2 : Content {
	libv::vec4f padding;

	libv::vec3f fragmentLayout1(const ContextLayout1& environment) {
		return {Content::fragmentLayout1(environment.size - padding_size()) + padding_size(), 0.f};
	}

	void fragmentLayout2(const ContextLayout2& environment) {
		Content::fragmentLayout2(environment.enter(padding_BL(), environment.size - padding_size()));
	}

	void fragmentRender(Renderer& r) {
		r.translate(padding_BL());
		Content::fragmentRender(r);
		r.translate(-padding_BL());
	}
};

struct FragmentBackground : BaseFragment {
	Background background;

	void fragmentRender(Renderer& r) {
		background.render(r, pos, size, component);
	}
};

struct FragmentText : BaseFragment {
	PropertyL1L2LP<std::shared_ptr<Font2D>> font;
	PropertyL1L2LP<FontSize> font_size;
	PropertyL2<AlignHorizontal> align_horizontal;
	PropertyL2<AlignVertical> align_vertical;

	LayoutTextLite text;

	libv::vec3f fragmentLayout1(const ContextLayout1& environment) {
		return text.measure_content_size(
				xy(environment.size),
				property.font(),
				property.font_size());
	}

	void fragmentLayout2(const ContextLayout2& environment) {
		text.layout2(
				xy(environment.size),
				property.font(),
				property.font_size(),
				property.align_horizontal(),
				property.align_vertical());
	}

	void fragmentRender(Renderer& r) {
		r.text(
				text.vertices_data(),
				property.font(),
				property.font_color(),
				property.font_shader());
	}
};

template <typename Content>
struct FragmentButton : Content {

//	int when_submit = LMB | KEY_Enter | KEY_KPEnter | KEY_Space;

	void fragmentFocus(const EventFocus& event) {
		style_state(StyleState::focus, event.gain());
	}

	void fragmentOnMouseButton(const EventMouseButton& event) {
		if (event.action == libv::input::Action::release && event.button == libv::input::MouseButton::Left)
			style_state(StyleState::active, false);

		if (event.action == libv::input::Action::press)
			focus();

		fire(EventMouseButton{event});

		// TODO P3: use hotkey event (ui.select) (even for mouse)
		if (event.action == libv::input::Action::press && event.button == libv::input::MouseButton::Left) {
			style_state(StyleState::active, true);
			fire(EventSubmit{});
		}

		event.stop_propagation();
	}

	void fragmentOnMouseMovement(const EventMouseMovement& event) {
		if (event.enter)
			style_state(StyleState::hover, true);

		if (event.leave)
			style_state(StyleState::hover, false);

		// TODO P1: style_state(StyleState::active, false); after the mouse leaves the component area (while it was active): maybe it has to acquire or soft acquire the mouse? so it can track the release (ergo deactivate) event
		//		style_state(StyleState::active, false);

		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
		fire(EventMouseMovement{event});

		event.stop_propagation();
	}

	void fragmentOnMouseScroll(const EventMouseScroll& event) {
		fire(EventMouseScroll{event});

		event.stop_propagation();
	}

	void fragmentOnKeyboard(const EventKey& event) {
		if (
				event.keycode != libv::input::Keycode::Enter ||
				event.keycode != libv::input::Keycode::KPEnter ||
				event.keycode != libv::input::Keycode::Space)
			return;

		style_state(StyleState::active, true);
		fire(EventSubmit{});

		fire(EventKey{event});

		event.stop_propagation();
	}
};

template <typename... Content>
struct FragmentLayer : Content... {

	void stuffThatControls(stuff...) {
		((Content::stuffThatControls(stuff...), event.propagation_stopped()) || ...);
	}

	libv::vec3f fragmentLayout1(const ContextLayout1& environment) {
		return libv::max(Content::fragmentLayout1(environment)...);
	}

	void fragmentLayout2(const ContextLayout2& environment) {
		(Content::fragmentLayout2(environment), ...);
	}

	void fragmentRender(Renderer& r) {
		(Content::fragmentRender(r), ...);
	}
};

// -------------------------------------------------------------------------------------------------

void foo() {
	FragmentComposition{
		FragmentButton{
			FragmentLayer{
				FragmentBackground{},
				FragmentPadding{
					FragmentText
				}
			}
		}
	};



	FragmentComposition<
		FragmentButton<
			FragmentLayer<
				FragmentBackground,
				FragmentPadding<
					FragmentText
				>
			>
		>
	>{};



	struct B {
		[[no_unique_address]] FragmentBackground background;
	};
	struct T {
		[[no_unique_address]] FragmentText text;
	};
	struct P {
		[[no_unique_address]] FragmentPadding<T> padding;
	};
	struct L {
		[[no_unique_address]] FragmentLayer<B, P> layer;
	};
	struct Composition {
		[[no_unique_address]] FragmentButton<L> button;
	};

	Composition x;
	x.button.layer.padding.text.text.string("Hello World!");



//#define UseFragment(S, Type, Name, Children...) \
//    struct S {                                  \
//		[[no_unique_address]] Type<Children...> Name; \
//	};
//
//
//	UseFragment(L, FragmentLayer, layer,
//		UseFragment(P, FragmentPadding, padding,
//			UseFragment(T, FragmentText, text)
//		),
//		UseFragment(B, FragmentBackground, background)
//	);
//
//
//	[[no_unique_address]] FragmentButton<L> button;
//
//	CompositionMacro x;
//	x.button.layer.padding.text.text.string("Hello");



	auto y =
			fragmentButton(
				fragmentLayer(
					fragmentBackground(),
					fragmentPadding(
						fragmentText("Hello World!")
					)
				)
			);



	auto dual_on_off_button =
			fragmentLine(
				fragmentButton(MAP_STYLE("on"), MAP_EVENTS("on"))(
					fragmentLayer(
						fragmentBackground(),
						fragmentPadding(
							fragmentText("Hello World!")
						)
					)
				),
				fragmentButton(MAP_STYLE("off"), MAP_EVENTS("off"))(
					fragmentLayer(
						fragmentBackground(),
						fragmentPadding(
							fragmentText("Hello World!")
						)
					)
				)
			);


	struct ButtonCore {
		Background background;
		ButtonHandler button;
		Padding padding;
		Text text;

		template <typename T>
		auto foo(T& access) {
			access(
				background(
					button(
						padding(
							line(
								icon,
								spacing,
								text
							)
						)
					)
				)
			);

			access
				| background
				| button
				| padding
				| line(
					icon,
					spacing,
					text
				);

			access(
				+ background
				+ button
				+ padding
				+ line
					* icon
					* spacing
					* text
			);
		}
	};

	struct CoreTemplate {
		template <typename T>
		void access_properties(T& ctx);
		template <typename T>
		void access_components(T& ctx);
	};
}


// =================================================================================================



// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
