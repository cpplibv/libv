-- -------------------------------------------------------------------------------------------------

local module = {}

module.golden = 1.618033988749894848204586834365638118

module.fonts = {
	packed = "arialn.ttf",
	label = "calibril.ttf",
	title = "consola.ttf",
	mono = "consola.ttf",
}

module.colors = {
	--text_light =         "hsva(  0°,  0%,  80%, 100%)",
	--text_dark =          "hsva(  0°,  0%,  20%, 100%)",
	--text_black =         "hsva(  0°,  0%,   0%, 100%)",
	--hud_light_bg_color = "rgba(0.5, 0.5, 0.5, 0.65)",
}

-- -------------------------------------------------------------------------------------------------

--	font_shader = "default",
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),

module.stripped_background = {
	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.05}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
}

module.stripped_background_interactive = {
	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.05}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},

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
	state("disable") {
		background = {type = "border_padding_pattern", color_border = {0.4, 0.4, 0.4, 0.6}, color_pattern = {0.4, 0.4, 0.4, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("select") {
		background = {type = "border_padding_pattern", color_border = {0.4, 0.8, 0.4, 0.8}, color_pattern = {0.8, 1.0, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
}

--module.debug_background = {
--	background = "black",
--	state("select") {
--		background = "blue",
--	},
--	state("hover") {
--		background = "green",
--	},
--	state("active") {
--		background = "red",
--	},
--	state("hover, select") {
--		background = "rgb(0, 1., 1.)",
--	},
--	state("active, select") {
--		background = "rgb(1.,0, 1.)",
--	},
--	state("active, hover") {
--		background = "rgb(1., 1., 0)",
--	},
--	state("active, hover, select") {
--		background = "rgb(1., 1., 1.)",
--	},
--}

-- -------------------------------------------------------------------------------------------------

return module;
