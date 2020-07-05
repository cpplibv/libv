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
//	explicit KeyValuePanel(CoreComponent& parent);
//	KeyValuePanel(CoreComponent& parent, std::string name);
//	KeyValuePanel(CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);
//
//public:
//	std::shared_ptr<CoreComponent>& operator()(size_t x, size_t y);
//};

class PropertyPanelGroupProxy {
public:
	template <typename K, typename V>
	void add(K key, V value);
};

class PropertyPanelGroup {
private:
	using key_type = std::shared_ptr<libv::ui::CoreComponent>;
	using value_type = std::shared_ptr<libv::ui::CoreComponent>;

	struct key_value_type {
		key_type key;
		value_type value;
	};

public:
	std::vector<key_value_type> properties;

	void add(key_type, value_type);
};

class PropertyPanel : public libv::ui::PanelLine {
	std::vector<PropertyPanelGroup> groups;

public:
	explicit PropertyPanel(CoreComponent& parent);
	PropertyPanel(CoreComponent& parent, std::string name);
	PropertyPanel(CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);

public:
	PropertyPanelGroupProxy group(const std::string& name, libv::intrusive_ptr<libv::ui::Style>& style);
};

// =================================================================================================

class ViewerInfoPanel : public libv::ui::PanelLine {
private:
//	KeyValuePanel panel_general{*this, "general"};

	std::shared_ptr<libv::ui::Label> label_name;
	std::shared_ptr<libv::ui::Label> label_info;
	std::shared_ptr<libv::ui::Label> label_bounds;
	std::vector<std::shared_ptr<libv::ui::Label>> label_material;

//	libv::ui::PanelObserver panel_info;
//	libv::ui::Label label_name;
//	libv::ui::Label label_file;
//	libv::ui::Label label_info;
//	std::vector<libv::ui::Label> label_lod;
//	std::vector<libv::ui::Label> label_material;
//	std::vector<libv::ui::Label> label_node;
//	std::vector<libv::ui::Label> label_mesh;
//	std::vector<libv::ui::Label> label_animation;
//	std::vector<libv::ui::Label> label_animationChannel;

public:
	explicit ViewerInfoPanel(CoreComponent& parent);
	ViewerInfoPanel(CoreComponent& parent, std::string name);
	ViewerInfoPanel(CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type);

	void update(const libv::vm4::Model& model);

private:
	void doAttach() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
