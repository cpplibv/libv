// Project: libv.ui, File: src/libv/ui/component/_idea_file_picker.cpp
//
//// hpp
//#include <libv/ui/component/file_picker.hpp>
//// ext
////#include <fmt/format.h>
//#include <boost/algorithm/string/replace.hpp>
//#include <boost/container/flat_set.hpp>
////#include <range/v3/view/enumerate.hpp>
//// libv
//#include <libv/algo/wildcard.hpp>
//#include <libv/parse/color.hpp>
//#include <libv/ui/component/image.hpp> // testing only, remove it
//#include <libv/ui/component/label.hpp>
////#include <libv/ui/component/label_image.hpp>
////#include <libv/ui/component/panel_anchor.hpp>
//#include <libv/ui/component/panel_full.hpp>
////#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component/quad.hpp> // testing only, remove it
//#include <libv/ui/component/stretch.hpp> // testing only, remove it
//#include <libv/ui/component/input_field.hpp>
//#include <libv/ui/context/context_ui.hpp>
//#include <libv/ui/event/event_keyboard.hpp>
//#include <libv/ui/parse/parse_size.hpp>
////#include <libv/ui/shader/shader_font.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
//#include <libv/ui/shader/shader_image.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
////#include <libv/ui/shader/shader_quad.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
//// std
////#include <sstream>
//// pro
////#include <vm4_viewer/log.hpp>
//
////#include <vm4_viewer/version.hpp>
//
//
//namespace libv {
//namespace ui {
//
// =================================================================================================
//
//core_ptr QuickFilePicker6::create_core(std::string name) {
//	return create_core_ptr<CoreQuickFilePicker6>(std::move(name));
//}
//
//QuickFilePicker6::~QuickFilePicker6() { }
//
//void QuickFilePicker6::doAttach() {
//	set(property.align_horizontal, libv::ui::AlignHorizontal::left);
//	set(property.align_vertical, libv::ui::AlignVertical::top);
//	set(property.orientation, libv::ui::Orientation::down);
//
////	{
////		auto style = context().style("vm4pv.file_list");
////		style->set("align", libv::ui::AlignHorizontal::left);
////		style->set("align_vertical", libv::ui::AlignVertical::top);
////		style->set("orientation", libv::ui::Orientation::down);
////		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
////		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
////		style->set("font_size", libv::ui::FontSize{11});
////		style->set("size", libv::ui::parse_size_or_throw("d, d"));
////	}
//
//	// -------------------------------------------------------------------------------------------------
//
//	{
//		search_field = std::make_shared<libv::ui::InputField>(*this, "input0");
//		search_field->style(context().style("vm4pv.info.input"));
//		search_field->text(".game.vm4");
//		search_field->event_change(this, [this](const libv::ui::InputField::EventChange& event) {
//			(void) event;
//			this->update_filelist();
//		});
//		search_field->event_submit(this, [this](const libv::ui::InputField::EventSubmit& event) {
//			// Open selected file
//			log_app.info("Submit: {}", event.component.text());
//			this->fire(EventPick{*this});
//		});
//		add(search_field);
//	}
//
//	{
//		list_panel = std::make_shared<libv::ui::PanelFull>(*this, "layer");
//		auto& child = add(list_panel);
//		child.ptr->set(child.property.size, libv::ui::parse_size_or_throw("1rD, D"));
//	}
//
//	{
//		const auto label_help = std::make_shared<libv::ui::Label>(*this, "help");
//		label_help->style(context().style("vm4pv.file_list"));
//		label_help->text(
//				"[Up/Down] Navigate  [Enter] Open  [A-Z] Search\n"
//				"? Wildcard matching a single character\n"
//				"* Wildcard matching any single level file or directory name\n"
//				"** Wildcard matching any recursive file or directory name"
//		);
//		auto& child = add(label_help);
//		child.ptr->set(child.property.size, libv::ui::parse_size_or_throw("1rD, D"));
//	}
//
//	watchKey(true);
//	update_filelist();
//}
//
//void QuickFilePicker6::onKey(const libv::ui::EventKey& event) {
//	if (event.keycode == libv::input::Keycode::Up && event.action != libv::input::Action::release) {
//		select--;
//		log_app.info("Select {}", select);
//		update_filelist();
//		return event.stop_propagation();
//	}
//
//	if (event.keycode == libv::input::Keycode::Down && event.action != libv::input::Action::release) {
//		select++;
//		log_app.info("Select {}", select);
//		update_filelist();
//		return event.stop_propagation();
//	}
//}
//
//void QuickFilePicker6::update_filelist() {
//	list_panel->clear();
//
//	{
//		const auto temp = std::make_shared<libv::ui::Quad>(*list_panel, "bg-quad");
//		temp->style(context().style("vm4pv.info.bg"));
//		list_panel->add(temp);
//	} {
//		const auto temp = std::make_shared<libv::ui::Stretch>(*list_panel, "border");
//		temp->style(context().style("vm4pv.info.slim_border"));
//		list_panel->add(temp);
//	} {
//		const auto panel_files = std::make_shared<libv::ui::PanelLine>(*list_panel, "list");
//		panel_files->style(context().style("vm4pv.file_list"));
//
////			path = "res/model";
//		path = "res/";
//		boost::container::flat_set<std::filesystem::path> exts_vm = {".vm3", ".vm4", ".vm5"};
//		boost::container::flat_set<std::filesystem::path> exts_importable = {".dae"};
//
////		const auto filter_pattern = search_field->text(); // full filter
////		const auto filter_pattern = search_field->text() + "**"; // prefix filter
////		const auto filter_pattern = "**" + search_field->text() + "**"; // any filter
//		const auto filter_pattern = "**" + boost::algorithm::replace_all_copy(search_field->text(), " ", "**") + "**"; // relax filter
////		const auto filter_pattern = not wildcard just match every split by space and / in search_field->text() // lose filter
//
//		switch (pathDisplayMode) {
//		case PathDisplayMode::flat: {
//			int i = 0;
//			for(auto& p : std::filesystem::recursive_directory_iterator(path)) {
//				if (not p.is_regular_file())
//					continue;
//
//				if (not libv::match_wildcard_glob(libv::generic_path(p), filter_pattern))
//					continue;
//
//				auto ext = p.path().extension();
//				auto entry = std::make_shared<libv::ui::Label>(*list_panel, "entry");
//
//				if (select == i++) {
//					value_ = libv::generic_path(p);
//					entry->style(context().style("vm4pv.file_list.entry.selected"));
//				}
//				else if (exts_vm.contains(ext))
//					entry->style(context().style("vm4pv.file_list.entry.vm"));
//				else if (exts_importable.contains(ext))
//					entry->style(context().style("vm4pv.file_list.entry.importable"));
//				else
//					entry->style(context().style("vm4pv.file_list.entry.other"));
//
//				entry->text(libv::generic_path(p));
//				panel_files->add(entry);
//			}
//
//			break;
//		}
//		case PathDisplayMode::tree: {
//			for(auto& p : std::filesystem::directory_iterator(path)) {
//				if (p.is_directory()) {
//					add_button();
//
//					continue;
//				} else if (p.is_regular_file()) {
//					if (not libv::match_wildcard_glob(libv::generic_path(p), filter_pattern))
//						continue;
//
//					auto ext = p.path().extension();
//					auto entry = std::make_shared<libv::ui::Label>(*list_panel, "entry");
//
//					if (select == i++) {
//						value_ = libv::generic_path(p);
//						entry->style(context().style("vm4pv.file_list.entry.selected"));
//					}
//					else if (exts_vm.contains(ext))
//						entry->style(context().style("vm4pv.file_list.entry.vm"));
//					else if (exts_importable.contains(ext))
//						entry->style(context().style("vm4pv.file_list.entry.importable"));
//					else
//						entry->style(context().style("vm4pv.file_list.entry.other"));
//
//					entry->text(libv::generic_path(p));
//					panel_files->add(entry);
//				}
//			}
//
//			break;
//		}
//		}
//
//		list_panel->add(panel_files);
//	}
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv
