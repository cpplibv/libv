// Project: libv.ui, File: sandbox/libv_ui_main.cpp

// pro
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/overlay/overlay_tooltip.hpp>
#include <libv/ui/ui.hpp>
#include <libv/ui/log.hpp>



// --- Tooltip ---------------------------------------------------------------------------------------

libv::ui::Component createTabTooltip(libv::ui::UI& ui) {
	libv::ui::PanelLine card{"Tooltip"};
	libv::ui::PanelLine line{"TooltipLine"};
	card.style("tooltip.card");
	line.style("tooltip.line");

	libv::ui::Button button;
	{
		button.text("«Nothing is true; everything is permitted»\n"
				 "\n"
				 "Most of the important things in the world have been accomplished by people who\n"
				 "have kept on trying when there seemed to be no hope at all.");
		button.align_horizontal(libv::ui::AlignHorizontal::center);
		button.align_vertical(libv::ui::AlignVertical::center);
		button.font_color({1, 0.5, 0.5, 1});
		//lbl.font_size(120);
		button.style("tooltip.button1");
	}
	libv::ui::Button button2;
	{
		button2.text("«Nothing is false; everything is forbidden»\n"
				 "\n"
				 "Most of the important things in the world have been accomplished by people who\n"
				 "have kept on trying when there seemed to be no hope at all.");
		button2.align_horizontal(libv::ui::AlignHorizontal::center);
		button2.align_vertical(libv::ui::AlignVertical::center);
		button2.font_color({1, 0.5, 0.5, 1});
		//lbl.font_size(120);
		button2.style("tooltip.button1");
	}

	auto tooltip = libv::ui::Label::nsa("tooltip", "tooltip.tooltip01", "Tooltip text");
	{
		tooltip.align_horizontal(libv::ui::AlignHorizontal::center);
		tooltip.align_vertical(libv::ui::AlignVertical::center);
		tooltip.font_color({1, 1, 1, 1});
		tooltip.show(false);
		button.show_tooltip(tooltip);
	}

	auto tooltip2 = libv::ui::Label::nsa("tooltip2", "tooltip.tooltip01", "Tooltip2 text");
	{
		tooltip2.align_horizontal(libv::ui::AlignHorizontal::center);
		tooltip2.align_vertical(libv::ui::AlignVertical::center);
		tooltip2.font_color({1, 1, 1, 1});
		tooltip2.show(false);
		button2.show_tooltip(tooltip2);
	}

	button.event().mouse_movement.connect([tooltip](libv::ui::Button& button, libv::ui::EventMouseMovement event) mutable {
		// libv::ui::log_ui.fatal_if(event.enter || event.leave, "tooltip 1 enter: {}, leave: {}", event.enter, event.leave);
		if(event.enter)
			tooltip.show(true);
		if(event.leave)
			tooltip.show(false);
	});

	button2.event().mouse_movement.connect([tooltip2](libv::ui::Button& button, libv::ui::EventMouseMovement event) mutable {
		// libv::ui::log_ui.fatal_if(event.enter || event.leave, "tooltip 2 enter: {}, leave: {}", event.enter, event.leave);
		if(event.enter)
			tooltip2.show(true);
		if(event.leave)
			tooltip2.show(false);
	});

	line.spacing(10);

	line.add(button);
	line.add(button2);
	card.add(std::move(line));
	card.add_a<libv::ui::Label>("Text");

	return card;
}

// -------------------------------------------------------------------------------------------------
