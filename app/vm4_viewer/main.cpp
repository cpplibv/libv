// Project: libv, File: app/vm4_viewer/main.cpp


// TODO P5: app.vm4_viewer: show model wireframe (GS based wireframe)
// TODO P5: app.vm4_viewer: vm4 LOD support
// TODO P5: app.vm4_viewer: pointless loading bar
// TODO P5: app.vm4_viewer: thumbnail support, and auto generation (async operation)
// TODO P5: app.vm4_viewer: display vm4 node hierarchy
// TODO P5: app.vm4_viewer: show material information
// TODO P5: app.vm4_viewer: show model texture information
// TODO P5: app.vm4_viewer: show model textured
// TODO P5: app.vm4_viewer: vm4 slot/attachment support
// TODO P5: app.vm4_viewer: save model
// TODO P5: app.vm4_viewer: skybox
// TODO P5: app.vm4_viewer: animations
// TODO P5: app.vm4_viewer: lighting
// TODO P5: app.vm4_viewer: multiple light
// TODO P5: app.vm4_viewer: shaders
// TODO P5: app.vm4_viewer: Forward+
// TODO P5: app.vm4_viewer: AA
// TODO P5: app.vm4_viewer: full game render engine (what ?!)
// TODO P5: app.vm4_viewer: billboard LOD texture

// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model
// TODO P5: libv.vm4: include AABB for each node(?) / LOD(?) / mesh(?) / model

void main() {
	std::shared_ptr<app::ui::OverlayShaderError> overlay_shader_error;
	std::shared_ptr<app::ui::OverlayShaderInspector> overlay_shader_inspector;
	std::shared_ptr<app::ui::OverlayNotification> overlay_notification;

	file_picker->event_pick(this, [this](const app::ui::QuickFilePicker::EventPick& event) {
		this->load(event.component.value());
	});

	label_help->text(
			"Strafe camera - WASD\n"
			"Lift camera - YH\n"
			"Roll camera - QE\n"
			"Zoom camera - TG\n"
			"Focus camera - F\n"
			"Reset camera - Shift+F"
	);

	//	KeyValuePanel panel_general{*this, "general"};
	//	std::shared_ptr<libv::ui::Label> label_name;
	//	std::shared_ptr<libv::ui::Label> label_info;
	//	std::shared_ptr<libv::ui::Label> label_bounds;
	//	std::vector<std::shared_ptr<libv::ui::Label>> label_material;
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

	style->set("bg_image", context().texture2D("separator_bar_256x16.png"));
}

void PanelViewer::find_bounding_sphere() {
	// Find (BSO) Bounding sphere origin and (BSR) Bounding sphere radius
	const auto& node = scene.model.nodes[0];

	auto referencePointW = libv::vec3f{};
	auto referenceDistanceWSQ = 0.f;

	for (const auto& meshID : node.meshIDs) {
		const auto& mesh = scene.model.meshes[meshID];
		const auto indexBegin = mesh.baseIndex;
		const auto indexEnd = mesh.baseIndex + mesh.numIndices;

		for (std::size_t index = indexBegin; index < indexEnd; ++index) {
			const auto vertexID = scene.model.indices[index] + mesh.baseVertex;
			const auto vertexPositionM = scene.model.vertices[vertexID].position;
			const auto vertexPositionW = parentTransformation * node.transformation * vertexPositionM;

			const auto vertexDistanceWSQ = (vertexPositionW - referencePointW).length_sq();
			if (vertexDistanceWSQ > referenceDistanceWSQ) {
				referencePointW

			}
		}
	}

	for (const auto& childID : node.childrenIDs)
		referencePoint = findFarestVertex(scene.model.nodes[childID], referencePoint);

//	for (const auto& node : scene.model.nodes)
//	for (const auto& node : scene.model.nodes)
}

void PanelViewer::update_model() {
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);
}

void ViewerInfoPanel::update(const libv::vm4::Model& model) {
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

void PanelViewer::doAttach() {
	{
		auto panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show");
		panel.style(context().style("vm4pv.scroll-show"));

		{
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(100); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(0); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(10); tmp.value_high(210); tmp.value_low(10); tmp.value_range(20); tmp.value_step(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(10); tmp.value_high(200); tmp.value_low(0); tmp.value_range(20); tmp.value_step(20);
		}
	}

	{
		auto panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show2");
		panel.style(context().style("vm4pv.scroll-show2"));

		{
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(5); tmp.value_high(10); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(100); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(0); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(10); tmp.value_high(210); tmp.value_low(10); tmp.value_range(20); tmp.value_step(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(10); tmp.value_high(200); tmp.value_low(0); tmp.value_range(20); tmp.value_step(20);
		}
	}
}
