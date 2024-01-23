// Project: libv.ui, File: sandbox/ui/tab_buttons.cpp

// ext
#include <fmt/format.h>
// pro
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/scroll_pane.hpp>
#include <libv/ui/component/selection_group.hpp>
#include <libv/ui/component/toggle_button.hpp>

#include "log.hpp"


// -------------------------------------------------------------------------------------------------

libv::ui::Component createTabButtons() {
	libv::ui::PanelLine card{"card-buttons-root"};

	libv::ui::PanelLine line{"card-buttons-line"};
//		line.size(libv::ui::parse_size_or_throw("250px, 300px"));
	line.orientation(libv::ui::Orientation::down);
	line.anchor(libv::ui::Anchor::center_center);
	line.spacing(10);
	line.padding(10);
	card.add(line);

//		libv::ui::Label label;
//		label.text("Buttons");
//		label.align_vertical(libv::ui::AlignVertical::center);
//		label.align_horizontal(libv::ui::AlignHorizontal::center);
//		line.add(label);

	libv::ui::Button button0;
	button0.text("Normal");
	button0.padding(10);
	line.add(button0);

	libv::ui::ToggleButton button1;
	button1.text_on("Toggle: ON");
	button1.text_off("Toggle: OFF");
	button1.padding(10);
	line.add(button1);

	// ---

	libv::ui::PanelLine row0{"card-buttons-row0"};
	row0.orientation(libv::ui::Orientation::right);
	row0.spacing(10);
	line.add(row0);

	libv::ui::ToggleButton g1Button0;
	g1Button0.text_on("G1 Option A: ON");
	g1Button0.text_off("G1 Option A: OFF");
	g1Button0.padding(10);
	row0.add(g1Button0);

	libv::ui::ToggleButton g1Button1;
	g1Button1.text_on("G1 Option B: ON");
	g1Button1.text_off("G1 Option B: OFF");
	g1Button1.padding(10);
	row0.add(g1Button1);

	libv::ui::ToggleButton g1Button2;
	g1Button2.text_on("G1 Option C: ON");
	g1Button2.text_off("G1 Option C: OFF");
	g1Button2.padding(10);
	row0.add(g1Button2);

	libv::ui::SelectionGroup<std::string> group1;
	group1.event().change.connect([](libv::ui::SelectionGroup<std::string>& group) {
		log_sandbox.info_if(group.has_value(), "Group1 change: {}", group.value());
		log_sandbox.info_if(!group.has_value(), "Group1 has no value");
	});
	group1.add(g1Button0, "G1:A");
	group1.add(g1Button1, "G1:B");
	group1.add(g1Button2, "G1:C");

	// ---

	libv::ui::PanelLine row1{"card-buttons-row1"};
	row1.orientation(libv::ui::Orientation::right);
	row1.spacing(10);
	line.add(row1);

	libv::ui::ToggleButton g2Button0;
	g2Button0.text_on("G2 Option A: ON");
	g2Button0.text_off("G2 Option A: OFF");
	g2Button0.padding(10);
	row1.add(g2Button0);

	libv::ui::ToggleButton g2Button1;
	g2Button1.text_on("G2 Option B: ON");
	g2Button1.text_off("G2 Option B: OFF");
	g2Button1.padding(10);
	row1.add(g2Button1);

	libv::ui::ToggleButton g2Button2;
	g2Button2.text_on("G2 Option C: ON");
	g2Button2.text_off("G2 Option C: OFF");
	g2Button2.padding(10);
	row1.add(g2Button2);

	libv::ui::SelectionGroup<std::string> group2;
	group2.event().change.connect([](libv::ui::SelectionGroup<std::string>& group) {
		log_sandbox.info_if(group.has_value(), "Group2 change: {}", group.value());
		log_sandbox.info_if(!group.has_value(), "Group2 has no value");
	});
	group2.add(g2Button0, "G2:A");
	group2.add(g2Button1, "G2:B", true);
	group2.add(g2Button2, "G2:C");

	// Overlapping selection groups are just chaos event graph wise, Skip for now
	//		libv::ui::SelectionGroup<std::string> groupC;
	//		group2.event().change.connect([](libv::ui::SelectionGroup<std::string>& group) {
	//			log_sandbox.info_if(group.has_value(), "GroupC change: {}", group.value());
	//			log_sandbox.info_if(!group.has_value(), "GroupC has no value");
	//		});
	//		group2.add(g1Button2, "GC:G1:C");
	//		group2.add(g2Button2, "GC:G2:C");

	return card;
}
