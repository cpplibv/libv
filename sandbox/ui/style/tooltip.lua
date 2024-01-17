-- -------------------------------------------------------------------------------------------------

ui.style("tooltip.tooltip01") {
	--background = "#66F8",
	background = "#F668",
	--padding = 5,
	size = "D, D",
	--size = "100px D, D",
	--align = "left",
}

ui.style("tooltip.card") {
	background = "#0608",
	align = "center",
	align_vertical = "center",
	orientation = "down",
	--align = "left",
}

ui.style("tooltip.line") {
	background = "#6008",
	align = "center",
	align_vertical = "center",
	--align = "left",
}

ui.style("tooltip.button1") {
	background = "#66F8",
	--background = "#66F8",
	--padding = 5,
	size = "D, D",
	--size = "100px D, D",
	--align = "left",
	--	state("focus") {
	--		background = {type = "border_padding_pattern", color_border = {0.8, 0.8, 0.8, 0.8}, color_pattern = {0.8, 0.8, 0.8, 0.4}, inner_padding = {4, -3}, border_extent = 3, texture = "stripes_border.png"},
	--	},
	state("hover") {
		background = "hsv(120, 20%, 70%, 60%)",
		--bar_visual = {type = "border", texture = "stretch_border.png"},
		--background = {type = "border", texture = "stretch_border.png"},
	},
}
