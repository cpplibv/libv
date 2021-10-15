// Project: libv, File: app/vm4_viewer/frame.cpp

// hpp
#include <vm4_viewer/frame.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/viewer_ui.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

VM4ViewerFrame::VM4ViewerFrame(app::ConfigViewer& config) :
	Frame("VM4 Viewer", config.window_width, config.window_height) {
	setPosition(FramePosition::center_current_monitor);
	setOpenGLProfile(OpenGLProfile::core);
	setOpenGLVersion(3, 3);
	setOpenGLSamples(OpenGLSamples{4});
	ui.attach(*this);
	ui.setSize(config.window_width, config.window_height); // TODO P2: auto load size

	panel = std::make_shared<app::ViewerUI>(ui.root());
	ui.add(panel);

	onKey.output([&](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape && e.action == libv::input::Action::press)
			closeDefault();
	});
	onSize.output([&](const libv::frame::EventSize& e) {
		if (config.save_window_size) {
			config.window_width = e.size.x;
			config.window_height = e.size.y;
		}
	});
}

VM4ViewerFrame::~VM4ViewerFrame() {
	disconnectAll();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
