-- Test surface config example: noise

require("config/common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	resolution = 256,
	--biomeResolution = 16, --TODO: Use fix biome resolution for LOD
	numChunks = 400,

	amplitude = 0.5,
	numVeggie = 100,
}


biomes = {
	{
		name = "dark-blue",
		dominance = 1,
		coord = vec2f(0.1, 0.9),
		cutOff = vec2f(0.3, 0.9),
		colorGrad = {
			--blue
			{0, "hsv(210, 70%, 30%)"},
			{1, "hsv(210, 70%, 50%)"}
		},
		vegetation = {
			{
				name = "tree",
				color = "hsv(210, 70%, 70%)",
				size = 0.03,
				probability = 0.2,
			},
			{
				name = "bush",
				color = "hsv(219, 70%, 65%)",
				size = 0.02,
				probability = 0.2,
			}
		}
	},
	{
		name = "green",
		coord = vec2f(0.5, 0.8),
		dominance = 1.0,
		cutOff = vec2f(0.45, 0.5),
		colorGrad = {
			--green
			{0, "hsv(120, 60%, 30%)"},
			{1, "hsv(120, 60%, 100%)"}
		},
		vegetation = {
			{
				name = "tree",
				color = "hsv(125, 60%, 100%)",
				size = 0.025,
				probability = 0.2,
			},
			{
				name = "bush",
				color = "hsv(129, 60%, 100%)",
				size = 0.022,
				probability = 0.2,
			}
		}
	},
}

height = {
	name = "height",
	heightSensitivity = 0.0,
	colorGrad = {
		{ -1.0, "hsv(240, 80%, 20%)" },
		{ -0.01, "hsv(240, 80%, 60%)" },
		{ 0.6, "hsv(120, 60%, 60%)" },
		{ 1.0, "hsv(220, 5%, 95%)" },
	},
	nodes =
	smoothstep {
		coord(1,0.0),
		constant {
			value = 0
		},
		constant {
			value = 2
		},
	}
}


--heatMaps = {
temperature = {
	name = "temperature",
	heightSensitivity = 5, --//200m-kent 1 fok
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes =

 add {
	 simplexFractal {
		 seed = 1,
		 octaves = 6,
		 amplitude = 0.5,
		 frequency = 0.5,
		 lacunarity = 2.0,
		 persistence = 0.5,
	 },
	 --warp{
		-- seed = 423,
		-- octaves = 5,
		-- amplitude = 4,
		-- frequency = 0.25,
		-- lacunarity = 2.5,
		-- persistence = 0.5,
		-- cellular {
		--	 seed = 123,
		--	 distanceFn = DistFun.euclidean,
		--	 returnType = ReturnType.cellValue,
		--	 jitter = 1,
		-- },
	 --},

		constant {
			value = 0.5
		}
	}
}
humidity = {
	name = "humidity",
	--heightSensitivity = 0.2,
	colorGrad = {
		{0.0 , "white"},
		{1.0 , "blue"}
	},
	nodes =

	--add {
	--	mul {
	--	perlin {
	--		seed = 10,
	--
	--	--	--octaves = 6,
	--	--	--amplitude = 0.5,
	--	--	--frequency = 0.5,
	--	--	--lacunarity = 2.0,
	--	--	--persistence = 0.5,
	--	},
	--		constant{
	--			value = 1
	--		}
	--	},
	----cellular {
	----	seed = 123,
	----	distanceFn = DistFun.euclidean,
	----	returnType = ReturnType.cellValue,
	----	jitter = 1,
	----},
	--	constant{
	--		value = 0.5
	--	}
	--}
	add {
		simplexFractal {
			seed = 103421,

			octaves = 6,
			amplitude = 0.5,
			frequency = 0.5,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		--cellular {
		--	seed = 123,
		--	distanceFn = DistFun.euclidean,
		--	--distanceFn = DistFun.manhattan,
		--	returnType = ReturnType.cellValue,
		--	--returnType = ReturnType.distance,
		--	jitter = 1,
		--},
		constant{
			value = 0.5
		},
	}


}

fertility =	{
	name = "fertility",
	heightSensitivity = 0.2,
	colorGrad = {
		{0.0 , "white"},
		{1.0 , "green"}
	},
	nodes =
	add{
		warp{
			seed = 1233,
			octaves = 2,
			amplitude = 1,
			frequency = 0.5,
			lacunarity = 2,
			persistence = 0.5,
			cellular {
				seed = 14523,
				distanceFn = DistFun.euclidean,
				returnType = ReturnType.cellValue,
				jitter = 1,
			},
		},
		constant {
			value = 0.5
		},
	}


}
	--hardness = {
	--	...
	--}
--}

--option 3
--NodeTree = {
--	node_type = "add",
--	{
--		node_type = "warp",
--		warp_value = 0.5,
--		input = {
--			node_type = "perlin"
--			--seed,
--			--args,
--		}
--	},
--	{
--		node_type = "perlin"
--		--return_value = cell.distance0,
--	}
--}




