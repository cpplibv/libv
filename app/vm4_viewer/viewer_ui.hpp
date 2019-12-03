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
#include <vm4_viewer/scene/scene.hpp>


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

private:
//	float keyboard_spin_speed = libv::to_rad(60.f); // 60°/s
//	float mouse_spin_speed = libv::to_rad(60.f) * 0.0025f; // 60°/400px
//	float keyboard_move_speed = zoom * 0.5f + 1.0f;
//	float keyboard_zoom_scale = zoom * 0.2f + 0.1f;
//	float mouse_move_speed = zoom * 0.0006f + 0.05f;
//	float mouse_scroll_scale = zoom * 0.08f + 0.1f;

	app::Scene scene;

public:
	explicit ViewerUI(BaseComponent& parent);

	void update(libv::ui::time_duration elapsed_time);
	void load(const std::string& path);

private:
	virtual bool onKey(const libv::input::EventKey& event) override;
	virtual void onFocus(const libv::ui::EventFocus& event) override;
	virtual bool onMouseButton(const libv::ui::EventMouseButton& event) override;
	virtual bool onMouseMovement(const libv::ui::EventMouseMovement& event) override;
	virtual bool onMouseScroll(const libv::ui::EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doRender(libv::ui::ContextRender& ctx) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
