// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/vm4/fwd.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//class KeyValuePanel : public libv::ui::PanelLine {
//private:
//	libv::ui::LabelImage title{*this, "title"};
//	libv::ui::PanelTable table{*this, "table"};
//
//public:
//	explicit KeyValuePanel(BaseComponent& parent);
//	KeyValuePanel(BaseComponent& parent, std::string name);
//	KeyValuePanel(BaseComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
//
//public:
//	std::shared_ptr<BaseComponent>& operator()(size_t x, size_t y);
//};

class ViewerInfoPanel : public libv::ui::PanelLine {
private:
//	KeyValuePanel panel_general{*this, "general"};

//	libv::ui::PanelObserver panel_info;
//	libv::ui::Label label_name{*this};
//	libv::ui::Label label_file;
//	libv::ui::Label label_info;
//	std::vector<libv::ui::Label> label_lod;
//	std::vector<libv::ui::Label> label_material;
//	std::vector<libv::ui::Label> label_node;
//	std::vector<libv::ui::Label> label_mesh;
//	std::vector<libv::ui::Label> label_animation;
//	std::vector<libv::ui::Label> label_animationChannel;

public:
	explicit ViewerInfoPanel(BaseComponent& parent);
	ViewerInfoPanel(BaseComponent& parent, std::string name);
	ViewerInfoPanel(BaseComponent& parent, libv::ui::GenerateName_t, const std::string_view type);

	void update(const libv::vm4::Model& model);

private:
	void doAttach() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
