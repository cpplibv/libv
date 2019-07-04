// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/layout_line.hpp>
#include <libv/ui/component/panel.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component_static.hpp>


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


class PanelViewer : public libv::ui::ComponentStatic<PanelViewer> {
	friend class libv::ui::ComponentStaticAccess;

private:
//	libv::ui::LayoutLine layout; // <<< P4: Into properity layout you go
//
//	libv::ui::Panel panel_left;
//	libv::ui::Panel panel_help;
//
//	libv::ui::Label label_info;
//	libv::ui::Label label_help;

private:
	template <typename T>
	void ui_access(T&& access) {
		(void) access;

//		access(layout);
//		access(panel_left);
//		access(panel_help);
//		access(label_info);
//		access(label_help);
	}

public:
	PanelViewer();

//private:
//	void doAttach(ContextUI& context) override;
//	void doLayoutPass1(const ContextLayoutPass1& environment) override;
//	void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
