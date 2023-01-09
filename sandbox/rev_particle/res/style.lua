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

-- === Overlay =====================================================================================

ui.style("overlay.fps") {
	font = fonts.mono,
	align_horizontal = "left",
	align_vertical = "bottom",
	margin = {4, 2},
	font_color = {1, 1, 1, 0.8},
}

ui.style("overlay.version") {
	font = fonts.mono,
	font_size = 12,
	align_horizontal = "right",
	align_vertical = "bottom",
	margin = {4, 2},
	font_color = {1, 1, 1, 0.8},
}

ui.style("overlay.shader.line") {
	font = fonts.mono,
	font_size = 12,
	anchor = "top_left",
	size = "D, D",
	align_horizontal = "left",
	align_vertical = "top",
	orientation = "down",
	spacing = 8,
	margin = 8,
}

ui.style("overlay.shader.message") {
	size = "1rD, D",
	align_horizontal = "left",
	align_vertical = "bottom",
	padding = {8, 4},
}

ui.style("overlay.shader.message.success") {
	background = "hsva(120°, 20%, 23%, 0.75)",
	font_color = "rgba(0.7235, 0.9333, 0.2433, 1.0)",
}

ui.style("overlay.shader.message.failure") {
	background = "hsva(0°, 25%, 20%, 0.75)",
	font_color = "rgba(0.9333, 0.8235, 0.0078, 1.0)", -- Warning yellow
}
