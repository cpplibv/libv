// Project: libv, File: app/star/game/game_client_frame.cpp

// hpp
#include <star/game/game_client_frame.hpp>
// pro
#include <star/game/view/icon_set.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

GameClientFrame::GameClientFrame() :
//	Frame("Star", 1280, 800) {
	Frame("Star", 1600, 900) {
//	Frame("Star", 1680, 945) {
//	Frame("Star", 1680, 1050) {
//	Frame("Star", 1920, 1080) {

	setPosition(libv::Frame::FramePosition::center_current_monitor);
//	setPosition(libv::Frame::FramePosition::os_default);
	setOpenGLProfile(libv::Frame::OpenGLProfile::core);
	setOpenGLVersion(4, 5);
	setOpenGLSamples(libv::Frame::OpenGLSamples{0});
	setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
	setIcon(icon_set_iris_cyan());

	onKey.output([this](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			closeForce();
	});

	// TODO P1: Fix bug where setDisplayMode(DisplayMode::fullscreen_windowed) fails with assert (incorrect matrix)
	//setDisplayMode(DisplayMode::fullscreen_windowed);

//		onSize.output([&](const libv::frame::EventSize& e) {
//			if (config.save_window_size) {
//				config.window_width = e.size.x;
//				config.window_height = e.size.y;
//			}
//		});
}

GameClientFrame::~GameClientFrame() {
	disconnectAll();
}

// -------------------------------------------------------------------------------------------------

} // namespace star


