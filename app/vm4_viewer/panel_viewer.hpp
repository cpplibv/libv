// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component_static.hpp>
//#include <libv/ui/layout_line.hpp>
#include <libv/input/input.hpp>
// std
#include <filesystem>
// pro
#include <vm4_viewer/scene.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

namespace ui {

class QuickFilePicker;

} // namespace ui

class PanelViewer : public libv::ui::PanelFloat {
private:
	std::shared_ptr<libv::ui::Button> button_close;
	std::shared_ptr<libv::ui::Label> label_help;
	std::shared_ptr<libv::ui::Label> label_version;
	std::shared_ptr<libv::ui::PanelLine> panel_left;
	std::shared_ptr<app::ui::QuickFilePicker> picker_files;

//	std::filesystem::path path;
//	app::Scene scene;

//	int select = -1;
private:
//	libv::ui::Panel panel_left;
//
//	libv::ui::PanelObserver panel_info;
//	libv::ui::Label label_name;
//	libv::ui::Label label_file;
//	libv::ui::Label label_info;
//	std::vector<libv::ui::Label> label_lod;
//	std::vector<libv::ui::Label> label_material;
//	std::vector<libv::ui::Label> label_node;
//	std::vector<libv::ui::Label> label_mesh;
//	std::vector<libv::ui::Label> label_animation;
//	std::vector<libv::ui::Label> label_animationChannel;

//	libv::ui::Panel panel_outline;

//private:
//	friend class libv::ui::ComponentStaticAccess;
//	template <typename T>
//	void ui_access(T&& access) {
////		(void) access;
////		access(layout);
//		access(main);
//	}

public:
	explicit PanelViewer(BaseComponent& parent);

	void init();
//	void update_filelist();
	void update_model();
	void update_camera();
	void update_ui();
	void update_info();

	void key(libv::input::Key key);

private:
	void doAttach() override;
//	void doLayout1(const ContextLayout1& environment) override;
//	void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
