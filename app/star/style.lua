-- -------------------------------------------------------------------------------------------------

--	font_shader = "default",
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),

-- -------------------------------------------------------------------------------------------------

local golden = 1.618033988749894848204586834365638118

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

-- === Main Menu ===================================================================================

ui.style("main-menu") {
	font = fonts.label,
}

ui.style("main-menu.golden-line") {
	-- Golden ratio is achieved with a 2.0r box with center alignment and a 0.61r gap
	orientation = "left",
}

ui.style("main-menu.golden-gap") {
	size = (golden - 1) .. "r, 1r",
}

ui.style("main-menu.golden-box") {
	size = "2r, 2r",
	align_horizontal = "center",
	align_vertical = "center",
}

ui.style("main-menu.menu.panel") {
	size = "D, D",
	orientation = "down",
	anchor = "center_center",
	spacing = 8,
}

ui.style("main-menu.menu.title") {
	size = "1rD, D",
	align_horizontal = "center",
	align_vertical = "center",
	font_color = {1, 1, 1, 1},
	font_size = 34,
	margin = {0, 40, 0, 0}
}

ui.style("main-menu.menu.entry") {
	size = "1rD, D",
	align_horizontal = "center",
	align_vertical = "center",
	padding = {40, 5},
	font_size = 22,
	font_color = {1, 1, 1, 1},

	--background = "hsva(0.0, 0.0, 5%, 1.0)",
	--background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
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
		font_color = {0.5, 0.5, 0.5, 1},
		background = {type = "border_padding_pattern", color_border = {0.4, 0.4, 0.4, 0.6}, color_pattern = {0.4, 0.4, 0.4, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
}

ui.style("main-menu.version-lbl") {
	size = "D, D",
	anchor = "bottom_right",
	align_horizontal = "center",
	align_vertical = "center",
	margin = {10, 0},
	font_color = {1, 1, 1, 1},
}

-- === Settings ====================================================================================

	--align_horizontal = "left",
	--align_horizontal = "justify_all",
	--anchor = "center_center",

ui.style("settings") {
	caret_color = {1, 1, 1, 1},
}

ui.style("settings.main") {
	anchor = "center",
	--size = "1r, 1r",
	orientation = "down",
	--background = "red",
	padding = 25,
}

ui.style("settings.title") {
	size = "1r, D",
	font_size = 20,
	padding = {18, 6},
	font = fonts.title,
	font_color = {1, 1, 1, 1},
	--background = "green",
}

ui.style("settings.ctrl_line") {
	spacing = 8,
	orientation = "right",
}

ui.style("settings.ctrl") {
	size = "D, D",
	font_size = 20,
	padding = {28, 6},
	font = fonts.title,
	font_color = {1, 1, 1, 1},
	margin = {0, 0, 0, 20},
	--background = "green",

	--background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
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
		font_color = {0.5, 0.5, 0.5, 1},
		background = {type = "border_padding_pattern", color_border = {0.4, 0.4, 0.4, 0.6}, color_pattern = {0.4, 0.4, 0.4, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	--state("important disable") {
	--
	--},
	--
	--DERIVED_PROPERTIES...
	--state.above("select") {...}
	--state("select", "override:hover") {...}
	--state("select") {...}
}

ui.style("settings.grid") {
	size = "1r, D",
	orientation2 = "right down",
	column_count = 2,
	padding = {20, 10},
	spacing2 = {20, 4},
	background = "hsva(240, 15%, 15%, 80%)",
	--background = "yellow",
}

ui.style("settings.entry.lbl") {
	size = "D, D",
	anchor = "left",
	font = fonts.label,
	font_color = {1, 1, 1, 1},
}

ui.style("settings.entry.value") {
	size = "1r, D",
	padding = {4, 4},
	anchor = "left",
	align_horizontal = "center",
	align_vertical = "center",
	font_color = {1, 1, 1, 1},
	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
	state("error") {
		background = {type = "border_padding_pattern", color_border = {1.0, 0.3, 0.3, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.1}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
}

ui.style("settings.entry.value.number") {
}

ui.style("settings.entry.value.toggle") {
}

ui.style("settings.entry.value.text") {
}

ui.style("settings.entry.value.option.row") {
	orientation = "right",
	background = "none",
	padding = 0,
	spacing = 5,
}

ui.style("settings.entry.value.option.button") {
	--size = "D, D",
	--background = "yellow",
	--background = IMPLICIT;
	--background = inherit,
	--background = inherit("settings.entry.value"),
	--background = inherit("settings.entry.value", "active, select"),

	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},

	state("select") {
		--background = "green",
		--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},

	--state_override("select") {
	--
	--},
	--state("select") {
	--	--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	background = "green",
	--},
	--inherit("background", "settings.entry.value"),

	--state("!select") {
	--	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	background = "blue",
	--},
}

-- =================================================================================================

ui.style("controls.visual.line") {
	align_horizontal = "center";
	align_vertical = "center";
}

ui.style("controls.visual.board") {
	--background = {type = "border", color = {0.5, 0.6, 0.0, 0.8}, texture = "light_border.png"},
	size = "D, D",
}

ui.style("controls.visual.key") {
	font = fonts.packed,
	font_color = "white",
	font_size = 12,
	align_horizontal = "center";
	align_vertical = "center";


	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
	state("hover") {
		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},
	state("hover, active") {
		background = {type = "border_padding_pattern", color_border = {1.0, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	},

	--state("select") {
	--	--background = "green",
	--	--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--},

	--state_override("select") {
	--
	--},
	--state("select") {
	--	--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	background = "green",
	--},
	--inherit("background", "settings.entry.value"),

	--state("!select") {
	--	background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {1.0, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	background = "blue",
	--},
}


-- =================================================================================================

--ui.style("star") {
--	font = "consola.ttf",
--	font_color = "rgba(0.0, 0.0, 0.0, 1.0)",
--	font_size = 12,
--}
--
--ui.style("star.canvas.main") {
--
--}
--
--ui.style("star.canvas.mini") {
--	anchor = "center_right",
--	size = "1r, 15%",
--	margin = 10,
--}
--
--ui.style("star.hud-bar.cmd.panel") {
--	anchor = "bottom_right",
--	orientation = "down",
--	size = "D, D",
--	spacing = 5,
--	margin = 5,
--}
--
--ui.style("star.hud-bar.cmd.btn") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	background = colors.hud_light_bg_color,
--	size = "10pxD1r, 4pxD",
--
--	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
--	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
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
--}
--
----ui.style("star.hud-bar.mp.main") {
----	anchor = "top_center",
----	align_horizontal = "center",
----	align_vertical = "center",
----	orientation = "up",
----	size = "D, D",
----}
--
--ui.style("star.hud-bar.mp.panel") {
--	anchor = "top_center",
--	orientation = "right",
--	size = "D, D",
--	spacing = 5,
--	margin = 5,
--}
--
--ui.style("star.hud-bar.mp.btn") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	background = colors.hud_light_bg_color,
--	size = "10pxD, 4pxD",
--}
--
--ui.style("star.hud-bar.mp.input") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	background = colors.hud_light_bg_color,
--	--size = "10pxD, D1r",
--	size = "10pxD, 4pxD",
--}
--
--ui.style("star.hud-bar.mp.lbl") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	background = colors.hud_light_bg_color,
--	size = "10pxD, 4pxD",
--}
--
--ui.style("star.title-star") {
--	--ui.style("star.hud-bar.mp.btn") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
--	font_size = 18,
--	--background = "color: red",
--	--background = "color:" .. colors.hud_light_bg_color,
--	--background = "texture: atlas_ui_slate.png",
--	--background = "texture: hexagon_metal_0001_diffuse.dds",
--	--background = "pattern: hexagon_metal_0001_diffuse.dds",
--	--background = "texture: separator_bar_256x16.png",
--	--background = "texture: stretch_border.png",
--	--background = "border: stretch_border.png",
--	--background = "pattern: stretch_border.png",
--	--background = "padding_pattern: stripes_border.png",
--	--background = "padding_pattern: stripes.png",
--	--background = "padding_pattern: stripes.png",
--	--background = {type = "padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, texture = "stripes.png"},
--	--background = {type = "padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border = 3, texture = "stripes.png"},
--	--background = {type = "pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, texture = "stripes.png"},
--	--background = {type = "border_padding_pattern", color = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
--	--background = {type = "border_padding_pattern", color = {1, 1, 1, 1}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
--	--background = {type = "border_padding_pattern", color_border = {1, 1, 1, 1}, color_pattern = {1, 1, 1, 1}, inner_padding = {4, 0}, border_extent = 3, texture = "stripes_border.png"},
--	--background = {type = "border_padding_pattern", color_border = {1, 1, 1, 1}, color_pattern = {1, 1, 1, 1}, inner_padding = {4, -1}, border_extent = 3, texture = "stripes_border.png"},
--	--background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.6}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--
--	background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
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
--
--	padding = {60, 8},
--	--padding = { 0, 7},
--
--	--size = "200px, 200px",
--	--size = "100px, 100px",
--	--size = "6px D 1r, 10px D",
--	--size = "6px D 1r, 16px",
--	--size = "6px D 1r, 19px",
--
--	--size = "6px D 1r, 1px D",
--	--size = "6px D 1r, 2px D",
--	--size = "6px D 1r, 4px D",
--	--size = "6px D 1r, 8px D",
--
--	--size = "6px D 1r, D",
--	--size = "6px D 1r, 20px",
--	--size = "4pxD, D",
--	size = "10pxD, 4pxD",
--}
--
--ui.style("star.hud-bar.mp-cam.panel") {
--	anchor = "center_right",
--	orientation = "down",
--	size = "25%, 100%",
--	spacing = 5,
--	margin = 5,
--}
--
--
--
--
--
--ui.style("star.hud-bar.lobby.panel") {
--	align_horizontal = "left",
--	align_vertical = "center",
--	anchor = "center_left",
--	background = colors.hud_light_bg_color,
--	margin = 5,
--	padding = 2,
--	orientation = "down",
--	size = "D, D",
--	spacing = 2,
--}
--
--ui.style("star.hud-bar.lobby.name") {
--	align_horizontal = "left",
--	align_vertical = "center",
--	background = colors.hud_light_bg_color,
--	font_color = colors.text_black,
--	padding = {10, 4},
--	size = "D1r, D",
--}
--
--ui.style("star.hud-bar.lobby.title") {
--	align_horizontal = "center",
--	align_vertical = "center",
--	font_color = colors.text_black,
--	size = "10pxD, 4pxD",
--}
--
--ui.style("star.overlay.controls-help.lbl") {
--	align_horizontal = "left",
--	align_vertical = "center",
--	anchor = "center",
--	background = "color: rgba(0.0, 0.0, 0.0, 0.6)",
--	padding = {6, 4},
--	font_color = colors.text_light,
--
--	size = "D, D",
--}
