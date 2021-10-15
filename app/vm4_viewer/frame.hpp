// Project: libv, File: app/vm4_viewer/frame.hpp

#pragma once

// libv
#include <libv/frame/frame.hpp>
#include <libv/glr/remote.hpp>
#include <libv/ui/ui.hpp>
// std
#include <memory>
// pro
#include <vm4_viewer/config.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class ViewerUI;

class VM4ViewerFrame : public libv::Frame {
private:
	libv::ui::UI ui;
	std::shared_ptr<app::ViewerUI> panel;

public:
	VM4ViewerFrame(app::ConfigViewer& config);
	~VM4ViewerFrame();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
