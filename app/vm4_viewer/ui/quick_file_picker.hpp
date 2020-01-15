// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component_static.hpp>
//#include <libv/ui/layout_line.hpp>
// std
#include <filesystem>
//// pro
//#include <vm4_viewer/scene/scene.hpp>


// -------------------------------------------------------------------------------------------------

namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

class QuickFilePicker : public libv::ui::PanelLine {
public:
	struct EventPick {
		QuickFilePicker& component;
	};

private:
	std::filesystem::path path;
	std::string value_;

//	std::string filter_pattern;
//
//	struct StyleRule {
//		std::filesystem::path pattern;
////		std::string pattern;
//		bool show = true;
//		libv::intrusive_ptr<libv::ui::Style> style;
//	};

	bool displayRelativePath = true;
	bool restrictRelativePath = true;
	bool listDirectories = true;
	bool expandDirectories = false;

//	enum class SelectionMode {
//		single,
//		multiple,
//		interval,
//	};

	enum class PathDisplayMode {
		flat,
		tree,
	} pathDisplayMode = PathDisplayMode::flat;

	bool followUpdates = false;

	int select = -1;

	std::shared_ptr<libv::ui::InputField> search_field;
	std::shared_ptr<libv::ui::PanelFull> list_panel;

public:
	explicit QuickFilePicker(libv::ui::BaseComponent& parent);
	QuickFilePicker(libv::ui::BaseComponent& parent, std::string name);
	QuickFilePicker(libv::ui::BaseComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
	~QuickFilePicker();

	void update_filelist();

public:
	template <typename F>
	inline void event_pick(libv::observer_ptr<BaseComponent> slot, F&& func) {
		connect<EventPick>(slot, std::forward<F>(func));
	}

	[[nodiscard]] inline const std::string& value() const noexcept {
		return value_;
	}

private:
	virtual void doAttach() override;
	virtual void onKey(const libv::ui::EventKey& event) override;
//	void doLayout1(const ContextLayout1& environment) override;
//	void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
