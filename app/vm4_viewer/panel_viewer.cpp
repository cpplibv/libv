// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/panel_viewer.hpp>
// ext
#include <fmt/format.h>
#include <boost/container/flat_set.hpp>
//#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/image.hpp> // testing only, remove it
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/label_image.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/quad.hpp> // testing only, remove it
#include <libv/ui/component/stretch.hpp> // testing only, remove it
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/shader/shader_font.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/ui/shader/shader_image.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/ui/shader/shader_quad.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/utility/generic_path.hpp>
#include <libv/utility/overload.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/quick_file_picker.hpp>
#include <vm4_viewer/version.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

PanelViewer::PanelViewer() : libv::ui::PanelFloat("VM4Viewer") { }

void PanelViewer::key(libv::input::Key key) {
	if (!isAttached())
		return;

	static int16_t i = 11;
	if (key == libv::input::Key::Q) i++;
	if (key == libv::input::Key::A) i--;

	if (picker_files != nullptr)
		picker_files->key(key);

	{
		auto style = context().style("vm4pv.info.bar");
		style->set("font_size", libv::ui::FontSize{i});
	}

	{
		auto style = context().style("vm4pv.button");
		auto v = style->get_or_throw<libv::ui::Size>("size");
		v[0].pixel = (static_cast<float>(i)-11.f) * 2.f;
		v[1].pixel = (static_cast<float>(i)-11.f) * 5.f;
		style->set("size", v);
	}

	log_app.trace("Font size {}", i);
}

