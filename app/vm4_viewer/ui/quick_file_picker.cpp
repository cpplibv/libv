// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/ui/quick_file_picker.hpp>
// ext
//#include <fmt/format.h>
#include <boost/container/flat_set.hpp>
//#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/image.hpp> // testing only, remove it
#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/quad.hpp> // testing only, remove it
#include <libv/ui/component/stretch.hpp> // testing only, remove it
#include <libv/ui/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
//#include <libv/ui/shader/shader_font.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/ui/shader/shader_image.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
//#include <libv/ui/shader/shader_quad.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/utility/generic_path.hpp>
// std
//#include <sstream>
// pro
#include <vm4_viewer/log.hpp>

//#include <vm4_viewer/version.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

QuickFilePicker::QuickFilePicker() :
	PanelLine(libv::ui::UnnamedTag, "QuickFilePicker") { }

QuickFilePicker::QuickFilePicker(std::string name) :
	PanelLine(std::move(name)) { }

QuickFilePicker::QuickFilePicker(libv::ui::UnnamedTag_t, const std::string_view type) :
	PanelLine(libv::ui::UnnamedTag, type) { }

QuickFilePicker::~QuickFilePicker() { }

void QuickFilePicker::key(libv::input::Key key) {
	if (!isAttached())
		return;

	if (key == libv::input::Key::Up) {
		select--;
		log_app.info("Select {}", select);
		update_filelist();
	}
	if (key == libv::input::Key::Down) {
		select++;
		log_app.info("Select {}", select);
		update_filelist();
	}
}

void QuickFilePicker::doAttach() {
	set(properties.alignHorizontal, libv::ui::AlignHorizontal::Left);
	set(properties.alignVertical, libv::ui::AlignVertical::Top);
	set(properties.orientation, libv::ui::Orientation::TOP_TO_BOTTOM);

	{
		auto style = context().style("vm4pv.file_list");
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
		style->set("font_size", libv::ui::FontSize{11});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.file_list.entry");
		style->set("font_size", libv::ui::FontSize{11});
	}

	{
		auto style = context().style("vm4pv.file_list.entry.selected");
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 100%, 100%)"));
	}

	{
		auto style = context().style("vm4pv.file_list.entry.vm");
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(120, 22%, 90%, 100%)"));
	}

	{
		auto style = context().style("vm4pv.file_list.entry.importable");
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 22%, 90%, 100%)"));
	}

	{
		auto style = context().style("vm4pv.file_list.entry.other");
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 50%, 100%)"));
	}

	{
		auto style = context().style("vm4pv.info.slim_border");
		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("image_shader", context().shader<libv::ui::ShaderImage>());
		style->set("image", context().texture2D("slate_line_edge.png"));
		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
	}

	// -------------------------------------------------------------------------------------------------

	update_filelist();
}

void QuickFilePicker::update_filelist() {
	clear();

	{
		const auto panel_border = std::make_shared<libv::ui::PanelFull>("layer");
		{
			const auto temp = std::make_shared<libv::ui::Quad>("bg-quad");
//			temp->style(context().style("vm4pv"));
			temp->set(temp->properties.color, libv::parse::parse_color_or_throw("hsva(0, 0%, 20%, 35%)"));
			panel_border->add(temp);
		} {
			const auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_border->add(temp);
		} {
			const auto panel_files = std::make_shared<libv::ui::PanelLine>("list");
			panel_files->style(context().style("vm4pv.file_list"));

			path = "res/model";
			boost::container::flat_set<std::filesystem::path> exts_vm = {".vm3", ".vm4", ".vm5"};
			boost::container::flat_set<std::filesystem::path> exts_importable = {".dae"};

			int i = 0;
			for(auto& p : std::filesystem::recursive_directory_iterator(path)) {
				if (not p.is_regular_file())
					continue;

				auto ext = p.path().extension();
				auto entry = std::make_shared<libv::ui::Label>("entry");

				if (select == i++)
					entry->style(context().style("vm4pv.file_list.entry.selected"));
				else if (exts_vm.contains(ext))
					entry->style(context().style("vm4pv.file_list.entry.vm"));
				else if (exts_importable.contains(ext))
					entry->style(context().style("vm4pv.file_list.entry.importable"));
				else
					entry->style(context().style("vm4pv.file_list.entry.other"));

				entry->setText(libv::generic_path(p));
				panel_files->add(entry);
			}

			panel_border->add(panel_files);
		}
		auto& child = add(panel_border);
		child.ptr->set(child.properties.size, libv::ui::parse_size_or_throw("1rD, D"));
	}

	{
		const auto label_help = std::make_shared<libv::ui::Label>("help");
		label_help->style(context().style("vm4pv.file_list"));
		label_help->setText(
				"[Up/Down] Navigate  [Enter] Open  [A-Z] Search\n"
				"? Wildcard matching a single character\n"
				"* Wildcard matching single level any file or directory name\n"
				"** Wildcard matching any recursive file or directory name"
		);
		auto& child = add(label_help);
		child.ptr->set(child.properties.size, libv::ui::parse_size_or_throw("1rD, D"));
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
