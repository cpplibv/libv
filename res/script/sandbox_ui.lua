
local dump = require("lib.util.dump")

----------------------------------------------------------------------------------------------------

--local result = {}

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
	layout = ui.layout_line(ui.ORIENT_Orientation.right),
--	layout_orient = ui.ORIENT_Orientation.right,
	size = "1r, 20px",
}

ui.style("info_bar.left") {
	size = "dynamic, 1r",
	background = "#00FF007F",
	layout = ui.layout_line(ui.ORIENT_Orientation.right),
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
	layout = ui.layout_line(ui.ORIENT_Orientation.right),
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

--dump(result, "result")

--return ui.component("info_bar") {
--	stlye = "info_bar",
--
--	ui.component("left") {
--		stlye = "info_bar.left",
--
--		ui.component("time_control") {
--			stlye = "info_bar.time_control",
--
--			ui.component {
--				stlye = "info_bar.time_control.button",
--				-- how does style / component reads the current mode (game state), speed buttons should reflect the current speed at all time, even if the change was from outside of the UI (network, mod or script)
--				text = "||",
--				onSubmit = function (event) game.speed(0) end,
--			},
--			ui.component {
--				stlye = "info_bar.time_control.button",
--				text = ">",
--				onSubmit = function (event) game.speed(1) end,
--			},
--			ui.component {
--				stlye = "info_bar.time_control.button",
--				text = ">>",
--				onSubmit = function (event) game.speed(2) end,
--			},
--			ui.component {
--				stlye = "info_bar.time_control.button",
--				text = ">>>",
--				onSubmit = function (event) game.speed(3) end,
--			},
--		},
--
--		ui.component("time") {
--			stlye = "info_bar.time",
--
--			text = "0000.00.00 00:00",
--			text_width_min = string.len("0000.00.00 00:00"),
--		},
--
--		ui.component("balance") {
--			stlye = "info_bar.balance",
--
--			text = "0 :kredit:",
--			onUpdate = function (event) event.component:set{text = game.current_player.kredit() .. " :kredit:"} end,
----			onUpdate = function (event) event.component:set{text = "Update ran!"} end,
----			onUpdate = function (event) event.component:set{text = current_player.balance .. ":kredit:"} end,
--		},
--	},
--
--	ui.component {
--		size = "1r, 1r",
--	},
--
--	ui.component("right") {
--		stlye = "info_bar.right",
--
--		ui.component("fps") {
--			stlye = "info_bar.fps",
--
--			text = "FPS: ",
--			onUpdate = function (event) event.component:set{text = "FPS: "} end,
----			event = {
----				onUpdate = function (event)
----					local change = {}
----					if (1 / event.delta_time) < 59 then
----						change.font_color = "#FF9999FF"
----					else
----						change.font_color = "#FFFFFFFF"
----					end
----					change.text = "FPS: "
----					event.component:set{change}
----				end,
----			},
--		},
--	},
--}



return nil;




--[[

----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------


--local component_mt = {
--
--	__index = function (self, index)
--		return rawget(self, rawget(self, "__index_mapping")[index])
--	end,
--
--	__newindex = function (self, index, value)
--		local replace_index = #self + 1
--		self.__index_mapping[index] = replace_index
--		rawset(self, replace_index, value)
--		return value
--	end,
--}
--
--ui.component = function (tab)
--	tab.__index_mapping = {}
--	setmetatable(tab, component_mt)
--
--	return tab
--end

----------------------------------------------------------------------------------------------------

ui.layout_line = function () return "layout_line" end
ui.layout_float = function () return "layout_float" end

ui.ALIGN_CENTER_CENTER = "ALIGN_CENTER_CENTER"
ui.ANCHOR_BOTTOM_LEFT = "ANCHOR_BOTTOM_LEFT"
ui.ANCHOR_TOP_CENTER = "ANCHOR_TOP_CENTER"
ui.ORIENT_Orientation.right = "ORIENT_Orientation.right"

ui.component = function (tab) return tab end

----------------------------------------------------------------------------------------------------

local star_map_ui = ui.component{
	icon_set = "base",
	layout = ui.layout_float(),
	name = "star_map_ui",
	size = "1r, 1r",
}

star_map_ui:add("planets_hud") {
	size = "1r, 1r",
	layout = ui.layout_float(),
	font = "courine_new.ttf ...",
	font_size = "12px",
	font_shader = "default", -- font_shader = "outline",

--	onUpdate = function (event)
--		world.foreach({game.component.PLANET, game.component.UI}, function (planet, component)
--			component.position = magic_3D_to_2D(planet.position) - vec2f(0, -35)
--		end)
--	end,

	hive = {
		size = "dynamic, dynamic",
		font_color = planet.owner.magic_planet_name_color, -- "#FFFFFFFF",

		onCreate = function (event)
			event.component:set{text = planet.name}
		end,

		onUpdate = function (event)
			event.component:set{position = scene.camera.toScreen(planet.position) - vec3f(0, -35, 0)}
		end,
	},
}

--world.watcher(game.component.PLANET, game.ecs.CREATE, function (planet)
--	star_map_ui:get("planets_hud"):add(planet.name) {
--		size = "dynamic, dynamic",
--		font_color = planet.owner.magic_planet_name_color, -- "#FFFFFFFF",
--
--		onCreate = function (event)
--			event.component.text = planet.name
--		end,
--
--		onUpdate = function (event)
--			event.component.position = magic_3D_to_2D( planet.position ) - vec2f(0, -35)
--		end,
--	}
--end)
--
--world.watcher(game.component.PLANET, game.ecs.DESTROY, function (planet)
--	star_map_ui:get("planets_hud"):remove(planet.name)
--end)


--star_map_ui:add("star_map_ui.planets_hud", "planet_####") {
--star_map_ui.planets_hud.planet[i] = ui.component{
--star_map_ui.planets_hud:child{ name = "planet_####", -- #### is placeholder for an auto incremental integer
--	size = "dynamic, dynamic",
----	font_color = magic color of owner -- "#FFFFFFFF",
----	text = magic -- ui.constraint(function (object) return object.name end),
----	position = magic (3D_to_2D - vec2f(0, -35)) -- ui.constraint(function (object) return object.position end),
--}

star_map_ui:add("planet_info_selected") {
	size = "360px, 160px",
	anchor = ui.ANCHOR_BOTTOM_LEFT,
	text = "<< Planet info goes here >>",
	text_align = ui.ALIGN_CENTER_CENTER
}

star_map_ui:add("info_bar_top") {
	size = "1r, 20px",
	anchor = ui.ANCHOR_TOP_CENTER,
	background = "image res/ui/star_map/info_bar.dds",
	layout = ui.layout_line(ui.ORIENT_Orientation.right),
	font = "courine_new.ttf",
--	font = "courine_new.ttf ...",
	font_size = "12px",
	font_shader = "default",
	font_color = "#FFFFFFFF",

	ui.component("left") {
		size = "dynamic, 1r",
		background = "#00FF007F",
		layout = ui.layout_line(ui.ORIENT_Orientation.right),

		ui.component {
			name = "balance",
			size = "dynamic, 1r",
			text = "",
			onUpdate = function (event) event.component:set{text = "Update ran!"} end,
--			onUpdate = function (event) event.component:set{text = current_player.balance .. ":kredit:"} end,
		},
	},

	ui.component {
		size = "1r, 1r",
	},

	ui.component("right") {
		size = "dynamic, 1r",
		ui.component{
			name = "fps",
			size = "100px, 1r",
			--size = "max(dynamic, 100px), 1r",
			text = "FPS: ",
			onUpdate = function (event) event.component:set{text = "FPS: "} end,
--			onUpdate = function (event) event.component:set{text = "FPS: " .. (1 / event.delta_time)} end,
--			onUpdate = function (event)
--				local change = {}
--				if (1 / event.delta_time) < 59 then
--					change.font_color = "#FF9999FF"
--				else
--					change.font_color = "#FFFFFFFF"
--				end
--				change.text = "FPS: "
--				event.component:set{change}
--			end,

--			event = {
--				onUpdate = function (event)
--					local change = {}
--					if (1 / event.delta_time) < 59 then
--						change.font_color = "#FF9999FF"
--					else
--						change.font_color = "#FFFFFFFF"
--					end
--					change.text = "FPS: "
--					event.component:set{change}
--				end,
--			},
		},
	},
}

dump(star_map_ui, "star_map_ui")
return star_map_ui

]]
