// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/viewer_info_panel.hpp>
// libv
#include <libv/parse/color.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/vm4/model.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

ViewerInfoPanel::ViewerInfoPanel(BaseComponent& parent) :
	libv::ui::PanelLine(parent, libv::ui::GenerateName, "ViewerInfoPanel") { }

ViewerInfoPanel::ViewerInfoPanel(BaseComponent& parent, std::string name) :
	libv::ui::PanelLine(parent, std::move(name)) { }

ViewerInfoPanel::ViewerInfoPanel(BaseComponent& parent, libv::ui::GenerateName_t, const std::string_view type) :
	libv::ui::PanelLine(parent, libv::ui::GenerateName, type) { }

void ViewerInfoPanel::doAttach() {
	set(property.align_horizontal, libv::ui::AlignHorizontal::Left);
	set(property.align_vertical, libv::ui::AlignVertical::Top);
	set(property.orientation, libv::ui::Orientation::TOP_TO_BOTTOM);

//	{
//		auto style = context().style("vm4pv.label_info");
//		style->set("align", libv::ui::AlignHorizontal::Right);
//		style->set("font", context().font("consola.ttf"));
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
//		style->set("font_size", libv::ui::FontSize{10});
//		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
//		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}

	{
		auto style = context().style("vm4pv.info.h1");
		style->set("align", libv::ui::AlignHorizontal::Right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.info.value");
		style->set("align", libv::ui::AlignHorizontal::Right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	// -------------------------------------------------------------------------------------------------

//	{
//		label_version = std::make_shared<libv::ui::Label>(*this, "version");
//		label_version->style(context().style("vm4pv.label_version"));
//		label_version->text(app::full_version);
//		add(label_version);
//	}
	{
//		label_name.style(context().style("vm4_viewer.info.value"));
		//	label_file.style(context().style("vm4_viewer.info.file"));
		//	label_info.style(context().style("vm4_viewer.info.info"));
	}
}

void ViewerInfoPanel::update(const libv::vm4::Model& model) {
//			panel_info.style(context().style("vm4_viewer.info"));

//	label_name.text(fmt::format(
//			"Name: {}",
//			model.name
//	));

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

//	label_info.text(fmt::format(
//			"Vertex count:       {:9}\n"
//			"Index count:        {:9}\n"
//			"LOD count:          {:9}\n"
//			"Material count:     {:9}\n"
//			"Node count:         {:9}\n"
//			"Mesh count:         {:9}\n"
//			"Animation count:    {:9}\n"
//			"Anim.Channel count: {:9}",
//			scene.model.vertices.size(),
//			scene.model.indices.size(),
//			scene.model.lods.size(),
//			scene.model.materials.size(),
//			scene.model.nodes.size(),
//			scene.model.meshes.size(),
//			scene.model.animations.size(),
//			scene.model.animationChannels.size()
//	));
//
//	int i = 0;
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
//
//	i = 0;
//	label_material.clear();
//	for (const auto& material : scene.model.materials) {
//		auto& label = label_material.emplace_back();
//
//		label.style(context().style("vm4_viewer.info.material"));
//
//		std::ostringstream ss;
//		ss << fmt::format(
//				"ID: {}\n"
//				"Name: {}\n"
//				"Shader: {}\n",
//				i++,
//				material.name,
//				material.shader
//		);
//		for (const auto& [key, data] : material.property) {
//			std::visit([&key, &ss](const auto& value) {
//				ss << fmt::format("\t{:20}: {}\n", key, value);
//			}, data);
//		}
//
//		label.text(std::move(ss).str());
//	}
//

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
