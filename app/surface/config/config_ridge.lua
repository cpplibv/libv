-- Test surface config example: Nova

require("config/common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 0,
	resolution = 256,
	--numVeggie = 500,
	numVeggie = 20000,

	fogIntensity = 0.015,
	fogColor = "rgb(0.7, 0.8, 0.9)";

	sunColor = "hsv(0, 0%, 150%)";
	sunDirection = vec3f(0.8, 0.2, 0.6),

	skyboxTexture = "cube_fluffball.dds",
}

biomes = {
	{
		name = "snow",
		coord = vec2f(-3 + -12, 0),
		dominance = 1.0,
		handover = { soft = 0.3, hard = 0.0 },
		takeover = { soft = 0.8, hard = 0.9 },
		colorGrad = {
			{ 0, "hsv(210, 10%, 98%)" },
			--{ 1, "hsv(215, 0%, 100%)" }
			{ 1, vec3f(2, 2, 2) }
		},
		vegetation = {
		}
	},
	{
		name = "mountain",
		coord = vec2f(-3 + -6, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(210, 10%, 98%)" },
			{ 1, "hsv(215, 10%, 92%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "rock",
				scale = range(1.0, 5.0),
				hue = shift(160.0, 25.0),
				saturation = shift(-0.2, 0.1),
				value = shift(0.4, 0.2),
				probability = 0.005,
			}
		}
	},
	{
		name = "forest_pine",
		coord = vec2f(-3 + -3, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "hsv(90, 70%, 40%)" }
		},
		vegetation = {
			{
				name = "tree",
				model = "pine",
				scale = range(0.5, 2.0),
				saturation = shift(-0.3, 0.1),
				probability = 1.0,
			},
		}
	},
	{
		name = "forest",
		coord = vec2f(-3 + 0, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "hsv(90, 70%, 40%)" }
		},
		vegetation = {
			{
				name = "tree",
				model = "oak",
				scale = range(1.0, 3.0),
				value = shift(0.1, 0.05),
				probability = 1.0,
			},
		}
	},
	{
		name = "grassland",
		coord = vec2f(-3 + 3, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "hsv(90, 40%, 80%)" }
		},
		vegetation = {
			{
				name = "tree",
				model = "oak",
				scale = range(1.0, 3.0),
				value = shift(0.3, 0.1),
				probability = 0.1,
			},
		}
	},
	{
		name = "desert",
		coord = vec2f(-3 + 9, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(34, 54%, 92%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "rock",
				probability = 0.05,
			}
		}
	},
}

height = {
	--nodes = constant(0.0),

	nodes =
		add {
			coord(-0.5, 0),
			simplexFractal {
				seed = 1001,
				octaves = 4,
				amplitude = 0.75,
				frequency = 0.03,
				lacunarity = 2.0,
				persistence = 0.5,
			},
		},


		--max {
		--	add {
		--		coord(-0.15, 0),
		--		simplexFractal {
		--			seed = 600733745,
		--			octaves = 4,
		--			amplitude = 0.75,
		--			frequency = 0.03,
		--			lacunarity = 2.0,
		--			persistence = 0.5,
		--		},
		--	},
		--}
		--warp {
		--	seed = 98001,
		--	octaves = 3,
		--	amplitude = 50.0,
		--	frequency = 0.02,
		--	lacunarity = 2.0,
		--	persistence = 0.25,
		--	--frequency = 0.015,
		--	--persistence = 0.5,
		--
		--	mul {
		--		constant(4.0),
		--		pow {
		--			exponent = 2.0,
		--			add {
		--				simplexFractal {
		--					seed = 600733745,
		--					octaves = 4,
		--					amplitude = 0.75,
		--					frequency = 0.01,
		--					lacunarity = 2.0,
		--					persistence = 0.5,
		--				},
		--			},
		--		},
		--	},
		--}
}

temperature = {
	heightSensitivity = 0.4,
	--nodes = constant(0.0),
	--nodes = coord(0.1, 0.0),
	nodes = add {
		simplexFractal {
			seed = 1005,
			octaves = 3,
			amplitude = 0.2,
			frequency = 0.1,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		--constant {
		--	value = 0.5
		--}
	}
}

humidity = {
	nodes = constant(0.0),

	--nodes =
	--	mul{
	--		constant(10.0),
	--		sin {
	--			nodes = coord(0.0, 0.01),
	--		},
	--	},

	--nodes =
	--	add {
	--		simplexFractal {
	--			seed = 143421,
	--
	--			octaves = 3,
	--			amplitude = 0.45,
	--			frequency = 0.02,
	--			lacunarity = 2.0,
	--			persistence = 0.5,
	--		},
	--		simplexFractal {
	--			seed = 143422,
	--
	--			octaves = 3,
	--			amplitude = 0.45,
	--			frequency = 0.02,
	--			lacunarity = 2.0,
	--			persistence = 0.5,
	--		},
	--		--constant {
	--		--	value = -0.5
	--		--},
	--	}
}

fertility = {
	nodes = constant(0.0),
}
