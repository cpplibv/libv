-- Test surface config example: Nova

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
	seed = 0,
	resolution = 128,
	--numChunks = 9,
	numChunks = 81,
	--numChunks = 169,
	--numChunks = 441,
	--numVeggie = 150,
	--numVeggie = 500,
	numVeggie = 0,

	fogIntensity = 0.02,
	fogColor = "hsv(120, 5%, 95%)";
}

biomes = {
	{
		name = "grassland",
		coord = vec2f(0.2, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(100, 40%, 80%)" },
			{ 1, "hsv(120, 30%, 80%)" }
		},
		vegetation = {
			--{
			--	name = "tree",
			--	color = "hsv(150, 65%, 70%)",
			--	size = 0.05,
			--	probability = 0.2,
			--},
			{
				name = "bush",
				scale = range(2.0, 4.0),
				hue = shift(-60.0, 10.0),
				saturation = shift(0.2, 0.1),
				value = shift(0.1, 0.1),
				probability = 0.03,
			},
			{
				name = "tree",
				scale = range(1.0, 3.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.0, 0.0),
				probability = 1.0,
			},
		}
	},
	{
		name = "desert",
		coord = vec2f(0.9, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(34, 14%, 100%)" }
		},
		vegetation = {
			--{
			--	name = "tree",
			--	color = "hsv(60, 80%, 100%)",
			--	size = 0.05,
			--	probability = 0.2,
			--	--fertilityRange = vec2f{0.6, 1.0},
			--	--color = "hsv(120, 60%, 100%)",
			--},
			{
				name = "rock",
				color = "hsv(69, 10%, 60%)",
				size = 0.022,
				probability = 0.002,
			}
		}
	},
}

-- =================================================================================================

-- =================================================================================================

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
	--	--{key = -2.0 ,value =  "blue"},
	--	--{key = 0.0 , value = "yellow"},
	--	--{key = 1.0 , value = "green"},
	--	--{key = 2.0 , value = "grey"},
	--	--[-2.0] = "blue",
	--	--[0.0] = "yellow",
	--	--[1.0] = "green",
	--	--[2.0] = "grey",
	},
	nodes =
		simplexFractal {
			seed = 42,
			octaves = 2,
			amplitude = 1,
			frequency = 0.025,
			lacunarity = 2.0,
			persistence = 0.5,
		},
}

temperature = {
	name = "temperature",
	heightSensitivity = 2, --//200m-kent 1 fok
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes = add {
		simplexFractal {
			seed = 1,
			octaves = 60,
			amplitude = 0.3,
			frequency = 0.25,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant {
			value = 0.1
		}
	}
}

humidity = {
	name = "humidity",
	--heightSensitivity = 0.2,
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "blue" }
	},
	nodes = --add {
	add {
		--simplexFractal {
		--	seed = 103421,
		--
		--	octaves = 6,
		--	amplitude = 0.5,
		--	frequency = 0.5,
		--	lacunarity = 2.0,
		--	persistence = 0.5,
		--},
		--cellular {
		--	seed = 123,
		--	distanceFn = DistFun.euclidean,
		--	--distanceFn = DistFun.manhattan,
		--	returnType = ReturnType.cellValue,
		--	--returnType = ReturnType.distance,
		--	jitter = 1,
		--},
		constant {
			value = 0.7
		},
	}
}

fertility = {
	name = "fertility",
	heightSensitivity = 0.2,
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "green" }
	},
	nodes = add {
		warp {
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
