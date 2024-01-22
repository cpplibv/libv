-- -------------------------------------------------------------------------------------------------

local globals = import "style/globals"

import "style/surface"
import "style/starmap"

-- === Info bar ============================================================================================

ui.style("game.info-bar") {
	font = globals.fonts.label,
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
}

ui.style("game.info-bar.panel") {
	font = globals.fonts.label,
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
	globals.stripped_background_interactive,
	-- debug_background,
}

ui.style("game.info-bar.time.speed") {
	align_horizontal = "center",
	align_vertical = "center",
	size = "40px, D1r",
	padding = 2,
	focus_mode = "passive",
	globals.stripped_background_interactive,
	-- debug_background,
}

-- Menu --------------------------------------------------------------------------------------------

ui.style("game.menu") {
	font = globals.fonts.label,
}

ui.style("game.menu.line") {
	anchor = "bottom_center",
	orientation = "down",
	size = "D, D",
	padding = 8,
	spacing = 4,
	background = "#0006",
}

ui.style("game.menu.label") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	caret_color = "rgba(1.0, 1.0, 1.0, 1.0)", -- Debug only, remove (input should have label style, but that is a debug input)
	padding = {16, 2},
	size = "1rD, D",
	globals.stripped_background,
}

ui.style("game.menu.button") {
	align_horizontal = "center",
	align_vertical = "center",
	font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
	padding = {16, 2},
	size = "1rD, D",
	globals.stripped_background_interactive,
}


