// Project: libv, File: app/space/view/make_shader_error_overlay.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/view/make_shader_error_overlay.hpp>
// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
// libv
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_load_event.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_status_line.hpp>
#include <libv/ui/parse/parse_size.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

libv::ui::Component make_shader_error_overlay() {
	libv::ui::PanelStatusLine shader_errors;

	shader_errors.align_horizontal(libv::ui::AlignHorizontal::left);
	shader_errors.align_vertical(libv::ui::AlignVertical::bottom);
	shader_errors.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
//		shader_errors.size(libv::ui::parse_size_or_throw("1r, 1r"));
	shader_errors.spacing(10);

	shader_errors.event().global.connect<libv::rev::ShaderLoadSuccess>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadSuccess& e) mutable {
		if (e.shader.load_version() == 0)
			return; // Do not display the first load successes on program startup

		libv::ui::Label label;
		label.align_horizontal(libv::ui::AlignHorizontal::left);
		label.align_vertical(libv::ui::AlignVertical::bottom);
		label.size(libv::ui::parse_size_or_throw("1r, D"));
		label.font_color({.7235f, 0.9333f, 0.2433f, 1.f});

		label.text(fmt::format("[{:%H:%M:%S}] Successful shader reload: {} v{} ({})", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id));
		psl.add(e.id, label, std::chrono::seconds(3));
	});

	shader_errors.event().global.connect<libv::rev::ShaderLoadFailure>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadFailure& e) mutable {
		libv::ui::Label label;
		label.align_horizontal(libv::ui::AlignHorizontal::left);
		label.align_vertical(libv::ui::AlignVertical::bottom);
		label.size(libv::ui::parse_size_or_throw("1r, D"));
		label.font_color({.9333f, 0.8235f, 0.0078f, 1.f}); // Warning yellow

		if (e.include_failure) {
			std::string message;
			message += fmt::format("[{:%H:%M:%S}] Failed to load shader: {} v{} ({}) using v{}\n", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version());
			message += fmt::format("Failed to include: \"{}\" from file: \"{}\" - {}: {}", e.include_failure->include_path, e.include_failure->file_path, e.include_failure->ec, e.include_failure->ec.message());
			for (const auto& [file, line] : e.include_failure->include_stack)
				message += fmt::format("\n    Included from: {}:{}", file, line);
			label.text(message);

		} else if (e.compile_failure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to compile shader: {} v{} ({}) using v{}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version(), e.compile_failure->message));

		} else if (e.link_failure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to link shader: {} v{} ({}) using v{}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version(), e.link_failure->message));
		}

		psl.add(e.id, label);
	});

	shader_errors.event().global.connect<libv::rev::ShaderUnload>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderUnload& e) mutable {
		psl.remove(e.id);
	});

	return shader_errors;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
