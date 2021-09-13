


-- -------------------------------------------------------------------------------------------------

--	font_shader = "default",
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),

-- -------------------------------------------------------------------------------------------------

local colors = {
	text_light = "hsva(  0°,  0%,  80%, 100%)",
	text_dark = "hsva(  0°,  0%,  20%, 100%)",
	hud_light_bg_color = "rgba(0.5, 0.5, 0.5, 0.65)",
}

-- -------------------------------------------------------------------------------------------------

ui.style("space") {
	font = "consola.ttf",
	font_color = "rgba(0.0, 0.0, 0.0, 1.0)",
	font_size = 12,
}

ui.style("space.canvas.main") {

}

ui.style("space.canvas.mini") {
	anchor = "center_right",
	size = "1r, 15%",
	margin = 10,
}

ui.style("space.hud-bar.cmd.panel") {
	anchor = "bottom_right",
	orientation = "down",
	size = "D, D",
	spacing = 5,
	margin = 5,
}

ui.style("space.hud-bar.cmd.btn") {
	align_horizontal = "center",
	align_vertical = "center",
	bg_color = colors.hud_light_bg_color,
	size = "10pxD1r, 4pxD",
}

ui.style("space.hud-bar.mp.panel") {
	anchor = "top_center",
	orientation = "right",
	size = "D, D",
	spacing = 5,
	margin = 5,
}

ui.style("space.hud-bar.mp.btn") {
	align_horizontal = "center",
	align_vertical = "center",
	bg_color = colors.hud_light_bg_color,
	size = "10pxD, 4pxD",
}

ui.style("space.hud-bar.mp.input") {
	align_horizontal = "center",
	align_vertical = "center",
	bg_color = colors.hud_light_bg_color,
	size = "10pxD, D1r",
}

ui.style("space.hud-bar.mp.lbl") {
	align_horizontal = "center",
	align_vertical = "center",
	bg_color = colors.hud_light_bg_color,
	size = "10pxD, 4pxD",
}

ui.style("space.hud-bar.mp.iris-lbl") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	--background = "color: red";
	--background = "color:" .. colors.hud_light_bg_color;
	--background = "texture: atlas_ui_slate.png";
	--background = "texture: hexagon_metal_0001_diffuse.dds";
	--background = "pattern: hexagon_metal_0001_diffuse.dds";
	--background = "texture: separator_bar_256x16.png";
	--background = "texture: stretch_border.png";
	--background = "border: stretch_border.png";
	--background = "pattern: stretch_border.png";
	--background = "padding_pattern: padding_stripes.png";
	background = "padding_pattern: stripes.png";

	padding = { 40, 7 },
	--padding = {  0, 7 },

	--size = "200px, 200px",
	--size = "100px, 100px",
	--size = "6px D 1r, 10px D",
	--size = "6px D 1r, 16px",
	--size = "6px D 1r, 19px",

	--size = "6px D 1r, 1px D",
	--size = "6px D 1r, 2px D",
	--size = "6px D 1r, 4px D",
	--size = "6px D 1r, 8px D",

	--size = "6px D 1r, D",
	--size = "6px D 1r, 20px",
	--size = "4pxD, D",
	size = "10pxD, 4pxD",
}

ui.style("space.hud-bar.mp-cam.panel") {
	anchor = "center_right",
	orientation = "down",
	size = "25%, 100%",
	spacing = 5,
	margin = 5,
}
