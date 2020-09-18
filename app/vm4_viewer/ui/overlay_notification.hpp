// Project: libv.ui, File: app/vm4_viewer/ui/overlay_notification.hpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/panel_line.hpp>
// std
#include <string>
#include <string_view>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

class OverlayNotification : public libv::ui::PanelLine {
public:
	explicit OverlayNotification(libv::ui::CoreComponent& parent);
	OverlayNotification(libv::ui::CoreComponent& parent, std::string name);
	OverlayNotification(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, std::string_view type);
	~OverlayNotification();

public:
	void info(std::string text, libv::ui::time_duration duration);
	void warn(std::string text, libv::ui::time_duration duration);
	void error(std::string text, libv::ui::time_duration duration);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
