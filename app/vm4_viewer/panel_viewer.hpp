// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/component/panel.hpp>
#include <libv/ui/component/label.hpp>
// std
#include <memory>


namespace app {

// -------------------------------------------------------------------------------------------------

class PanelViewer : public libv::ui::Panel {
private:
	std::shared_ptr<libv::ui::Panel> panel_info = std::make_shared<libv::ui::Panel>();
	std::shared_ptr<libv::ui::Label> label_info = std::make_shared<libv::ui::Label>();

public:
	PanelViewer();

private:
	void create();
	void render();
	void destroy();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
