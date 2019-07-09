// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/layout_line.hpp>
#include <libv/ui/component/panel.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component_static.hpp>
// pro
#include <vm4_viewer/scene.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//struct Label : Component {
//	AlignHorizontal align;
//	Font2D_view font;
//	Color font_color;
//	FontSize font_size;
//
//	template <typename Archive>
//	constexpr void properties(Archive& ar) {
//		Component::properties(ar);
//
//		// <<< P4: default font should not be null
//		ar(font, "font", "Font file path", nullptr);
//		ar(font_color, "font_color", "Font color", Color{1, 1, 1, 1});
//		ar(font_size, "font_size", "Font size in pixel", FontSize{14});
//		ar(align, "align", "Text vertical alignment", AlignHorizontal::Left);
//	}
//};
//} // namespace PS
//
//using PropertySetLabel = PropertySet<PS::Label>;


//answer: where to store property size
//		- fix member in ComponentBase (current)
//			pro: everyone has size
//			pro: fast access
//			con: does everyone really has it? default layout wont even care, in grid layout too shits on it
//			con: non-uniform property
//answer: where to store properties that are related to the parent's layout
//		- fix member in "Dynamic" Component
//			pro: flexible
//			con: no access
//		- as a member paired with the component in the container
//			pro: EVERYTHING
//			pro: container is the only one that could handle them anyway
//			pro: even faster access
//			con: how can container access the style of the component during insertion
//

class PanelViewer : public libv::ui::ComponentStatic<PanelViewer> {
	friend class libv::ui::ComponentStaticAccess;

private:
	libv::ui::LayoutLine layout; // <<< P4: Into properity layout you go

//	libv::ui::Panel panel_left;
//	libv::ui::Panel panel_help;
//
////	libv::ui::Label label_info;
//	libv::ui::Label label_help;

	app::Scene scene;

private:
//	libv::ui::Panel panel_left;

	libv::ui::PanelObserver panel_info;
	libv::ui::Label label_name;
	libv::ui::Label label_file;
	libv::ui::Label label_info;
	std::vector<libv::ui::Label> label_lod;
	std::vector<libv::ui::Label> label_material;
//	std::vector<libv::ui::Label> label_node;
//	std::vector<libv::ui::Label> label_mesh;
//	std::vector<libv::ui::Label> label_animation;
//	std::vector<libv::ui::Label> label_animationChannel;

//	libv::ui::Panel panel_outline;

	template <typename T>
	void ui_access(T&& access) {
		(void) access;

//		access(layout);
//		access(panel_info);
	}

public:
	PanelViewer();

	void init();
	void update_model();
	void update_camera();
	void update_ui();
	void update_info();

//private:
//	void doAttach(ContextUI& context) override;
//	void doLayoutPass1(const ContextLayoutPass1& environment) override;
//	void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
