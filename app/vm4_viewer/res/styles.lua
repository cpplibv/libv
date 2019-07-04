

local colors = {
	research = "hsva(240°, 40%, 100%, 100%)",
	kredit   = "hsva(  0°, 40%, 100%, 100%)",
	text     = "hsva(  0°,  0%,  80%, 100%)",
	fps_low  = "hsva( 11°, 56%, 100%, 100%)",
	fps_hold = "hsva( 56°, 56%, 100%, 100%)",
	fps_high = "hsva(110°, 56%, 100%, 100%)",
--	fps_high = ui.color_hsv(110, 56, 100, 100),
--	fps_halt = fps_hold.adjust_hsv(15),
}

ui.style("info_bar") {
--	align_vertical = "center",
	anchor = ui.ANCHOR_TOP_CENTER,
	background = "#00FF007F",
--	background = ui.image("ui/star_map/info_bar.dds"),
--	background = ui.stretch("ui/star_map/info_bar.dds", 0, 0, 15, 15, 30, 30, 45, 45),
	font = "consola.ttf",
	font_color = colors.text,
	font_shader = "default",
	font_size = 14,
	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
--	layout_orient = ui.ORIENT_LEFT_TO_RIGHT,
	size = "1r, 20px",
}

ui.style("info_bar.left") {
	size = "dynamic, 1r",
	background = "#00FF007F",
	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
}

ui.style("info_bar.balance") {
	align = "right",
	font_color = colors.kredit,
	size = "100px, 1r",
}

ui.style("info_bar.time_control") {
--	background = "image stretch_border.png",
}

ui.style("info_bar.time_control.button") {
	size = "25px, 1r",
	align = "center",
}

ui.style("info_bar.right") {
	size = "dynamic, 1r",
	background = "#00FF007F",
	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
}

ui.style("info_bar.fps") {
	size = "100px, 1r",
--	font_color = function (text)
--		if (global.metrics.frame_delivery_rate > 0.99) then
--			return colors.fps_high,
--		elseif (global.metrics.frame_delivery_rate > 0.95) then
--			return colors.fps_hold,
--		else
--			return colors.fps_low,
--		end
--	end
}
