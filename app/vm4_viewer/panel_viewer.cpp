// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/panel_viewer.hpp>
// libv
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/panel.hpp>
// pro
#include <vm4_viewer/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

PanelViewer::PanelViewer() : libv::ui::ComponentStatic<PanelViewer>("PanelViewer") {
}

//void PanelViewer::create() {
//	add(panel_left);
//	add(panel_help);
//
//	panel_left->add(label_info);
//	label_info->setText("Hello world!");
//	label_info->properties.font

//	const auto style_label_01 = ui.context().style("style-label-01");
//	style_label_01->set("color", libv::parse::parse_color_or_throw("rgba(167, 152, 120, 100%)"));
//	style_label_01->set("font", ui.context().font("consola.ttf"));
//	style_label_01->set("align", libv::ui::Anchor::Left);
//
//	label0->properties.set(style_label_01);
//	label0->setText("Hello, Label0!");
//	label1->properties.set(style_label_01);
//	label1->setText("Hello, Label1!");
//	label2->properties.set(style_label_01);
//	label2->setText("Hello, Label2!");
//	label2->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");
//
//	quad0->color(libv::parse::parse_color_or_throw("rgba(134, 189, 111, 80%)"));
//	stretch0->image(ui.context().texture2D("stretch_border.png"));
//	stretch0->color(libv::parse::parse_color_or_throw("rgba(183, 190, 135, 100%)"));
//	image0->image(ui.context().texture2D("atlas_ui_slate.png"));
//	image0->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");

//		panel0->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel0->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel0->layout.orient = libv::ui::Orientation::TOP_TO_BOTTOM;
//		panel0->add(label0);
//		panel0->add(image0);
//		panel0->add(panel1);
//		panel0->add(label1);
//
//		panel1->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel1->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel1->layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//		panel1->add(quad0);
//		panel1->add(label2);
//		panel1->add(stretch0);
//
//		ui.add(panel0);
//}
//
//void PanelViewer::render() {
//}
//
//void PanelViewer::destroy() {
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
