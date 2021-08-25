

--local colors = {
--	text = "hsva(  0°,  0%,  80%, 100%)",
--}

-- TODO P1: Set font and other font settings

ui.style("space-canvas-main") {

}

ui.style("space-canvas-mini") {
	--canvas_mini.size(libv::ui::parse_size_or_throw("1r, 15%"));
	--canvas_mini.margin(10);
	--canvas_mini.anchor(libv::ui::Anchor::center_right);
}

ui.style("space.hud-bar.cmd") {
	--cmd_bar.anchor(libv::ui::Anchor::bottom_right);
	--cmd_bar.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	--cmd_bar.size(libv::ui::parse_size_or_throw("D, D"));
	--cmd_bar.spacing(5);
	--cmd_bar.margin(5);
}

ui.style("space.hud-bar.cmd.button") {
	--clear_fleets.align_horizontal(libv::ui::AlignHorizontal::center);
	--clear_fleets.align_vertical(libv::ui::AlignVertical::center);
	--clear_fleets.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
	--clear_fleets.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
}

ui.style("space.hud-bar.mp") {
	--mp_bar.anchor(libv::ui::Anchor::top_center);
	--mp_bar.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	--mp_bar.size(libv::ui::parse_size_or_throw("D, D"));
	--mp_bar.spacing(5);
	--mp_bar.margin(5);
}

ui.style("space.hud-bar.mp.btn") {
	--btn_host.align_horizontal(libv::ui::AlignHorizontal::center);
	--btn_host.align_vertical(libv::ui::AlignVertical::center);
	--btn_host.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
	--btn_host.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
}

ui.style("space.hud-bar.mp.input") {
	--in_name.align_horizontal(libv::ui::AlignHorizontal::center);
	--in_name.align_vertical(libv::ui::AlignVertical::center);
	--in_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
	--in_name.size(libv::ui::parse_size_or_throw("10pxD, 1r"));
}

ui.style("space.hud-bar.mp.lbl") {
	--lbl_name.align_horizontal(libv::ui::AlignHorizontal::center);
	--lbl_name.align_vertical(libv::ui::AlignVertical::center);
	--lbl_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
	--lbl_name.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
}

ui.style("space.hud-bar.mp-cam") {
	--mp_cam_bar.anchor(libv::ui::Anchor::center_right);
	--mp_cam_bar.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	--mp_cam_bar.size(libv::ui::parse_size_or_throw("25%, 100%"));
	--mp_cam_bar.spacing(5);
	--mp_cam_bar.margin(5);
}


--ui.style("vm4_viewer") {
--	size = "1r, 1r",
--	--	layout = ui.layout_float(),
--
--	layout = ui.layout_float(
--			ui.ANCHOR_TOP_LEFT,     -- Info
--			ui.ANCHOR_BOTTOM_RIGHT, -- Help
--			ui.ANCHOR_TOP_RIGHT     --
--	),
--}
--
--ui.style("vm4_viewer.info") {
--	background = "#00FF007F",
--	font = "consola.ttf",
--	font_color = colors.text,
--	font_shader = "default",
--	font_size = 14,
--	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
--	size = "dynamic, 1r",
----	align = "right",
----	align_vertical = "center",
----	anchor = ui.ANCHOR_TOP_CENTER,
----	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),
--}
--
--ui.style("vm4_viewer.info.name") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.file") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.info") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.lod") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.material") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.node") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.lod") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.mesh") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.animation") {
--	size = "1r, dynamic",
--}
--
--ui.style("vm4_viewer.info.animationChannel") {
--	size = "1r, dynamic",
--}
