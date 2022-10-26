// Project: libv.ui, File: sandbox/libv_ui_scroll.cpp

// ext
#include <fmt/format.h>
// pro
// #include <libv/ui/component/button.hpp>
// #include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/scroll_pane.hpp>


// --- Scroll --------------------------------------------------------------------------------------

libv::ui::Component createTabScroll() {
	auto main = libv::ui::PanelLine();
	main.align_horizontal(libv::ui::AlignHorizontal::center);
	main.align_vertical(libv::ui::AlignVertical::center);

	auto pane = main.add_s<libv::ui::ScrollPane>("tabs.scroll.pane");
	auto line = libv::ui::PanelLine::s("tabs.scroll.line");

	for (int i = 0; i < 100; ++i)
		line.add_sa<libv::ui::Label>("tabs.scroll.lbl", fmt::format("Label {}", i));

	pane.content(std::move(line));
	return main;
}
