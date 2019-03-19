
local dump = require("lib.util.dump")
local ui = {}

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
ui.ORIENT_LEFT_TO_RIGHT = "ORIENT_LEFT_TO_RIGHT"

ui.component = function (tab) return tab end

----------------------------------------------------------------------------------------------------

-- Forced
--	enable/disable
--	show/hide

-- Non-Inheriting
--	name
--	size
--	background
--	layout
--	onEvent

-- Overrideable
--	font
--	font_size
--	font_shader
--	font_color
--	icon_enable

-- Additive
--	icon_set

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
	layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),
	font = "courine_new.ttf",
--	font = "courine_new.ttf ...",
	font_size = "12px",
	font_shader = "default",
	font_color = "#FFFFFFFF",

	ui.component("left") {
		size = "dynamic, 1r",
		background = "#00FF007F",
		layout = ui.layout_line(ui.ORIENT_LEFT_TO_RIGHT),

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
