// Project: libv, File: app/vm4_viewer/viewer_info_panel.cpp

// hpp
#include <vm4_viewer/viewer_info_panel.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/label_image.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/vm4/model.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/context/context_ui.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

ViewerInfoPanel::ViewerInfoPanel(CoreComponent& parent) :
	libv::ui::PanelLine(parent, libv::ui::GenerateName, "ViewerInfoPanel") { }

ViewerInfoPanel::ViewerInfoPanel(CoreComponent& parent, std::string name) :
	libv::ui::PanelLine(parent, std::move(name)) { }

ViewerInfoPanel::ViewerInfoPanel(CoreComponent& parent, libv::ui::GenerateName_t, const std::string_view type) :
	libv::ui::PanelLine(parent, libv::ui::GenerateName, type) { }

void ViewerInfoPanel::doAttach() {
	set(property.align_horizontal, libv::ui::AlignHorizontal::left);
	set(property.align_vertical, libv::ui::AlignVertical::top);
	set(property.orientation, libv::ui::Orientation::down);

//	{
//		auto style = context().style("vm4pv.label_info");
//		style->set("align", libv::ui::AlignHorizontal::right);
//		style->set("font", context().font("consola.ttf"));
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
//		style->set("font_size", libv::ui::FontSize{10});
//		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
//		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}

	{
		auto style = context().style("vm4pv.info.h1");
		style->set("align", libv::ui::AlignHorizontal::right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.info.value");
		style->set("align", libv::ui::AlignHorizontal::left);
//		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("size", libv::ui::parse_size_or_throw("100%, d"));
	}

	{
		auto style = context().style("vm4pv.info.bar");
		style->set("align", libv::ui::AlignHorizontal::center);
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("bg_image", context().texture2D("separator_bar_256x16.png"));
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(170, 2%, 90%, 100%)"));
		style->set("font_size", libv::ui::FontSize{11});
		style->set("size", libv::ui::parse_size_or_throw("1r D, D"));
	}

	// -------------------------------------------------------------------------------------------------

//	{
//		label_version = std::make_shared<libv::ui::Label>(*this, "version");
//		label_version->style(context().style("vm4pv.label_version"));
//		label_version->text(app::full_version);
//		add(label_version);
//	}
	{
		auto tmp = std::make_shared<libv::ui::LabelImage>(*this);
		tmp->style(context().style("vm4pv.info.bar"));
		tmp->text("General");
		add(std::move(tmp));
	}
	{
		label_name = std::make_shared<libv::ui::Label>(*this, "name");
		label_name->style(context().style("vm4pv.info.value"));
		add(label_name);
	}

	// -------------------------------------------------------------------------------------------------

	{
		auto tmp = std::make_shared<libv::ui::LabelImage>(*this);
		tmp->style(context().style("vm4pv.info.bar"));
		tmp->text("Bounds");
		add(std::move(tmp));
	}
	{
		label_bounds = std::make_shared<libv::ui::Label>(*this, "bounds");
		label_bounds->style(context().style("vm4pv.info.value"));
		add(label_bounds);
	}

	// -------------------------------------------------------------------------------------------------

	{
		auto tmp = std::make_shared<libv::ui::LabelImage>(*this);
		tmp->style(context().style("vm4pv.info.bar"));
		tmp->text("Count");
		add(std::move(tmp));
	}
	{
		label_info = std::make_shared<libv::ui::Label>(*this, "info");
		label_info->style(context().style("vm4pv.info.value"));
		add(label_info);
	}

	// -------------------------------------------------------------------------------------------------

	{
		auto tmp = std::make_shared<libv::ui::LabelImage>(*this);
		tmp->style(context().style("vm4pv.info.bar"));
		tmp->text("Material");
		add(std::move(tmp));
	}
	//	label_file.style(context().style("vm4_viewer.info.file"));
	//	label_info.style(context().style("vm4_viewer.info.info"));
}

//void ViewerInfoPanel::update(const std::string& path, const libv::vm4::Model& model) {
void ViewerInfoPanel::update(const libv::vm4::Model& model) {
//			panel_info.style(context().style("vm4_viewer.info"));

//	panel_general.title("General");
//	panel_general.set(panel_general.property.orientation, libv::ui::Orientation::down);
//
//	{
//		auto tmp = std::make_shared<libv::ui::Label>(*this);
//		tmp->style(context().style("vm4_viewer.info.key"));
//		tmp->text("Name");
//		panel_general(0, 0) = std::move(tmp);
//	}
//	{
//		auto tmp = std::make_shared<libv::ui::Label>(*this);
//		tmp->style(context().style("vm4_viewer.info.value"));
//		tmp->text(model.name);
//		panel_general(1, 0) = std::move(tmp);
//	}

	label_name->text(fmt::format(
			"Name: {}",
			model.name
	));

//	label_file.text(fmt::format(
//			"File:      {}"
//			"Version:   {}"
//			"Hash:      {}"
//			"Last edit: {}",
//			// TODO P5: Implement vm4 viewer addition file information display
//			"Not Implemented Yet",
//			"Not Implemented Yet",
//			"Not Implemented Yet",
//			"Not Implemented Yet"
//	));

	label_bounds->text(fmt::format(
			"AABB_max:  {}\n"
			"AABB_min:  {}\n"
			"BS_origin: {}\n"
			"BS_radius: {}",
			model.AABB_max,
			model.AABB_min,
			model.BS_origin,
			model.BS_radius
	));

	label_info->text(fmt::format(
			"Vertex count:    {:9}\n"
			"Index count:     {:9}\n"
			"Triangle count:  {:9}\n"
			"LOD count:       {:9}\n"
			"Material count:  {:9}\n"
			"Node count:      {:9}\n"
			"Mesh count:      {:9}\n"
			"Animation count: {:9}\n"
			"Channel count:   {:9}",
			model.vertices.size(),
			model.indices.size(),
			model.indices.size() / 3,
			model.lods.size(),
			model.materials.size(),
			model.nodes.size(),
			model.meshes.size(),
			model.animations.size(),
			model.animationChannels.size()
	));

	int i = 0;
//	label_lod.clear();
//	for (const auto& lod : scene.model.lods) {
//		auto& label = label_lod.emplace_back();
//
//		label.style(context().style("vm4_viewer.info.lod"));
//		label.text(fmt::format(
//				"Level: {}\n"
//				"Range far:   {:12.6f}\n"
//				"Range near:  {:12.6f}\n"
//				"Root nodeID: {}\n"
//				"Root node:   {}",
//				// TODO P5: Implement grid layout (? or grid text component; ? or expanding space text element)
//				// TODO P5: Implement component link, clicking root node should jump to node info
//				i++,
//				lod.rangeFar,
//				lod.rangeNear,
//				lod.rootNodeID,
//				scene.model.nodes[lod.rootNodeID].name
//		));
//	}

	i = 0;
	for (const auto& lbl : label_material)
		remove(lbl);
	label_material.clear();

	for (const auto& material : model.materials) {
		const auto& tmp = label_material.emplace_back(std::make_shared<libv::ui::Label>(*this, "material"));

		tmp->style(context().style("vm4pv.info.value"));

		std::ostringstream ss;
		ss << fmt::format(
				"ID: {}\n"
				"Name: {}\n"
				"Shader: {}\n",
				i++,
				material.name,
				material.shader
		);
		for (const auto& [key, data] : material.properties) {
			std::visit([&key, &ss](const auto& value) {
				ss << fmt::format("    {:16}: {}\n", key, value);
			}, data);
		}

		tmp->text(std::move(ss).str());
		add(tmp);
	}

	// =================================================================================================

	//		for (const auto& [level, lod] : scene.model.lods | ranges::view::enumerate) {
	//			auto& label = label_lod.emplace_back();
	//			label.text(fmt::format(
	//					"Level: {}\n"
	//					"Range far:   {:12.6f}\n"
	//					"Range near:  {:12.6f}\n"
	//					"Root nodeID: {}\n",
	//					"Root node:   {}",
	//					// TODO P5: Implement grid layout (? or grid text component)
	//					// TODO P5: Implement component link, clicking root node should jump to node info
	//					level,
	//					lod.rangeFar,
	//					lod.rangeNear,
	//					lod.rootNodeID,
	//					scene.model.nodes[lod.rootNodeID].name
	//			));
	//		}

	//		for (const auto& material : scene.model.materials) {
	//			material.name;
	//			material.property.size();
	//			material.shader;
	//		}
	//
	//		for (const auto& node : scene.model.nodes) {
	//			node.name;
	//			node.parentID;
	//			node.childrenIDs.size();
	//			scene.model.nodes[node.parentID];
	//			for (const auto& childID : node.childrenIDs) {
	//				scene.model.nodes[childID];
	//			}
	//			node.meshIDs.size();
	//			for (const auto& meshID : node.meshIDs) {
	//				scene.model.meshes[meshID];
	//			}
	//			node.transformation;
	//		}
	//
	//		for (const auto& mesh : scene.model.meshes) {
	//			mesh.name;
	//			mesh.baseIndex;
	//			mesh.baseVertex;
	//			mesh.materialID;
	//			scene.model.materials[mesh.materialID];
	//			mesh.numIndices;
	//		}
	//
	//		for (const auto& animation : scene.model.animations) {
	//			(void) animation;
	//		}
	//
	//		for (const auto& animationChannel : scene.model.animationChannels) {
	//			(void) animationChannel;
	//		}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
