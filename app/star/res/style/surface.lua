-- -------------------------------------------------------------------------------------------------

local fonts = {
	packed = "arialn.ttf",
	label = "calibril.ttf",
	title = "consola.ttf",
	mono = "consola.ttf",
}

local colors = {
	--text_light =         "hsva(  0°,  0%,  80%, 100%)",
	--text_dark =          "hsva(  0°,  0%,  20%, 100%)",
	--text_black =         "hsva(  0°,  0%,   0%, 100%)",
	--hud_light_bg_color = "rgba(0.5, 0.5, 0.5, 0.65)",
}


local stripped_background = {
	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.05}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
}

local stripped_background_interactive = {
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
}

-- === Time Control ========================================================================================

ui.style("game.info-bar") {
	font = fonts.label,
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
}

ui.style("game.info-bar.panel") {
	font = fonts.label,
	anchor = "top_center",
	orientation = "right",
	stripped_background,
	size = "100%, D",
}

--ui.style("game.info-bar.time.panel") {
--	anchor = "top_center",
--	orientation = "down",
--	size = "D, D",
--	padding = 8,
--	spacing = 4,
--}

ui.style("game.info-bar.group") {
	anchor = "top_center",
	orientation = "right",
	size = "D, D",
	padding = {8, 2},
	spacing = 8,
	background = "rgba(0.5, 0.5, 0.5, 0.9)",
}

ui.style("game.info-bar.label") {
	size = "D, D",
}

ui.style("game.info-bar.time.pause") {
	align_horizontal = "center",
	align_vertical = "center",
	size = "50px, D",
	stripped_background_interactive,
}

ui.style("game.info-bar.time.speed") {
	align_horizontal = "center",
	align_vertical = "center",
	--size = "40px, D",
	size = "40px, D",
	stripped_background_interactive,
}

--ui.style("game.info-bar.time.pause") {
--	--anchor = "bottom_center",
--	--orientation = "down",
--	--size = "D, D",
--	--padding = 8,
--	--spacing = 4,
--}
--
--ui.style("game.info-bar.time.speed") {
--	--anchor = "bottom_center",
--	--orientation = "down",
--	--size = "D, D",
--	--padding = 8,
--	--spacing = 4,
--}

--local stripped_background = {
--	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.05}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--}
--
--local stripped_background_interactive = {
--	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.05}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--
--	state("focus") {
--		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--	state("active") {
--		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--	state("hover") {
--		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--	state("hover, active") {
--		background = {type = "border_padding_pattern", color_border = {1.0, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--	state("disable") {
--		background = {type = "border_padding_pattern", color_border = {0.4, 0.4, 0.4, 0.6}, color_pattern = {0.4, 0.4, 0.4, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--}
--
--ui.style("surface.menu.label") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
--	caret_color = "rgba(1.0, 1.0, 1.0, 1.0)", -- Debug only, remove
--	padding = {16, 2},
--	size = "1rD, D",
--	stripped_background,
--}
--
--ui.style("surface.menu.button") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
--	padding = {16, 2},
--	size = "1rD, D",
--	stripped_background_interactive,
--}

-- === Menu ========================================================================================

ui.style("surface.menu") {
	font = fonts.label,
}

ui.style("surface.menu.line") {
	anchor = "bottom_center",
	orientation = "down",
	size = "D, D",
	padding = 8,
	spacing = 4,
}

ui.style("surface.menu.label") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	caret_color = "rgba(1.0, 1.0, 1.0, 1.0)", -- Debug only, remove
	padding = {16, 2},
	size = "1rD, D",
	stripped_background,
}

ui.style("surface.menu.button") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	padding = {16, 2},
	size = "1rD, D",
	stripped_background_interactive,
}
