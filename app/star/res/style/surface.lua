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
	state("select") {
		background = {type = "border_padding_pattern", color_border = {0.4, 0.8, 0.4, 0.8}, color_pattern = {0.8, 1.0, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
}

--local debug_background = {
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

-- === Info bar ============================================================================================

ui.style("game.info-bar") {
	font = fonts.label,
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
}

ui.style("game.info-bar.panel") {
	font = fonts.label,
	anchor = "top_center",
	orientation = "right",
	--margin = 2,
	size = "100%, D",
}

ui.style("game.info-bar.group") {
	anchor = "top_center",
	orientation = "right",
	size = "D, D1r",
	padding = {8, 2},
	spacing = 8,
	background = "#000A",
}

ui.style("game.info-bar.label") {
	align_horizontal = "center",
	align_vertical = "center",
	size = "D, D1r",
}

-- Time Control

ui.style("game.info-bar.time.pause") {
	align_horizontal = "center",
	align_vertical = "center",
	size = "60px, D1r",
	padding = 2,
	focus_mode = "passive",
	stripped_background_interactive,
	-- debug_background,
}

ui.style("game.info-bar.time.speed") {
	align_horizontal = "center",
	align_vertical = "center",
	size = "40px, D1r",
	padding = 2,
	focus_mode = "passive",
	stripped_background_interactive,
	-- debug_background,
}

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
	background = "#0006",
}

ui.style("surface.menu.label") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	caret_color = "rgba(1.0, 1.0, 1.0, 1.0)", -- Debug only, remove (input should have label style, but that is a debug input)
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

-- === Test ========================================================================================

ui.style("surface.test") {
	font = fonts.label,
}

ui.style("surface.test.bar") {
	anchor = "bottom_center",
	orientation = "left",
	size = "1r, 500px",
	padding = 8,
	spacing = 4,
	background = "#0004",
}

ui.style("surface.test.card") {
	--anchor = "bottom_center",
	align_vertical = "bottom",
	orientation = "up",
	--size = "1r, 500px",
	--padding = 8,
	--spacing = 4,
	background = "#0008",
	state ("focus-within") {
		background = "#1218",
	},
}

ui.style("surface.test.body") {
	--anchor = "bottom_center",
	orientation = "down",
	--size = "1r, 500px",
	--padding = 8,
	--spacing = 4,
	background = "#383A33FF",
}

ui.style("surface.test.input") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	caret_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	padding = {16, 2},
	size = "1rD, D",
	stripped_background,
}

ui.style("surface.test.button") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	padding = {16, 2},
	size = "1rD, D",
	stripped_background_interactive,
}

ui.style("surface.test.counter") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	padding = {16, 2},
	size = "1rD, D",
	--stripped_background_interactive,
}
