-- -------------------------------------------------------------------------------------------------

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
