// Project: libv, File: app/space/view/overlay_shader_error.cpp

// hpp
#include <space/view/overlay_shader_error.hpp>
// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/os.h> // For std::error_code
// libv
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_load_event.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_status_line.hpp>
#include <libv/ui/parse/parse_size.hpp>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

libv::ui::Component overlay_shader_error(bool useStyles) {
	auto line = libv::ui::PanelStatusLine::ns("overlay-shader", "overlay.shader.line");
	if (!useStyles) {
		line.anchor(libv::ui::Anchor::top_left);
		line.size(libv::ui::parse_size_or_throw("D, D"));
		line.align_horizontal(libv::ui::AlignHorizontal::left);
		line.align_vertical(libv::ui::AlignVertical::top);
		line.orientation(libv::ui::Orientation::down);
		line.spacing(8);
		line.margin(8);
	}

	line.event().global.connect<libv::rev::ShaderLoadSuccess>([useStyles](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadSuccess& e) mutable {
		if (e.shader.load_version() == 0)
			return; // Do not display the first load successes on program startup

		auto label = libv::ui::Label::s("overlay.shader.message.success");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.1843f, 0.2314f, 0.1843f, 0.75f}));
			label.font_color({.7235f, 0.9333f, 0.2433f, 1.f});
		}

		label.text(fmt::format("[{:%H:%M:%S}] Successful shader reload: {} v{} ({})", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id));
		psl.add(e.id, label, std::chrono::seconds(3));
	});

	line.event().global.connect<libv::rev::ShaderLoadFailure>([useStyles](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadFailure& e) mutable {
		auto label = libv::ui::Label::s("overlay.shader.message.failure");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.2f, 0.149f, 0.149f, 0.75f}));
			label.font_color({.9333f, 0.8235f, 0.0078f, 1.f});
		}

		const auto usingVersionStr = e.shader.current_version() >= 0 ? fmt::format("v{}", e.shader.current_version()) : "fallback";
		if (e.include_failure) {
			std::string message;
			message += fmt::format("[{:%H:%M:%S}] Failed to load shader: {} v{} ({}) using {}\n", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, usingVersionStr);
			message += fmt::format("Failed to include: \"{}\" from file: {} - {}: {}", e.include_failure->include_path, e.include_failure->file_path, e.include_failure->ec, e.include_failure->ec.message());
			for (const auto& [file, line] : e.include_failure->include_stack)
				message += fmt::format("\n    Included from: {}:{}", file, line);
			label.text(message);

		} else if (e.compile_failure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to compile shader: {} v{} ({}) using {}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, usingVersionStr, e.compile_failure->message));

		} else if (e.link_failure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to link shader: {} v{} ({}) using {}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, usingVersionStr, e.link_failure->message));
		}

		psl.add(e.id, label);
	});

	line.event().global.connect<libv::rev::ShaderUnload>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderUnload& e) mutable {
		psl.remove(e.id);
	});

	return line;
}

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace libv
