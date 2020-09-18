// Project: libv, File: app/vm4_viewer/frame.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <vm4_viewer/frame.hpp>
// ext
#include <GL/glew.h>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/viewer_ui.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void VM4ViewerFrame::create() {
	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK)
		log_app.error("Failed to initialize glew: {}", glewGetErrorString(err));

	log_app.debug("GL Vendor     {}", glGetString(GL_VENDOR));
	log_app.debug("GL Renderer   {}", glGetString(GL_RENDERER));
	log_app.debug("GL Version    {}", glGetString(GL_VERSION));

	remote.create();
	remote.enableDebug();
}

void VM4ViewerFrame::render() {
	auto gl = remote.queue();

	gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
	gl.clearColor();
	gl.clearDepth();

	ui.update(gl);

	remote.queue(std::move(gl));
	remote.execute();
}

void VM4ViewerFrame::destroy() {
	auto gl = remote.queue();
	ui.destroy(gl);
	remote.queue(std::move(gl));
	remote.execute();

	remote.destroy();
}

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
	onContextCreate.output([&](const libv::frame::EventContextCreate&) {
		create();
	});
	onContextUpdate.output([&](const libv::frame::EventContextUpdate&) {
		render();
	});
	onContextDestroy.output([&](const libv::frame::EventContextDestroy&) {
		destroy();
	});
}

VM4ViewerFrame::~VM4ViewerFrame() {
	disconnectAll();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
