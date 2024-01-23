-- -------------------------------------------------------------------------------------------------

--	font_shader = "default",
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),

-- -------------------------------------------------------------------------------------------------

local colors = {
	text_light =         "hsva(  0°,  0%,  80%, 100%)",
	text_dark =          "hsva(  0°,  0%,  20%, 100%)",
	text_black =         "hsva(  0°,  0%,   0%, 100%)",
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
	background = colors.hud_light_bg_color,
	size = "10pxD1r, 4pxD",

	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},

	state("focus") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("active") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("hover") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("hover, active") {
		background = {type = "border_padding_pattern", color_border = {1.0, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
}

--ui.style("space.hud-bar.mp.main") {
--	anchor = "top_center",
--	align_horizontal = "center",
--	align_vertical = "center",
--	orientation = "up",
--	size = "D, D",
--}

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
	background = colors.hud_light_bg_color,
	size = "10pxD, 4pxD",
}

ui.style("space.hud-bar.mp.input") {
	align_horizontal = "center",
	align_vertical = "center",
	background = colors.hud_light_bg_color,
	--size = "10pxD, D1r",
	size = "10pxD, 4pxD",
}

ui.style("space.hud-bar.mp.lbl") {
	align_horizontal = "center",
	align_vertical = "center",
	background = colors.hud_light_bg_color,
	size = "10pxD, 4pxD",
}

ui.style("space.title-space") {
	--ui.style("space.hud-bar.mp.btn") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	font_size = 18,
	--background = "color: red",
	--background = "color:" .. colors.hud_light_bg_color,
	--background = "texture: hexagon_metal_0001_diffuse.dds",
	--background = "pattern: hexagon_metal_0001_diffuse.dds",
	--background = "texture: separator_bar_256x16.png",
	--background = "texture: stretch_border.png",
	--background = "border: stretch_border.png",
	--background = "pattern: stretch_border.png",
	--background = "padding_pattern: stripes_border.png",
	--background = "padding_pattern: stripes.png",
	--background = "padding_pattern: stripes.png",
	--background = {type = "padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, texture = "stripes.png"},
	--background = {type = "padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border = 3, texture = "stripes.png"},
	--background = {type = "pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, texture = "stripes.png"},
	--background = {type = "border_padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
	--background = {type = "border_padding_pattern", color = {1, 1, 1, 1}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
	--background = {type = "border_padding_pattern", color_border = {1, 1, 1, 1}, color_pattern = {1, 1, 1, 1}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
	--background = {type = "border_padding_pattern", color_border = {1, 1, 1, 1}, color_pattern = {1, 1, 1, 1}, inner_padding = {4, -1}, border_extent = 3, texture = "stripes_border.png"},
	--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},

	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},

	state("focus") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("active") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("hover") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("hover, active") {
		background = {type = "border_padding_pattern", color_border = {1.0, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},

	padding = {60, 8},
	--padding = { 0, 7},

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





ui.style("space.hud-bar.lobby.panel") {
	align_horizontal = "left",
	align_vertical = "center",
	anchor = "center_left",
	background = colors.hud_light_bg_color,
	margin = 5,
	padding = 2,
	orientation = "down",
	size = "D, D",
	spacing = 2,
}

ui.style("space.hud-bar.lobby.name") {
	align_horizontal = "left",
	align_vertical = "center",
	background = colors.hud_light_bg_color,
	font_color = colors.text_black,
	padding = {10, 4},
	size = "D1r, D",
}

ui.style("space.hud-bar.lobby.title") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = colors.text_black,
	size = "10pxD, 4pxD",
}

ui.style("space.overlay.controls-help.lbl") {
	align_horizontal = "left",
	align_vertical = "center",
	anchor = "center",
	background = "color: rgba(0.0, 0.0, 0.0, 0.6)",
	padding = {6, 4},
	font_color = colors.text_light,

	size = "D, D",
}
