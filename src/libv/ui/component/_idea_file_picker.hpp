// Project: libv.ui, File: src/libv/ui/component/_idea_file_picker.hpp
//
//// libv
//#include <libv/ui/component/fwd.hpp>
//#include <libv/ui/component/panel_line.hpp>
////#include <libv/ui/component/label.hpp>
////#include <libv/ui/component/panel_anchor.hpp>
////#include <libv/ui/component/panel_full.hpp>
////#include <libv/ui/component_static.hpp>
////#include <libv/ui/layout_line.hpp>
//// std
//#include <filesystem>
////// pro
////#include <vm4_viewer/scene/scene.hpp>
//
//
//// -------------------------------------------------------------------------------------------------
//
//namespace libv {
//namespace ui {
//
//// -------------------------------------------------------------------------------------------------
//
//class QuickFilePicker6 : public libv::ui::PanelLine {
//public:
//	struct EventPick {
//		QuickFilePicker6& component;
//	};
//
//private:
//	std::filesystem::path path;
//	std::string value_;
//
////	std::string filter_pattern;
////
////	struct StyleRule {
////		std::filesystem::path pattern;
//////		std::string pattern;
////		bool show = true;
////		libv::intrusive_ptr<libv::ui::Style> style;
////	};
//
//	bool displayRelativePath = true;
//	bool restrictRelativePath = true;
//	bool listDirectories = true;
//	bool expandDirectories = false;
//
////	enum class SelectionMode {
////		single,
////		multiple,
////		interval,
////	};
//
//	enum class PathDisplayMode {
//		flat,
//		tree,
//	} pathDisplayMode = PathDisplayMode::flat;
//
//	bool followUpdates = false;
////	bool trackFilesystem = false;
//
//	int select = -1;
//
//	libv::ui::InputField search_field;
//	libv::ui::PanelFull list_panel;
//
//public:
//class QuickFilePicker6 : public ComponentAPI<Component, QuickFilePicker6, class CoreQuickFilePicker6, EventHostGeneral> {
//public:
//	using ComponentAPI::ComponentAPI;
//	static constexpr std::string_view component_type = "q-f-pick";
//	static core_ptr create_core(std::string name);
//
//	~QuickFilePicker6();
//
//	void update_filelist();
//
//public:
////	template <typename F>
////	inline void event_pick(libv::observer_ptr<CoreComponent> slot, F&& func) {
////		connect<EventPick>(slot, std::forward<F>(func));
////	}
//
//	[[nodiscard]] inline const std::string& value() const noexcept {
//		return value_;
//	}
//
//private:
//	virtual void doAttach() override;
//	virtual void onKey(const libv::ui::EventKey& event) override;
////	libv::vec3f doLayout1(const ContextLayout1& environment) override;
////	void doRender(Renderer& r) override;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv
