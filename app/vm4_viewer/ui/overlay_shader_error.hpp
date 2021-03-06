// Project: libv.ui, File: app/vm4_viewer/ui/overlay_shader_error.hpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/ui/component.hpp>
//#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/panel_line.hpp>
// std
#include <map>
#include <string>
#include <string_view>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

class OverlayShaderError : public libv::ui::PanelLine {
//	std::shared_ptr<libv::ui::Label> label_error_header;
//	std::shared_ptr<libv::ui::Label> label_error_message;
//	std::shared_ptr<libv::ui::PanelLine> details;
//
//	std::shared_ptr<libv::ui::Quad> background;
//
//	std::shared_ptr<libv::ui::PanelFloat> main;
//
//public:
//	void foo() {
//		main->add(background);
//		main->add(details);
//
//		details->add(label_error_header);
//		details->add(label_error_message);
//	}

//	std::map<std::string, std::shared_ptr<CoreComponent>> errors;
	std::map<std::string, Component> errors;

public:
	explicit OverlayShaderError(libv::ui::CoreComponent& parent);
	OverlayShaderError(libv::ui::CoreComponent& parent, std::string name);
	OverlayShaderError(libv::ui::CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
	~OverlayShaderError();

public:
	void error_add(std::string name, std::string error);
	void error_remove(const std::string& name);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
