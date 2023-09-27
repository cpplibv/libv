-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

local global_handover = {
	soft = 0.4,
	hard = 0.2
}

local global_takeover = {
	soft = 0.35,
	hard = 0.6
}

config = {
	seed = 0,
	resolution = 128,
	numVeggie = 20000,

	fogIntensity = 0.015,
	fogColor = "hsv(0, 5%, 95%)";
	sunColor = "hsv(0, 15%, 95%)";
}

biomes = {
	{
		name = "grassland",
		coord = vec2f(0.2, 0.2),
		dominance = 1.0,
		handover = global_handover,
		takeover = global_takeover,

		colorGrad = {
			{ 0, "hsv(100, 40%, 80%)" },
			{ 1, "hsv(120, 70%, 40%)" }
		},
		vegetation = {
			--{
			--	name = "tree",
			--	color = "hsv(150, 65%, 70%)",
			--	size = 0.05,
			--	probability = 0.2,
			--},
			--{
			--	name = "tree",
			--	model = "rock",
			--	scale = range(2.0, 4.0),
			--	hue = shift(0.0, 0.0),
			--	saturation = shift(0.0, 0.0),
			--	value = shift(0.0, 0.0),
			--	probability = 0.05,
			--},
			{
				name = "tree",
				model = "oak",
				scale = range(1.0, 2.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.0, 0.0),
				probability = 0.5,
			},
			{
				name = "tree",
				model = "pine",
				scale = range(1.0, 2.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(60.0, 30),
				saturation = shift(0.0, 0.0),
				value = shift(-0.2, -0.2),
				probability = 0.5,
			},
		}
	},
	{
		name = "rock",
		coord = vec2f(0.4, 0.4),
		dominance = 1.0,
		handover = global_handover,
		takeover = global_takeover,

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
				name = "tree",
				model = "rock",
				scale = range(2.0, 4.0),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.1, 0.5),
				probability = 0.05,
			},
			--{
			--	name = "tree",
			--	model = "oak",
			--	scale = range(1.0, 2.0),
			--	--hue = shift(20.0, 5.0),
			--	--saturation = shift(0.0, 0.0),
			--	--value = shift(0.0, 0.04),
			--	hue = shift(0.0, 0.0),
			--	saturation = shift(0.0, 0.0),
			--	value = shift(0.0, 0.0),
			--	probability = 0.5,
			--},
		}
	},
	{
		name = "desert",
		coord = vec2f(0.9, 0.2),
		dominance = 1.0,
		handover = global_handover,
		takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(200, 80%, 100%)" }
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
				probability = 0.000,
			}
		}
	},
	{
		name = "dark",
		coord = vec2f(3.5, 0.2),
		dominance = 1.0,
		handover = global_handover,
		takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(200, 80%, 50%)" }
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
				probability = 0.000,
			}
		}
	},
}

-- =================================================================================================

-- =================================================================================================

height = {
	nodes =
	pow{
		exponent = 1.2,
		add{
			simplexFractal {
				seed = 42,
				octaves = 2,
				amplitude = 2,
				frequency = 0.025,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			simplexFractal {
				seed = 123,
				octaves = 5,
				amplitude = 0.5,
				frequency = 0.05,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			constant{
				value=1
			}
		}

	}

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
			amplitude = 0.1,
			frequency = 0.25,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant {
			value = 0.9
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
