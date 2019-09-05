// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

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

class PanelViewer;

class VM4ViewerFrame : public libv::Frame {
private:
	libv::glr::Remote remote;
	libv::ui::UI ui;
	std::shared_ptr<app::PanelViewer> panel;

private:
	void create();
	void render();
	void destroy();

public:
	VM4ViewerFrame(app::ConfigViewer& config);
	~VM4ViewerFrame();
};

// -------------------------------------------------------------------------------------------------

} // namespace app