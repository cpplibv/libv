

local colors = {
	text = "hsva(  0°,  0%,  80%, 100%)",
}

ui.style("vm4_viewer") {
	size = "1r, 1r",
--	layout = ui.layout_float(),

	layout = ui.layout_float(
			ui.ANCHOR_TOP_LEFT,     -- Info
			ui.ANCHOR_BOTTOM_RIGHT, -- Help
			ui.ANCHOR_TOP_RIGHT,    --
	),
}

ui.style("vm4_viewer.info") {
	background = "#00FF007F",
	font = "consola.ttf",
	font_color = colors.text,
	font_shader = "default",
	font_size = 14,
	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
	size = "dynamic, 1r",
--	align = "right",
--	align_vertical = "center",
--	anchor = ui.ANCHOR_TOP_CENTER,
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),
}

ui.style("vm4_viewer.info.name") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.file") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.info") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.lod") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.material") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.node") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.lod") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.mesh") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.animation") {
	size = "1r, dynamic",
}

ui.style("vm4_viewer.info.animationChannel") {
	size = "1r, dynamic",
}
