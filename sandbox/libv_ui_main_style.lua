-- -------------------------------------------------------------------------------------------------































--- Scroll Card ------------------------------------------------------------------------------------

ui.style("tabs.scroll.pane") {
	background = "hsv(0, 70%, 100%, 40%)",
	size = "50%, 90%",

	spacing2 = {5, 5},
	--spacing2 = 0,
	--scroll_mode = "vertical",
	scroll_mode = "both",

	nest("area") {
		--padding = {50, 10, 4, 2},
		padding = {50, 10},
		background = "hsv(60, 70%, 100%, 40%)",
		--size = "1r, 1r",
	},
	nest("vbar") {
		bar_visual = "hsv(120, 100%, 100%, 40%)",
		background = "hsv(120, 70%, 100%, 40%)",
		state("hover") {
			bar_visual = "hsv(120, 100%, 100%, 60%)",
			background = "hsv(120, 70%, 100%, 60%)",
			--bar_visual = {type = "border", texture = "stretch_border.png"},
			--background = {type = "border", texture = "stretch_border.png"},
		},
		size = "25px, 1r",
	},
	nest("hbar") {
		bar_visual = "hsv(180, 100%, 100%, 40%)",
		background = "hsv(180, 70%, 100%, 40%)",
		size = "1r, 25px",
		--size = "200px, 25px", -- This has a BUG
	},
}

ui.style("tabs.scroll.line") {
	anchor = "top-left",
	background = "#6F68",
	orientation = "down",
	size = "100%, D",
	--size = "100%, 100%",
	--padding = 5,
	margin = 5,
	spacing = 5,
}

ui.style("tabs.scroll.lbl") {
	background = "#66F8",
	padding = 5,
	size = "D, D",
	--size = "100px D, D",
	--align = "left",
}


--ui.style("main-menu.menu.entry") {
--	size = "1rD, D",
--	align_horizontal = "center",
--	align_vertical = "center",
--	padding = {40, 5},
--	font_size = 22,
--	font_color = {1, 1, 1, 1},
--
--	--background = "hsva(0.0, 0.0, 5%, 1.0)",
--	--background = {type = "border", color = {0.8, 0.8, 0.8, 0.8}, texture = "light_border.png"},
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
--		font_color = {0.5, 0.5, 0.5, 1},
--		background = {type = "border_padding_pattern", color_border = {0.4, 0.4, 0.4, 0.6}, color_pattern = {0.4, 0.4, 0.4, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
--	},
--}
