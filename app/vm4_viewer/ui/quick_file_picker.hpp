// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component_static.hpp>
//#include <libv/ui/layout_line.hpp>
#include <libv/input/inputs.hpp>
// std
#include <filesystem>
// pro
#include <vm4_viewer/scene.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

class QuickFilePicker : public libv::ui::PanelLine {
private:
	std::filesystem::path path;

	struct StyleRule {
		std::filesystem::path pattern;
		bool show = true;
		libv::intrusive_ptr<libv::ui::Style> style;
	};

	bool displayRelativePath = true;
	bool restrictRelativePath = true;
	bool listDirectories = true;
	bool expandDirectories = false;

	enum class PathDisplayMode {
		flat,
		tree,
	} pathDisplayMode = PathDisplayMode::flat;

	bool followUpdates = false;

	int select = -1;

public:
	QuickFilePicker();
	QuickFilePicker(std::string name);
	QuickFilePicker(libv::ui::UnnamedTag, const std::string_view type);
	~QuickFilePicker();

	void update_filelist();
	void key(libv::input::Key key);

private:
	void doAttach() override;
//	void doLayout1(const ContextLayout1& environment) override;
//	void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
