// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/panel_viewer.hpp>
// ext
#include <fmt/format.h>
#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/ui/context_ui.hpp>
// std
#include <sstream>
// pro
#include <vm4_viewer/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

PanelViewer::PanelViewer() : libv::ui::ComponentStatic<PanelViewer>("PanelViewer") {
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);
}

void PanelViewer::init() {

}

// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model
//
//void PanelViewer::find_bounding_sphere() {
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

void PanelViewer::update_model() {
//	scene.model;
}

void PanelViewer::update_camera() {
//	scene.camera;
}

void PanelViewer::update_ui() {
//	this->in
}

void PanelViewer::update_info() {
	libv::ui::ContextUI context; // <<< P0: give me a context here

	//		panel_info.properties.set(context.style("vm4_viewer.info"));

	// <<< P0: better style assignment? One possibly without the need of context?
	label_name.properties.set(context.style("vm4_viewer.info.name"));
	label_name.setText(fmt::format(
			"Name: {}",
			scene.model.name
	));

	label_file.properties.set(context.style("vm4_viewer.info.file"));
	label_file.setText(fmt::format(
			"File:      {}"
			"Version:   {}"
			"Hash:      {}"
			"Last edit: {}",
			// TODO P5: Implement vm4 viewer addition file information display
			"Not Implemented Yet",
			"Not Implemented Yet",
			"Not Implemented Yet",
			"Not Implemented Yet"
	));

	label_info.properties.set(context.style("vm4_viewer.info.info"));
	label_info.setText(fmt::format(
			"Vertex count:       {:9}\n"
			"Index count:        {:9}\n"
			"LOD count:          {:9}\n"
			"Material count:     {:9}\n"
			"Node count:         {:9}\n"
			"Mesh count:         {:9}\n"
			"Animation count:    {:9}\n"
			"Anim.Channel count: {:9}",
			scene.model.vertices.size(),
			scene.model.indices.size(),
			scene.model.lods.size(),
			scene.model.materials.size(),
			scene.model.nodes.size(),
			scene.model.meshes.size(),
			scene.model.animations.size(),
			scene.model.animationChannels.size()
	));

	int i = 0;
	label_lod.clear();
	for (const auto& lod : scene.model.lods) {
		auto& label = label_lod.emplace_back();

		label.properties.set(context.style("vm4_viewer.info.lod"));
		label.setText(fmt::format(
				"Level: {}\n"
				"Range far:   {:12.6f}\n"
				"Range near:  {:12.6f}\n"
				"Root nodeID: {}\n"
				"Root node:   {}",
				// TODO P5: Implement grid layout (? or grid text component; ? or expanding space text element)
				// TODO P5: Implement component link, clicking root node should jump to node info
				i++,
				lod.rangeFar,
				lod.rangeNear,
				lod.rootNodeID,
				scene.model.nodes[lod.rootNodeID].name
		));
	}

	i = 0;
	label_material.clear();
	for (const auto& material : scene.model.materials) {
		auto& label = label_material.emplace_back();

		label.properties.set(context.style("vm4_viewer.info.material"));

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
				ss << fmt::format("\t{:20}: {}\n", key, value);
			}, data);
		}

		label.setText(std::move(ss).str());
	}

	//		for (const auto& [level, lod] : scene.model.lods | ranges::view::enumerate) {
	//			auto& label = label_lod.emplace_back();
	//			label.setText(fmt::format(
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
	//			material.properties.size();
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


//void PanelViewer::create() {
//	add(panel_left);
//	add(panel_help);
//
//	panel_left->add(label_info);
//	label_info->setText("Hello world!");
//	label_info->properties.font

//	const auto style_label_01 = ui.context().style("style-label-01");
//	style_label_01->set("color", libv::parse::parse_color_or_throw("rgba(167, 152, 120, 100%)"));
//	style_label_01->set("font", ui.context().font("consola.ttf"));
//	style_label_01->set("align", libv::ui::Anchor::Left);
//
//	label0->properties.set(style_label_01);
//	label0->setText("Hello, Label0!");
//	label1->properties.set(style_label_01);
//	label1->setText("Hello, Label1!");
//	label2->properties.set(style_label_01);
//	label2->setText("Hello, Label2!");
//	label2->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");
//
//	quad0->color(libv::parse::parse_color_or_throw("rgba(134, 189, 111, 80%)"));
//	stretch0->image(ui.context().texture2D("stretch_border.png"));
//	stretch0->color(libv::parse::parse_color_or_throw("rgba(183, 190, 135, 100%)"));
//	image0->image(ui.context().texture2D("atlas_ui_slate.png"));
//	image0->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");

//		panel0->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel0->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel0->layout.orient = libv::ui::Orientation::TOP_TO_BOTTOM;
//		panel0->add(label0);
//		panel0->add(image0);
//		panel0->add(panel1);
//		panel0->add(label1);
//
//		panel1->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel1->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel1->layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//		panel1->add(quad0);
//		panel1->add(label2);
//		panel1->add(stretch0);
//
//		ui.add(panel0);
//}
//
//void PanelViewer::render() {
//}
//
//void PanelViewer::destroy() {
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
