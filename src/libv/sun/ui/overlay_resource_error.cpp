// Project: libv.sun, File: src/libv/sun/ui/overlay_resource_error.cpp

// hpp
#include <libv/sun/ui/overlay_resource_error.hpp>
// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/std.h> // For std::error_code
// libv
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/shader_fmt.hpp>
#include <libv/re/resource/shader_load_event.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_fmt.hpp>
#include <libv/re/resource/texture_load_event.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_status_line.hpp>
#include <libv/ui/parse/parse_size.hpp>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

libv::ui::Component overlay_resource_error(bool useStyles) {
	auto line = libv::ui::PanelStatusLine::ns("overlay-shader", "overlay.resource.line");
	if (!useStyles) {
		line.anchor(libv::ui::Anchor::top_left);
		line.size(libv::ui::parse_size_or_throw("D, D"));
		line.align_horizontal(libv::ui::AlignHorizontal::left);
		line.align_vertical(libv::ui::AlignVertical::top);
		line.orientation(libv::ui::Orientation::down);
		line.spacing(8);
		line.margin(8);
	}

	static constexpr auto successDuration = std::chrono::seconds(3);

	// Shader ------------------------------------------------------------------------------------------

	line.event().global.connect<libv::re::ShaderLoadSuccess>([useStyles](libv::ui::PanelStatusLine& psl, const libv::re::ShaderLoadSuccess& e) mutable {
		if (e.shader->loadVersion() == 0)
			return; // Do not display the first load successes on program startup

		auto label = libv::ui::Label::s("overlay.resource.message.success");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.1843f, 0.2314f, 0.1843f, 0.75f}));
			label.font_color({.7235f, 0.9333f, 0.2433f, 1.f});
		}

		label.text(fmt::format("[{:%H:%M:%S}] Successful shader reload: {} v{} ({})", std::chrono::system_clock::now(), *e.shader, e.shader->loadVersion(), e.id));
		psl.add(e.id, label, successDuration);
	});

	line.event().global.connect<libv::re::ShaderLoadFailure>([useStyles](libv::ui::PanelStatusLine& psl, const libv::re::ShaderLoadFailure& e) mutable {
		auto label = libv::ui::Label::s("overlay.resource.message.failure");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.2f, 0.149f, 0.149f, 0.75f}));
			label.font_color({.9333f, 0.8235f, 0.0078f, 1.f});
		}

		const auto usingVersionStr = e.shader->currentVersion() >= 0 ? fmt::format("v{}", e.shader->currentVersion()) : "fallback";
		if (e.includeFailure) {
			std::string message;
			message += fmt::format("[{:%H:%M:%S}] Failed to load shader: {} v{} ({}) using {}", std::chrono::system_clock::now(), *e.shader, e.shader->loadVersion(), e.id, usingVersionStr);
			message += fmt::format("\nFailed to include: '{}'", e.includeFailure->includePath);
			for (const auto& [file, line] : e.includeFailure->includeStack)
				message += fmt::format("\n    Included from: {}:{}", file, line);

			for (const auto& [resourcePrefix, virtualPrefix] : e.includeFailure->unmatchedMappings)
				message += fmt::format("\nMapping '{}' -> '{}' - Unmatched prefix", resourcePrefix, virtualPrefix);
			for (const auto& [virtualFilepath, ec] : e.includeFailure->mappingErrors)
				message += fmt::format("\nVirtual '{}' - {}", virtualFilepath, libv::res::to_message(ec));
			for (const auto& [physicalFilepath, ec] : e.includeFailure->physicalErrors)
				message += fmt::format("\nPhysical '{}' - {} ({})", physicalFilepath, ec.message(), ec);

			label.text(message);

		} else if (e.compileFailure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to compile shader: {} v{} ({}) using {}\n{}", std::chrono::system_clock::now(), *e.shader, e.shader->loadVersion(), e.id, usingVersionStr, e.compileFailure->message));

		} else if (e.linkFailure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to link shader: {} v{} ({}) using {}\n{}", std::chrono::system_clock::now(), *e.shader, e.shader->loadVersion(), e.id, usingVersionStr, e.linkFailure->message));
		}

		psl.add(e.id, label);
	});

	line.event().global.connect<libv::re::ShaderUnload>([](libv::ui::PanelStatusLine& psl, const libv::re::ShaderUnload& e) mutable {
		psl.remove(e.id);
	});

	// Texture -----------------------------------------------------------------------------------------

	line.event().global.connect<libv::re::TextureLoadSuccess>([useStyles](libv::ui::PanelStatusLine& psl, const libv::re::TextureLoadSuccess& e) mutable {
		if (e.texture->loadVersion() == 0)
			return; // Do not display the first load successes on program startup

		auto label = libv::ui::Label::s("overlay.resource.message.success");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.1843f, 0.2314f, 0.1843f, 0.75f}));
			label.font_color({.7235f, 0.9333f, 0.2433f, 1.f});
		}

		label.text(fmt::format("[{:%H:%M:%S}] Successful texture reload: {} v{} ({})", std::chrono::system_clock::now(), *e.texture, e.texture->loadVersion(), e.id));
		psl.add(e.id, label, successDuration);
	});

	line.event().global.connect<libv::re::TextureLoadFailure>([useStyles](libv::ui::PanelStatusLine& psl, const libv::re::TextureLoadFailure& e) mutable {
		auto label = libv::ui::Label::s("overlay.resource.message.failure");
		if (!useStyles) {
			label.size(libv::ui::parse_size_or_throw("1rD, D"));
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.padding(8, 4);
			label.background(libv::ui::Background::color({0.2f, 0.149f, 0.149f, 0.75f}));
			label.font_color({.9333f, 0.8235f, 0.0078f, 1.f});
		}

		const auto usingVersionStr = e.texture->currentVersion() >= 0 ? fmt::format("v{}", e.texture->currentVersion()) : "fallback";
		if (e.ioFailure) {
			std::string message;
			message += fmt::format("[{:%H:%M:%S}] Failed to load texture: {} v{} ({}) using {}", std::chrono::system_clock::now(), *e.texture, e.texture->loadVersion(), e.id, usingVersionStr);

			for (const auto& [resourcePrefix, virtualPrefix] : e.ioFailure->unmatchedMappings)
				message += fmt::format("\nMapping '{}' -> '{}' - Unmatched prefix", resourcePrefix, virtualPrefix);
			for (const auto& [virtualFilepath, ec] : e.ioFailure->mappingErrors)
				message += fmt::format("\nVirtual '{}' - {}", virtualFilepath, libv::res::to_message(ec));
			for (const auto& [physicalFilepath, ec] : e.ioFailure->physicalErrors)
				message += fmt::format("\nPhysical '{}' - {} ({})", physicalFilepath, ec.message(), ec);

			label.text(message);

		} else if (e.loadFailure) {
			label.text(fmt::format("[{:%H:%M:%S}] Failed to load texture: {} v{} ({}) using {}\n"
					"    {}", std::chrono::system_clock::now(), *e.texture, e.texture->loadVersion(), e.id, usingVersionStr, *e.loadFailure));
		}

		psl.add(e.id, label);
	});

	line.event().global.connect<libv::re::TextureUnload>([](libv::ui::PanelStatusLine& psl, const libv::re::TextureUnload& e) mutable {
		psl.remove(e.id);
	});

	return line;
}

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace libv
