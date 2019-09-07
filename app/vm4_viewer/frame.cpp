// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/frame.hpp>
// ext
#include <GL/glew.h>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/panel_viewer.hpp>


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

//	auto gl = remote.queue();
//	ui.create(gl);
//	remote.queue(std::move(gl));
//	remote.execute();
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
	ui.setSize(
			static_cast<float>(config.window_width),
			static_cast<float>(config.window_height)); // TODO P4: auto detect size changes

	panel = std::make_shared<app::PanelViewer>();
	ui.add(panel);

	onKey.output([&](const libv::frame::EventKey& e) {
		if (e.action == libv::input::Action::release)
			return;

		if (e.key == libv::input::Key::Escape)
			closeDefault();

		switch (e.key) {
		case libv::input::Key::Q: break;
		case libv::input::Key::W: break;
		case libv::input::Key::E: break;
		case libv::input::Key::A: break;
		case libv::input::Key::S: break;
		case libv::input::Key::D: break;
		default: break;
		}
	});
	onChar.output([&](const libv::frame::EventChar& e) {
		(void) e;
	});
	onKey.output([&](const libv::frame::EventKey& e) {
		(void) e;

		if (e.action == libv::input::Action::release)
			panel->key(e.key);
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
