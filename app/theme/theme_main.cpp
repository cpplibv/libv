// Project: libv, File: app/theme/theme_main.cpp, Author: Császár Mátyás [Vader]

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
#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/panel_grid.hpp>
//#include <libv/ui/component/quad.hpp>
#include <libv/ui/component/scroll_bar.hpp>
//#include <libv/ui/component/scroll_pane.hpp>
//#include <libv/ui/component/stretch.hpp>

// pro
#include <theme/canvas.hpp>
#include <theme/effect.hpp>
#include <theme/engine.hpp>


// -------------------------------------------------------------------------------------------------

class UIThemeGenFrame : public libv::Frame {
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
	UIThemeGenFrame() :
		Frame("Gen UI Theme", 128+1024+30, 1024+20),
		lua_engine("app/theme/theme_slate.lua", [this](auto&&... v) { canvas.update_texture(std::forward<decltype(v)>(v)...); }) {

		setPosition(FramePosition::center_current_monitor);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(OpenGLSamples{4});
		ui.attach(*this);

		//

		button0.text("Mode: Pulse");
		button0.padding({5, 2, 5, 2});
		button0.size(libv::ui::parse_size_or_throw("D, 22px"));
		button0.event().submit([this](libv::ui::Button& component) {
			canvas.pulse = !canvas.pulse;
			component.text(canvas.pulse ? "Mode: Pulse" : "Mode: Steady");
		});

		//

		panel_line.add(button0);
		panel_line.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

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

		init();
	}

public:
	struct VarControl {
		libv::ui::PanelLine line;

		libv::ui::Label lbl_name;
		libv::ui::ScrollBar slider;
		libv::ui::Label lbl_value;
	};
	libv::ui::PanelLine var_panel;
	std::unordered_map<std::string, VarControl> var_controls;

	void init() {
		var_panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
		var_panel.size({libv::ui::pixel(400), libv::ui::ratio()});
		panel_line.add(var_panel);

		lua_engine.on_dynamic_var([this](auto list) {
			ui.execute_in_ui_loop([this, list = std::move(list)]() {
				this->update_dynamic_var_controls(list);
			});
		});
	}

	void update_dynamic_var_controls(const std::vector<app::DynamicVar>& vars) {
		const auto full_init = var_controls.empty();

		for (const auto& var : vars) {
			auto& control = var_controls[var.name];

			if (var.state == app::DynamicVar::State::remove) {
				control.line.markRemove();
				var_controls.erase(var.name);
				continue;

			} else if (var.state == app::DynamicVar::State::create || full_init) {
				control.line.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
				control.line.size({libv::ui::ratio(), libv::ui::pixel(20)});
				var_panel.add(control.line);

				control.slider.bar_color({1, 0, 0, 1});
				control.slider.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
				control.slider.value_max(var.high);
				control.slider.value_min(var.low);
				control.slider.value_step(var.step);
				control.slider.value_range(var.step != 0.0 ? var.step : 1.0);
				control.slider.value(var.value);
				control.slider.size({libv::ui::ratio(), libv::ui::ratio()});
				control.line.add(control.slider);

				control.lbl_value.text(fmt::format("{:6.2f}", var.value));
				control.lbl_value.size({libv::ui::dynamic(), libv::ui::ratio()});
				control.line.add(control.lbl_value);

				control.lbl_name.text(fmt::format(" {:<16}", var.name));
				control.lbl_name.size({libv::ui::dynamic(), libv::ui::ratio()});
				control.line.add(control.lbl_name);

				control.slider.event().change([this, lbl = control.lbl_value, name = var.name](const libv::ui::ScrollBar& slider) mutable {
					lbl.text(fmt::format("{:6.2f}", slider.value()));
					lua_engine.set_dynamic_var(name, slider.value());
				});

			} else {
				control.slider.value_max(var.high);
				control.slider.value_min(var.low);
				control.slider.value_step(var.step);
				control.slider.value_range(var.step != 0.0 ? var.step : 1.0);
			}
		}
	}

	~UIThemeGenFrame() {
		disconnectAll();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main(int, char**) {
	// For CLion console links
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
//	libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);
	std::cout << libv::logger_stream;

	UIThemeGenFrame frame;
	frame.show();
	frame.join();

	return EXIT_SUCCESS;
}
