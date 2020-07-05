// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

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
	OverlayNotification(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
	~OverlayNotification();

public:
	void info(std::string text, const libv::ui::time_duration duration);
	void warn(std::string text, const libv::ui::time_duration duration);
	void error(std::string text, const libv::ui::time_duration duration);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