void PanelViewer::doAttach() {
	set(properties.snapToEdge, false);
	set(properties.squish, false);

	{
		auto style = context().style("vm4pv");
		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_shader", context().shaderFont());
		style->set("font_size", libv::ui::FontSize{11});
		style->set("image_shader", context().shaderImage());
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
		auto style = context().style("vm4pv.button");
		style->set("align", libv::ui::AlignHorizontal::Center);
		style->set("align_vertical", libv::ui::AlignVertical::Center);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(170, 2%, 90%, 100%)"));
		style->set("font_size", libv::ui::FontSize{12});
		style->set("image", context().texture2D("separator_bar_256x16.png"));
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

	{
		auto style = context().style("vm4pv.info");
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
		style->set("anchor_target", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, 100%"));
	}

//	{
//		auto style = context().style("vm4pv.file_list");
//		style->set("align", libv::ui::AlignHorizontal::Left);
//		style->set("align_vertical", libv::ui::AlignVertical::Top);
//		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
//		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
//		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
//		style->set("font_size", libv::ui::FontSize{11});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry");
//		style->set("font_size", libv::ui::FontSize{11});
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.selected");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 100%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.vm");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(120, 22%, 90%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.importable");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 22%, 90%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.other");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 50%, 100%)"));
//	}

//	{
//		auto style = context().style("vm4pv.info.separator");
//		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("image", context().texture2D("separator_bar_256x16.png"));
//		style->set("size", libv::ui::parse_size_or_throw("1r, 8px"));
//	}

	{
		auto style = context().style("vm4pv.info.bar");
		style->set("align", libv::ui::AlignHorizontal::Center);
		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(170, 2%, 90%, 100%)"));
		style->set("font_size", libv::ui::FontSize{11});
		style->set("image", context().texture2D("separator_bar_256x16.png"));
		style->set("size", libv::ui::parse_size_or_throw("1r D, D"));
	}

//	{
//		auto style = context().style("vm4pv.info.title");
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("font_size", libv::ui::FontSize{12});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}
//
//	{
//		auto style = context().style("vm4pv.info.border");
//		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("image_shader", context().shaderImage());
//		style->set("image", context().texture2D("stretch_border.png"));
//		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
//	}

	{
		auto style = context().style("vm4pv.info.slim_border");
		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("image_shader", context().shaderImage());
		style->set("image", context().texture2D("slate_line_edge.png"));
		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
	}

	// -------------------------------------------------------------------------------------------------

	{
		label_version = std::make_shared<libv::ui::Label>("version");
		label_version->style(context().style("vm4pv.label_version"));
		label_version->setText(app::full_version);
		add(label_version);
	}

	{
		picker_files = std::make_shared<app::ui::QuickFilePicker>("picker");
		picker_files->style(context().style("vm4pv.file_list"));
		add(picker_files);
	}

	{
		label_version = std::make_shared<libv::ui::Label>("help");
		label_version->style(context().style("vm4pv.label_help"));
		label_version->setText(
				"Strafe camera - WASD\n"
				"Lift camera - RF\n"
				"Roll camera - QE\n"
				"Zoom camera - TG"
		);
		add(label_version);
	}

	{
		panel_left = std::make_shared<libv::ui::PanelLine>("info");
		panel_left->style(context().style("vm4pv.info"));
		add(panel_left);

		{
			button_close = std::make_shared<libv::ui::Button>("close");
			button_close->style(context().style("vm4pv.button"));
			button_close->setText("Button0");
			button_close->setCallback([this](const libv::ui::EventMouse& event) {
				if (event.isButton())
					button_close->focus();

				if (event.isButton())
					log_app.trace("Button0 Click {} {}", libv::to_value(event.button().button),  libv::to_value(event.button().action));

				if (event.isMovement())
					log_app.trace("Button0 Movement {} {} {} {}", event.movement().mouse_position, event.movement().mouse_movement, event.movement().enter, event.movement().leave);

				if (event.isScroll())
					log_app.trace("Button0 Scroll {} {}", event.scroll().scroll_position, event.scroll().scroll_movement);
			});

			panel_left->add(button_close);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>("bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->setText("Project");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>("bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->setText("File");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			button_close = std::make_shared<libv::ui::Button>("button1");
			button_close->style(context().style("vm4pv.button"));
			button_close->setText("Button1");
			button_close->setCallback([this](const libv::ui::EventMouse& event) {
				if (event.isButton())
					button_close->focus();

				if (event.isButton())
					log_app.trace("Button1 Click {} {}", libv::to_value(event.button().button),  libv::to_value(event.button().action));

				if (event.isMovement())
					log_app.trace("Button1 Movement {} {} {} {}", event.movement().mouse_position, event.movement().mouse_movement, event.movement().enter, event.movement().leave);

				if (event.isScroll())
					log_app.trace("Button1 Scroll {} {}", event.scroll().scroll_position, event.scroll().scroll_movement);
			});

			panel_left->add(button_close);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>("bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->setText("Model");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>("bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->setText("Node");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>("bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->setText("Material");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>("border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		}

//		{
//			auto temp = std::make_shared<libv::ui::Label>("text");
//			temp->set(temp->properties.align, libv::ui::AlignHorizontal::Right);
//			temp->set(temp->properties.font, context().font("consola.ttf"));
//			temp->set(temp->properties.font_color, libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//			temp->set(temp->properties.font_shader, context().shaderFont());
//			temp->set(temp->properties.font_size, libv::ui::FontSize{12});
//			temp->setText("Model");
//
//			auto& child = panel_left->add(temp);
//			child.ptr->set(child.properties.size, libv::ui::parse_size_or_throw("d, d"));
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Quad>();
//			temp->set(temp->properties.color, libv::parse::parse_color_or_throw("hsva(120°, 30%, 100%, 50%)"));
//
//			auto& child = panel_left->add(temp);
//			child.ptr->set(child.properties.size, libv::ui::parse_size_or_throw("100%, 50px"));
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Image>("separator");
//			temp->style(context().style("vm4pv.info.separator"));
//			panel_left->add(temp);
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Stretch>("border");
//			temp->style(context().style("vm4pv.info.border"));
//			panel_left->add(temp);
//		}
	}
}

void PanelViewer::init() { }

// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model
// TODO P5: libv.vm4: include AABB for each node(?) / LOD(?) / mesh(?) / model
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
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);
//	scene.model;
}

void PanelViewer::update_camera() {
//	scene.camera;
}

void PanelViewer::update_ui() {
//	this->in
}

void PanelViewer::update_info() {
//	//		panel_info.properties.set(context().style("vm4_viewer.info"));
//
//	label_name.properties.set(context().style("vm4_viewer.info.name"));
//	label_name.setText(fmt::format(
//			"Name: {}",
//			scene.model.name
//	));
//
//	label_file.properties.set(context().style("vm4_viewer.info.file"));
//	label_file.setText(fmt::format(
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
//	label_info.properties.set(context().style("vm4_viewer.info.info"));
//	label_info.setText(fmt::format(
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
//		label.properties.set(context().style("vm4_viewer.info.lod"));
//		label.setText(fmt::format(
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
//		label.properties.set(context().style("vm4_viewer.info.material"));
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
//		for (const auto& [key, data] : material.properties) {
//			std::visit([&key, &ss](const auto& value) {
//				ss << fmt::format("\t{:20}: {}\n", key, value);
//			}, data);
//		}
//
//		label.setText(std::move(ss).str());
//	}
//


	// =================================================================================================

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
