// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/ui/overlay_shader_error.hpp>
// libv
#include <libv/ui/component/label.hpp>
// pro
#include <vm4_viewer/log.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

OverlayShaderError::OverlayShaderError(libv::ui::CoreComponent& parent) :
	PanelLine(parent, libv::ui::GenerateName, "OverlayShaderError") { }

OverlayShaderError::OverlayShaderError(libv::ui::CoreComponent& parent, std::string name) :
	PanelLine(parent, std::move(name)) { }

OverlayShaderError::OverlayShaderError(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type) :
	PanelLine(parent, libv::ui::GenerateName, type) { }

OverlayShaderError::~OverlayShaderError() { }

// -------------------------------------------------------------------------------------------------

void OverlayShaderError::error_add(std::string name, std::string error) {
	error_remove(name);

	auto tmp = std::make_shared<libv::ui::Label>(*this);
	tmp->style(context().style("overlay.shader_error.text"));
	tmp->text(libv::concat("Failed to load shader: ", name, "\n", std::move(error)));
	add(tmp);

	errors[std::move(name)] = std::move(tmp);
}

void OverlayShaderError::error_remove(const std::string& name) {
	const auto it = errors.find(name);

	if (it == errors.end())
		return;

	remove(it->second);
	errors.erase(it);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
