// Project: libv, File: app/vm4_viewer/main.cpp


// pro
#include <vm4_viewer/frame.hpp>


// -------------------------------------------------------------------------------------------------

// TODO P5: app.vm4_viewer: show model wireframe (GS based wireframe)
// TODO P5: app.vm4_viewer: vm4 LOD support
// TODO P5: app.vm4_viewer: pointless loading bar
// TODO P5: app.vm4_viewer: thumbnail support, and auto generation (async operation)
// TODO P5: app.vm4_viewer: display vm4 node hierarchy
// TODO P5: app.vm4_viewer: show material information
// TODO P5: app.vm4_viewer: show model texture information
// TODO P5: app.vm4_viewer: show model textured
// TODO P5: app.vm4_viewer: vm4 slot/attachment support
// TODO P5: app.vm4_viewer: save model
// TODO P5: app.vm4_viewer: skybox
// TODO P5: app.vm4_viewer: animations
// TODO P5: app.vm4_viewer: lighting
// TODO P5: app.vm4_viewer: multiple light
// TODO P5: app.vm4_viewer: shaders
// TODO P5: app.vm4_viewer: Forward+
// TODO P5: app.vm4_viewer: AA
// TODO P5: app.vm4_viewer: full game render engine (what ?!)
// TODO P5: app.vm4_viewer: billboard LOD texture

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {

	{
		app::VM4ViewerFrame frame{config};
		frame.show();
		frame.join();
	}

	return EXIT_SUCCESS;
}
