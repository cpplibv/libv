-- Test surface config example: noise

require("common/nodes")

-- -------------------------------------------------------------------------------------------------
local global_handover = {
	soft = 0.2,
	hard = 0.1
}

local global_takeover = {
	soft = 0.35,
	hard = 0.6
}


config = {
	--currentScene = "temperature",
	resolution = 256,
	--biomeResolution = 16, --TODO: Use fix biome resolution for LOD
	numChunks = 400,

	amplitude = 0.5,
	--treeSize = 0.003,
	--circleNumber = 50,
	--circleSize = 0.01,
	fogIntensity = 0.02,
	fogColor = "hsv(160, 70%, 75%)",
	numVeggie = 50000,
	skyboxTexture = "sky/tryo_nebula80_colorful_cube_srgb.dds",
	--skyboxTexture = "sky/qwantani_puresky_cube_hdr.dds",

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
	{
		name = "dark-blue",
		dominance = 1,
		coord = vec2f(0.3, 0.1),
		handover = global_handover,
		takeover = global_takeover,
		colorGrad = {
			--blue
			{0, "hsv(210, 70%, 30%)"},
			{1, "hsv(170, 30%, 70%)"}
		},
		vegetation = {
			{
				name = "tree",
				model = "rock",
				scale = range(1.0, 2.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(100.0, 30.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.0, 0.0),
				probability = 0.1,
			},
		}
	},
	{
		name = "green",
		coord = vec2f(1.5, 0.8),
		dominance = 1.0,
		handover = global_handover,
		takeover = global_takeover,
		colorGrad = {
			--green
			{0, "hsv(120, 60%, 30%)"},
			{1, "hsv(120, 60%, 70%)"}
		},
		vegetation = {
			{
				name = "tree",
				model = "pine",
				scale = range(0.4, 0.8),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(80.0, 15.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.00, 0.0),
				probability = 1,
			},
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
	nodes =
	add{
		coord(0.1,0.1),
		pow{
			exponent = 0.8,
			mix{

				simplexFractal {
					seed = 435,
					octaves = 6,
					amplitude = 0.5,
					frequency = 0.01,
					lacunarity = 2.0,
					persistence = 0.5,
				},
				min{
					add{
						simplexFractal {
							seed = 123,
							octaves = 6,
							amplitude = 1,
							frequency = 0.2,
							lacunarity = 2.0,
							persistence = 0.5,
						},
						constant{
							value=0.5
						}
					},
					add{
						simplexFractal {
							seed = 810,
							octaves = 6,
							amplitude = 1,
							frequency = 0.2,
							lacunarity = 2.0,
							persistence = 0.5,
						},
						constant{
							value=0.5
						}
					},
				},


				clamp{
					add{
						simplexFractal {
							seed = 810,
							octaves = 6,
							amplitude = 1,
							frequency = 0.1,
							lacunarity = 2.0,
							persistence = 0.5,
						},
						constant{
							value=0.5
						}
					},

					constant{
						value=0
					},
					constant{
						value=2
					}
				}
			}

	}

	}




	--pow {
	--	exponent = 2.0,
	--	--	}
	--	--}
	--	add {
	--		--perlin{},
	--		warp_legacy {
	--			--value = 0.5,
	--			seed = 423,
	--			octaves = 5,
	--			amplitude = 0.05,
	--			frequency = 0.25,
	--			lacunarity = 2.0,
	--			persistence = 0.5,
	--			--simplex{
	--			--	seed = 126
	--			--}
	--			simplexFractal {
	--				seed = 810,
	--				octaves = 6,
	--				amplitude = 0.4,
	--				frequency = 0.5,
	--				lacunarity = 2.0,
	--				persistence = 0.5,
	--			},
	--		},
	--		simplexFractal {
	--			seed = 1,
	--			octaves = 6,
	--			amplitude = 0.2,
	--			frequency = 0.2,
	--			lacunarity = 2.0,
	--			persistence = 0.5,
	--		},
	--		--simplexFractal {
	--		--	seed = 1000,
	--		--	octaves = 6,
	--		--	amplitude = 0.6,
	--		--	frequency = 0.2,
	--		--	lacunarity = 2.0,
	--		--	persistence = 0.5,
	--		--},
	--		constant {
	--			value = 1
	--		},
	--		--cellular {
	--		--	distanceFn = DistFun.manhattan,
	--		--	returnType = ReturnType.distance2Div,
	--		--	jitter = 1,
	--		--},
	--
	--	}
	--}
}


--heatMaps = {
temperature = {
	name = "temperature",
	heightSensitivity = 0.4, --//200m-kent 1 fok
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
	 --warp_legacy {
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
		warp_legacy {
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




