// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/viewer_ui.hpp>
// ext
//#include <fmt/format.h>
//#include <boost/container/flat_set.hpp>
//#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
#include <libv/vm4/load.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/quick_file_picker.hpp>
#include <vm4_viewer/version.hpp>
#include <vm4_viewer/viewer_info_panel.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

ViewerUI::ViewerUI(BaseComponent& parent) : libv::ui::PanelFloat(parent, "VM4Viewer") { }

void ViewerUI::doAttach() {
	set(property.snapToEdge, libv::ui::SnapToEdge{false});
	set(property.squish, libv::ui::Squish{false});

	{
		auto style = context().style("vm4pv");
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("bg_shader", context().shaderImage());
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_shader", context().shaderFont());
		style->set("font_size", libv::ui::FontSize{11});
		style->set("quad_shader", context().shaderQuad());
	}

	{
		auto style = context().style("vm4pv.label_version");
		style->set("align", libv::ui::AlignHorizontal::Right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.info_panel");
		style->set("anchor_target", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.label_help");
		style->set("align", libv::ui::AlignHorizontal::Right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	// -------------------------------------------------------------------------------------------------

	{
		info_panel = std::make_shared<ViewerInfoPanel>(*this, "info_panel");
		info_panel->style(context().style("vm4pv.info_panel"));
		add(info_panel);
	}

	{
		label_version = std::make_shared<libv::ui::Label>(*this, "version");
		label_version->style(context().style("vm4pv.label_version"));
		label_version->text(app::full_version);
		add(label_version);
	}

	{
		picker_files = std::make_shared<app::ui::QuickFilePicker>(*this, "picker");
		picker_files->style(context().style("vm4pv.file_list"));
		picker_files->event_pick(this, [this](const app::ui::QuickFilePicker::EventPick& event) {
			this->load(event.component.value());
		});
		add(picker_files);
	}

	{
		label_version = std::make_shared<libv::ui::Label>(*this, "help");
		label_version->style(context().style("vm4pv.label_help"));
		label_version->text(
				"Strafe camera - WASD\n"
				"Lift camera - RF\n"
				"Roll camera - QE\n"
				"Zoom camera - TG"
		);
		add(label_version);
	}
}

void ViewerUI::init() { }

// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model
// TODO P5: libv.vm4: include AABB for each node(?) / LOD(?) / mesh(?) / model
//
//void ViewerUI::find_bounding_sphere() {
//	// Find (BSO) Bounding sphere origin and (BSR) Bounding sphere radius
//	const auto& node = scene.model.nodes[0];
//
//	auto referencePointW = libv::vec3f{};
//	auto referenceDistanceWSQ = 0.f;
//
//	for (const auto& meshID : node.meshIDs) {
//		const auto& mesh = scene.model.meshes[meshID];
//		const auto indexBegin = mesh.baseIndex;
//		const auto indexEnd = mesh.baseIndex + mesh.numIndices;
//
//		for (size_t index = indexBegin; index < indexEnd; ++index) {
//			const auto vertexID = scene.model.indices[index] + mesh.baseVertex;
//			const auto vertexPositionM = scene.model.vertices[vertexID].position;
//			const auto vertexPositionW = parentTransformation * node.transformation * vertexPositionM;
//
//			const auto vertexDistanceWSQ = (vertexPositionW - referencePointW).lengthSQ();
//			if (vertexDistanceWSQ > referenceDistanceWSQ) {
//				referencePointW
//
//			}
//		}
//	}
//
//	for (const auto& childID : node.childrenIDs)
//		referencePoint = findFarestVertex(scene.model.nodes[childID], referencePoint);
//
////	for (const auto& node : scene.model.nodes)
////	for (const auto& node : scene.model.nodes)
//}

void ViewerUI::update_model() {
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);
//	scene.model;
}

void ViewerUI::update_camera() {
//	scene.camera;
}

void ViewerUI::update_ui() {
//	this->in
}

void ViewerUI::load(const std::string& path) {
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);

	libv::Timer timer;

	log_app.info("Loading  {}...", path);
	auto file = libv::read_file_or_throw(path);
	const auto time_load = timer.timef_us().count();

	log_app.info("Parsing  {}...", path);
	scene.model = libv::vm4::load_or_throw(file);
	const auto time_parse = timer.timef_us().count();

	log_app.info("Updating {}...", path);

	info_panel->update(*scene.model);

	log_app.info("Name  : {}", scene.model->name);
	log_app.info("Vertex: {}", scene.model->vertices.size());
	const auto time_update = timer.timef_us().count();

	log_app.info("Processed {} in: Load {}us, Parse {}us, Update {}us", path, time_load, time_parse, time_update);
}

void ViewerUI::update_info() {
//	//		panel_info.property.set(context().style("vm4_viewer.info"));
//
//	label_name.property.set(context().style("vm4_viewer.info.name"));
//	label_name.text(fmt::format(
//			"Name: {}",
//			scene.model.name
//	));
//
//	label_file.property.set(context().style("vm4_viewer.info.file"));
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
//
//	label_info.property.set(context().style("vm4_viewer.info.info"));
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
//		label.property.set(context().style("vm4_viewer.info.lod"));
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
//		label.property.set(context().style("vm4_viewer.info.material"));
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
