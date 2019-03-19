
local dump = require("lib.util.dump")
local vm4 = require("lib.vm4.import")

----------------------------------------------------------------------------------------------------

--local engine = require("engine")
--local config = require("config")

local engine = {
	shader = {
		ship_hull = "ship_hull",
		metal = "metal",
		emission = "emission",
	},
}

local config = {
	path = {
		model_source = "res/source_model",
	},
}

----------------------------------------------------------------------------------------------------

local model = vm4.import(config.path.model_source .. "ship_cargo_02_situla.####.ase")

---

--model.material["body"] = vm4.material{
--	render = engine.shader.ship_hull,
--	diffuse    = "texture/ship/ship_cargo_02_situla_diffuse.dds",
--	specular   = "texture/ship/ship_cargo_02_situla_specular.dds",
--	ambient    = "texture/ship/ship_cargo_02_situla_ambient.dds",
--	normal     = "texture/ship/ship_cargo_02_situla_normal.dds",
--	emission   = "texture/ship/ship_cargo_02_situla_emission.dds",
--    opacity    = 1,
--    shininess  = 64,
--}
--
--model.material["blue_coil"] = vm4.material{
--	render = engine.shader.metal,
--	diffuse  = "texture/ship/ship_cargo_02_situla_diffuse.dds",
--	specular = "texture/ship/ship_cargo_02_situla_specular.dds",
--	ambient  = "texture/ship/ship_cargo_02_situla_ambient.dds",
--	normal   = "texture/ship/ship_cargo_02_situla_normal.dds",
--}
--
--model.material["blinker"] = vm4.material{
--	render = engine.shader.emission,
--	emission = "texture/ship/ship_cargo_02_situla_emission.dds",
--}

---

--model.node["engine"] = vm4.node{
--	position = ,
--}

---

dump(model, "model")
