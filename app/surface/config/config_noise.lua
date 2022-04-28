-- Test surface config example: noise

require("config/common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	--currentScene = "temperature",
	resolution = 128,
	--biomeResolution = 16, --TODO: Use fix biome resolution for LOD
	numChunks = 400,

	amplitude = 0.5,
	--treeSize = 0.003,
	--circleNumber = 50,
	--circleSize = 0.01,
	numVeggie = 100,

--                      /\
--  COLD|BARREN        /  \ Polar
--                    /    \
--      ^            /      \ Subpolar
--      |           /        \
--      |          /          \ Temperate
--                /            \
--  HOT|FERTILE  /              \ Tropical
--               ================
--  HUMID|WET|FERTILE   -->   ARID|DRY|BARREN

	objects = {
		--asd = 12,
		{
			type = "tropical rain forest - tree",
			size = 0.02,
			count = 60,
			color = "hsv(120, 70%, 75%)",
		},
		{
			type = "bush",
			size = 0.012,
			count = 30,
			color= "hsv(160, 70%, 75%)",
		},
		{
			type = "rock",
			size = 0.008,
			count = 20,
			color = "grey",
		},
		--{
		--	--type = "boulder",
		--	type = "rock",
		--	size = 0.005,
		--	count = 300,
		--	color = "hsv(160, 10%, 60%)",
		--},
	},

}

--BiomeZone = {
--	tropical = 0,
--	temperate = 1,
--	subpolar = 2,
--	polar = 3,
--};

--not regions, biomes-forest types
biomes = {
	--{
	--	name = "no-biome",
	--	name = "polar",
	--	name = "subpolar-boreal",
	--	name = "temperate-rainforest",
	--	name = "temperate-forest",
	--	name = "temperate-grassland",
	--	name = "tropical-savanna",
	--{
	--	name = "red",
	--	dominance = 1.0,
	--	coord = vec2f(0.3, 0.3),
	--	cutOff = vec2f(0.3, 0.6),
	--	colorGrad = {
	--		--red
	--		{0, "hsv(20, 100%, 30%)"},
	--		{1, "hsv(20, 100%, 80%)"}
	--	},
	--	vegetation = {
	--		{
	--			name = "tree",
	--			color = "hsv(25, 100%, 100%)",
	--			size = 0.03,
	--			probability = 0.2,
	--		},
	--		{
	--			name = "bush",
	--			color = "hsv(30, 100%, 100%)",
	--			size = 0.02,
	--			probability = 0.2,
	--		}
	--	}
	--},
	--{
	--	name = "blue",
	--	dominance = 1.0,
	--	coord = vec2f(0.6, 0.6),
	--	cutOff = vec2f(0.3, 0.6),
	--	colorGrad = {
	--		--blue
	--		{0, "hsv(210, 70%, 30%)"},
	--		{1, "hsv(210, 70%, 100%)"}
	--	},
	--	vegetation = {
	--		{
	--			name = "tree",
	--			color = "hsv(210, 70%, 100%)",
	--			size = 0.03,
	--			probability = 0.2,
	--		},
	--		{
	--			name = "bush",
	--			color = "hsv(219, 70%, 100%)",
	--			size = 0.02,
	--			probability = 0.2,
	--		}
	--	}
	--},
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
	--{
	--
	--	name = "desert",
	--	coord = vec2f(0.9, 0.1),
	--	dominance = 1.0,
	--	--radius = 0.025
	--	--min cut off, max cut off
	--	cutOff = vec2f(0.3, 0.6),
	--	colorGrad = {
	--		--yellow
	--		{0, "hsv(60, 80%, 30%)"},
	--		{1, "hsv(60, 80%, 100%)"}
	--	},
	--	vegetation = {
	--		{
	--			name = "tree",
	--			color = "hsv(60, 80%, 100%)",
	--			size = 0.025,
	--			probability = 0.2,
	--			--fertilityRange = vec2f{0.6, 1.0},
	--			--color = "hsv(120, 60%, 100%)",
	--
	--		},
	--		{
	--			name = "bush",
	--			color = "hsv(69, 80%, 100%)",
	--			size = 0.022,
	--			probability = 0.2,
	--		}
	--	}
	--},
}

height = {
	name = "height",
	heightSensitivity = 0.0,
	colorGrad = {
		{ -1.0, "hsv(240, 80%, 20%)" },
		{ -0.01, "hsv(240, 80%, 60%)" },
		--{0.0 , "hsv(240, 70%, 99%)"},
		{ 0.0, "hsv(220, 80%, 60%)" },
		{ 0.01, "hsv(60, 80%, 90%)" },
		{ 0.05, "hsv(120, 70%, 75%)" },
		{ 0.1, "hsv(120, 100%, 50%)" },
		{ 0.5, "hsv(120, 100%, 60%)" },
		{ 0.6, "hsv(120, 60%, 60%)" },
		{ 1.0, "hsv(220, 5%, 95%)" },
		--{key = -2.0 ,value =  "blue"},
		--{key = 0.0 , value = "yellow"},
		--{key = 1.0 , value = "green"},
		--{key = 2.0 , value = "grey"},
		--[-2.0] = "blue",
		--[0.0] = "yellow",
		--[1.0] = "green",
		--[2.0] = "grey",
	},
	nodes = pow {
		exponent = 2.0,
		--	}
		--}
		add {
			--perlin{},
			warp {
				--value = 0.5,
				seed = 423,
				octaves = 5,
				amplitude = 0.05,
				frequency = 0.25,
				lacunarity = 2.0,
				persistence = 0.5,
				--simplex{
				--	seed = 126
				--}
				simplexFractal {
					seed = 810,
					octaves = 6,
					amplitude = 0.4,
					frequency = 0.5,
					lacunarity = 2.0,
					persistence = 0.5,
				},
			},
			simplexFractal {
				seed = 1,
				octaves = 6,
				amplitude = 0.2,
				frequency = 0.2,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			--simplexFractal {
			--	seed = 1000,
			--	octaves = 6,
			--	amplitude = 0.6,
			--	frequency = 0.2,
			--	lacunarity = 2.0,
			--	persistence = 0.5,
			--},
			constant {
				value = 0.2
			},
			--cellular {
			--	distanceFn = DistFun.manhattan,
			--	returnType = ReturnType.distance2Div,
			--	jitter = 1,
			--},

		}
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




