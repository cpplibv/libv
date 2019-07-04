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

	auto gl = remote.queue();
	ui.create(gl);
	remote.queue(std::move(gl));
	remote.render();
}

void VM4ViewerFrame::render() {
	auto gl = remote.queue();

	gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
	gl.clearColor();
	gl.clearDepth();

	ui.update(gl);

	remote.queue(std::move(gl));
	remote.render();
}

void VM4ViewerFrame::destroy() {
	auto gl = remote.queue();
	ui.destroy(gl);
	remote.queue(std::move(gl));
	remote.render();

	remote.destroy();
}

VM4ViewerFrame::VM4ViewerFrame(app::ConfigViewer& config) :
	Frame("VM4 Viewer", config.window_width, config.window_height) {
	setPosition(POSITION_CENTER_CURRENT_MONITOR);
	setOpenGLProfile(OPENGL_PROFILE_CORE);
	setOpenGLVersion(3, 3);
	setOpenGLSamples(4);
	ui.attach(*this);
	ui.setSize(
			static_cast<float>(config.window_width),
			static_cast<float>(config.window_height)); // TODO P4: auto detect size changes

	panel = std::make_shared<app::PanelViewer>();
	ui.add(panel);

	onKey.output([&](const libv::frame::EventKey& e) {
		if (e.action == libv::frame::Action::release)
			return;

		if (e.key == libv::frame::Key::Escape)
			closeDefault();

		switch (e.key) {
		case libv::frame::Key::Q: break;
		case libv::frame::Key::W: break;
		case libv::frame::Key::E: break;
		case libv::frame::Key::A: break;
		case libv::frame::Key::S: break;
		case libv::frame::Key::D: break;
		default: break;
		}
	});
	onChar.output([&](const libv::frame::EventChar& e) {
		(void) e;
	});
	onWindowSize.output([&](const libv::frame::EventWindowSize& e) {
		config.window_width = e.size.x;
		config.window_height = e.size.y;
	});
	onContextInitialization.output([&](const libv::frame::EventContextInitialization&) {
		create();
	});
	onContextRefresh.output([&](const libv::frame::EventContextRefresh&) {
		render();
	});
	onContextTerminate.output([&](const libv::frame::EventContextTerminate&) {
		destroy();
	});
}

VM4ViewerFrame::~VM4ViewerFrame() {
	disconnectAll();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
