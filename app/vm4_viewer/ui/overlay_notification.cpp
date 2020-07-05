// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/ui/overlay_notification.hpp>
// pro
#include <vm4_viewer/log.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

OverlayNotification::OverlayNotification(libv::ui::CoreComponent& parent) :
	PanelLine(parent, libv::ui::GenerateName, "OverlayNotification") { }

OverlayNotification::OverlayNotification(libv::ui::CoreComponent& parent, std::string name) :
	PanelLine(parent, std::move(name)) { }

OverlayNotification::OverlayNotification(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type) :
	PanelLine(parent, libv::ui::GenerateName, type) { }

OverlayNotification::~OverlayNotification() { }

// -------------------------------------------------------------------------------------------------

void OverlayNotification::info(std::string text, const libv::ui::time_duration duration) {
	log_app.info("{} {}s", text, duration.count());
	// TODO P4: UI Report info, no just log
}

void OverlayNotification::warn(std::string text, const libv::ui::time_duration duration) {
	log_app.warn("{} {}s", text, duration.count());
	// TODO P4: UI Report warn, no just log
}

void OverlayNotification::error(std::string text, const libv::ui::time_duration duration) {
	log_app.error("{} {}s", text, duration.count());
	// TODO P4: UI Report error, no just log
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
