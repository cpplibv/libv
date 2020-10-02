// Project: libv.ui, File: app/gen_ui_theme/gen_ui_theme.cpp, Author: Császár Mátyás [Vader]

// ext
#include <GL/glew.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/gl/enum.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/log/log.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
//#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/ui.hpp>
// std
#include <iostream>
#include <atomic>
#include <mutex>
#include <string>

//#include <libv/ui/component/image.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/panel_grid.hpp>
//#include <libv/ui/component/quad.hpp>
//#include <libv/ui/component/scroll_bar.hpp>
//#include <libv/ui/component/scroll_pane.hpp>
//#include <libv/ui/component/stretch.hpp>

// pro
#include <gen_ui_theme/canvas.hpp>
#include <gen_ui_theme/effect.hpp>
#include <gen_ui_theme/engine.hpp>


// -------------------------------------------------------------------------------------------------

class GenUIThemeFrame : public libv::Frame {
private:
	libv::ui::UI ui;

private:
	app::Canvas canvas;
	app::Engine lua_engine;

private:
	libv::ui::Button button0;
	libv::ui::PanelLine panel_line;
	libv::ui::CanvasAdaptor canvas_adaptor;

public:
	GenUIThemeFrame() :
		Frame("Gen UI Theme", 128+1024+30, 1024+20),
		lua_engine("app/gen_ui_theme/theme_slate.lua", [this](auto... v) { canvas.update_texture(v...); }) {

		setPosition(FramePosition::center_current_monitor);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(OpenGLSamples{4});
		ui.attach(*this);

		//

		button0.text("Hello World!");
		button0.size(libv::ui::parse_size_or_throw("D, 22px"));
		button0.event().submit([this](libv::ui::Button& component, const libv::ui::EventSubmit& event) {
			(void) event;
			log_app.info("Button pressed {}", component.path());
			canvas.pulse = !canvas.pulse;
		});

		//

		panel_line.add(button0);

		canvas_adaptor.adopt(&canvas);

		ui.add(canvas_adaptor);
		ui.add(panel_line);

		//

		onKey.output([&](const libv::input::EventKey& e) {
			if (e.action == libv::input::Action::release)
				return;

			if (e.keycode == libv::input::Keycode::Escape)
				closeDefault();

			if (e.keycode == libv::input::Keycode::Space)
				canvas.pulse = !canvas.pulse;
		});
	}

	~GenUIThemeFrame() {
		disconnectAll();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main(int, char**) {
	// For CLion console
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	GenUIThemeFrame frame;
	frame.show();
	frame.join();

	return EXIT_SUCCESS;
}
