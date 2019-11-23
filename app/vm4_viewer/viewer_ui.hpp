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

class ViewerInfoPanel;

class ViewerUI : public libv::ui::PanelFloat {
private:
	std::shared_ptr<libv::ui::Label> label_help;
	std::shared_ptr<libv::ui::Label> label_version;
	std::shared_ptr<app::ui::QuickFilePicker> picker_files;
	std::shared_ptr<ViewerInfoPanel> info_panel;

	app::Scene scene;

public:
	explicit ViewerUI(BaseComponent& parent);

	void init();
	void update_model();
	void update_camera();
	void update_ui();
	void load(const std::string& path);
	void update_info();

private:
	void doAttach() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
