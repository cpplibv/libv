// Project: libv.ui, File: sandbox/libv_ui_scroll.cpp

// ext
#include <fmt/format.h>
// pro
// #include <libv/ui/component/button.hpp>
// #include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/scroll_pane.hpp>


// -------------------------------------------------------------------------------------------------

// template <typename Control>
// void focusControlled(libv::ui::Component& component, libv::ctrl::Controls& controls, Control* controlPtr) {
// 	component.event().focus.connect([controls, controlPtr](const libv::ui::EventFocus& event) mutable {
// 		if (event.gain())
// 			controls.context_enter<Control>(controlPtr);
// 		else
// 			controls.context_leave_if_matches<Control>(controlPtr);
// 	});
// 	component.event().focus_within.connect([controls, controlPtr](const libv::ui::EventFocusWithin& event) mutable {
// 		if (event.gain())
// 			controls.context_enter<Control>(controlPtr);
// 		else
// 			controls.context_leave_if_matches<Control>(controlPtr);
// 	});
// }
//
// // ---
//
// struct TestCardControl {
// 	libv::ui::Label lbl;
// 	int card = 0;
// 	int count = 0;
//
// 	static void register_controls(libv::ctrl::Controls& controls) {
// 		controls.feature_action<TestCardControl>("test.add", [](libv::ctrl::arg_action, TestCardControl& state) {
// 			state.lbl.text(fmt::format("Card {} counter: {}", state.card, ++state.count));
// 		});
// 		controls.feature_action<TestCardControl>("test.remove", [](libv::ctrl::arg_action, TestCardControl& state) {
// 			state.lbl.text(fmt::format("Card {} counter: {}", state.card, --state.count));
// 		});
// 	}
//
// 	static void bind_default_controls(libv::ctrl::Controls& controls) {
// 		controls.bind("test.add", "pageup");
// 		controls.bind("test.remove", "pagedown");
// 	}
// };
//
// void registerSurfaceControls(libv::ctrl::Controls& controls) {
// 	TestCardControl::register_controls(controls);
// 	TestCardControl::bind_default_controls(controls);
// }
//
// // -------------------------------------------------------------------------------------------------
//
// struct TabSurfaceData {
// 	libv::ui::Component testBar;
//
// 	struct ToggleTest {
// 		bool value = false;
// 	};
//
// 	[[nodiscard]] libv::ui::Component createControlTestBar(libv::Nexus& nexus, libv::ctrl::Controls& controls) {
// 		(void) nexus;
// 		auto line = libv::ui::PanelLine::ns("controls-test-bar", "surface.test.bar");
// 		line.anchor(libv::ui::Anchor::center_right);
// 		line.orientation(libv::ui::Orientation::left);
// 		line.spacing(10);
//
// 		for (int i = 0; i < 5; ++i) {
// 			auto card = line.add_ns<libv::ui::PanelLine>("test-card", "surface.test.card");
// 			card.orientation(libv::ui::Orientation::up);
// 			auto btnAdd = card.add_nsa<libv::ui::Button>("add", "surface.test.button", "+");
// 			auto btnDel = card.add_nsa<libv::ui::Button>("remove", "surface.test.button", "-");
// 			auto lblCounter = card.add_nsa<libv::ui::Label>("counter", "surface.test.counter", fmt::format("Card {} counter: {}", i, 0));
// 			auto controlPtr = &libv::ui::attach_state<TestCardControl>(card)(lblCounter, i, 0);
// 			focusControlled<TestCardControl>(card, controls, controlPtr);
//
// 			auto testBody = libv::ui::PanelLine::s("surface.test.body");
// 			btnAdd.event().submit.connect([card, testBody]() mutable {
// 				testBody.add_s<libv::ui::InputField>("surface.test.input");
// 				if (card.children_size() == 3)
// 					card.add(testBody);
// 			});
// 			btnDel.event().submit.connect([card, testBody]() mutable {
// 				card.remove(testBody);
// 			});
// 		}
//
// 		return line;
// 	}
// };

libv::ui::Component createTabSurface(libv::ui::UI& ui) {
	(void) ui;
	auto main = libv::ui::PanelLine();
	main.align_horizontal(libv::ui::AlignHorizontal::center);
	main.align_vertical(libv::ui::AlignVertical::center);

	auto pane = main.add_s<libv::ui::ScrollPane>("tabs.scroll.pane");

	// auto tglTest = line.add_sa<libv::ui::ToggleButton>("surface.menu.button", "Hide", "Show");
	// tglTest.event().submit.connect([nexus](libv::ui::ToggleButton& source) {
	// 	nexus.broadcast_global<ToggleTest>(ToggleTest{source.select()});
	// });
	//
	// nexus.connect_global<ToggleTest>(slot, [this, layers, nexus, controls](const ToggleTest& event) mutable {
	// 	if (!event.value) {
	// 		testBar = createControlTestBar(nexus, controls);
	// 		layers.add(testBar);
	// 	} else {
	// 		layers.remove(testBar);
	// 		testBar = libv::ui::Component(nullptr);
	// 	}
	// });
	//
	// pane.content(std::move(line));
	return main;
}
