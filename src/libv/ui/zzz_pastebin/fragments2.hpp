// Project: libv.ui, File: src/libv/ui/zzz_pastebin/fragments2.hpp

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


#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/event_host.hpp>
#include <libv/ui/style/style_state.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct FragmentAccess {
	template <typename Fragment>
	FragmentAccess& operator()(Fragment& fragment) {

		fragment.access(*this);

		return *this;
	}
};

struct FragmentContextLayout1 {
	libv::vec2f fragmentLayout1(const ContextLayout1& environment);

};

struct FragmentContextLayout2 {
	void fragmentLayout2(const ContextLayout2& environment);
};

// -------------------------------------------------------------------------------------------------

struct BaseFragment {
//	template <typename Self>
//	LIBV_FORCE_INLINE void fragmentOperation(Self& self, const EventFocus& event) {
//		(void) event;
//	}

	template <typename Self, typename... Contents>
	LIBV_FORCE_INLINE void fragmentOperation(Self& self, const EventFocus& event, Contents&... contents) {
		(void) self;
		(void) event;
		(void) contents;
	}

	// ---

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

	LIBV_FORCE_INLINE libv::vec2f fragmentLayout1(const ContextLayout1& environment) {
		(void) environment;
		return libv::vec2f::zero();
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

struct FragmentPadding : BaseFragment {
	libv::vec4f padding;

	template <typename Self, typename... Contents>
	libv::vec2f fragmentLayout1(Self& self, const ContextLayout1& environment, Contents&... contents) {
		const auto padding_size = xy(padding) + zw(padding);

		return libv::max(contents.fragmentLayout1(environment.size - padding_size) + padding_size...);
	}

	template <typename Self, typename... Contents>
	void fragmentLayout2(Self& self, const ContextLayout2& environment, Contents&... contents) {
		const auto padding_BL = xy(padding);
		const auto padding_size = xy(padding) + zw(padding);

		const auto content_environment = environment.enter(padding_BL, environment.size - padding_size);
		(contents.fragmentLayout2(content_environment), ...);
	}

	template <typename Self, typename... Contents>
	void fragmentRender(Self& self, Renderer& r, Contents&... contents) {
		const auto padding_BL = xy(padding);

		const auto t_guard = r.translate_guard(padding_BL);
		(contents.fragmentRender(r), ...);
	}
};

struct FragmentLayer : BaseFragment {
	void stuffThatControls(Event event) {
		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}

	template <typename Self, typename... Contents>
	libv::vec2f fragmentLayout1(Self& self, const ContextLayout1& environment, Contents&... contents) {
		return libv::max(contents.fragmentLayout1(environment)...);
	}

	template <typename Self, typename... Contents>
	void fragmentLayout2(Self& self, const ContextLayout2& environment, Contents&... contents) {
		(contents.fragmentLayout2(environment), ...);
	}

	template <typename Self, typename... Contents>
	void fragmentRender(Self& self, Renderer& r, Contents&... contents) {
		(contents.fragmentRender(r), ...);
	}
};

//template <auto onSubmit = [](const auto& self) { self.fire<EventSubmit>(EventSubmit{self}}); }>
struct FragmentButton : BaseFragment {
	int submit_on = enter | kpenter | space | lmb_press | lmb_release | click;
	int wants_focus_on_submit = true;

	template <typename Self, typename... Contents>
	void fragmentFocus(Self& self, const EventFocus& event, Contents&... contents) {
		self.style_state(StyleState::focus, event.gain());
		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}

	template <typename Self, typename... Contents>
	void fragmentOnMouseButton(Self& self, const EventMouseButton& event, Contents&... contents) {
		if (submit_on.matches(event)) {
			self.style_state(StyleState::active, true);
			// guard active state

			if (wants_focus_on_submit)
				self.style_state(StyleState::focus, true);

			onSubmit(self);
		}

		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}

	template <typename Self, typename... Contents>
	void fragmentOnMouseMovement(Self& self, const EventMouseMovement& event, Contents&... contents) {
		if (event.enter)
			self.style_state(StyleState::hobver, true);

		if (event.leave)
			self.style_state(StyleState::hobver | StyleState::active, false);

		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}

	template <typename Self, typename... Contents>
	void fragmentOnMouseScroll(Self& self, const EventMouseScroll& event, Contents&... contents) {
		(void) event;
		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}

	template <typename Self, typename... Contents>
	void fragmentOnKeyboard(Self& self, const EventKey& event, Contents&... contents) {
		if (submit_on.matches(event)) {
			self.style_state(StyleState::active, true);
			// guard active state

			if (wants_focus_on_submit)
				self.style_state(StyleState::focus, true);

			onSubmit(self);
		}

		((contents.stuffThatControls(event), event.propagation_stopped()) || ...);
	}
};

// -------------------------------------------------------------------------------------------------
//
//template <typename Content>
//struct FragmentPadding2 : Content {
//	libv::vec4f padding;
//
//	libv::vec3f fragmentLayout1(const ContextLayout1& environment) {
//		return {Content::fragmentLayout1(environment.size - padding_size()) + padding_size(), 0.f};
//	}
//
//	void fragmentLayout2(const ContextLayout2& environment) {
//		Content::fragmentLayout2(environment.enter(padding_BL(), environment.size - padding_size()));
//	}
//
//	void fragmentRender(Renderer& r) {
//		r.translate(padding_BL());
//		Content::fragmentRender(r);
//	}
//};

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
				environment.size,
				property.font(),
				property.font_size());
	}

	void fragmentLayout2(const ContextLayout2& environment) {
		text.layout2(
				environment.size,
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

// -------------------------------------------------------------------------------------------------

void foo() {
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

			access >
				background +
				button +
				padding +
				line(
					icon +
					spacing +
					text
			);
		}
	};

	struct ButtonType {
		ButtonCore* core;

		void foo() {
			core->padding;
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
