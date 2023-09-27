-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

local global_handover = function()
	return {
		soft = 0.4,
		hard = 0.1
	}
end

local global_takeover = function()
	return {
		soft = 0.6,
		hard = 0.8
	}
end

config = {
	seed = 123,
	resolution = 256,
	--numChunks = 1,
	--numChunks = 9,
	numChunks = 81,
	--numChunks = 169,
	numVeggie = 20000,
	--numVeggie = 500,
	--numVeggie = 1000,

	fogIntensity = 0.05,
	fogColor = "#75bfff";
}

biomes = {
	{
		name = "red",
		coord = vec2f(0.8, 0.0),
		dominance = 1.0,
		handover = global_handover(),
		takeover = global_takeover(),
		colorGrad = {
			{ 0, "hsv(210, 10%, 98%)" },
			{ 1, "#80302a" }
		},
		vegetation = {
			{
				name = "tree",
				model = "pine",
				probability = 0.2,
				--path = "resource/tundra_tree.xyz",
				scale = range(0.5, 2.0),
				hue = shift(0, 0), --0-360
				saturation = shift(0.8, 0.1), --0-1
				value = shift(0.8, 0.1), --0-1
			},
			{
				name = "oak",
				probability = 0.2,
				path = "resource/tundra_bush.xyz",
				scale = range(0.5, 2.0),
				hue = shift(0, 0), --0-360
				saturation = shift(0.8,0.1), --0-1
				value = shift(0.8,0.1), --0-1
			},
		}
	},
	{
		name = "grassland",
		coord = vec2f(0.6, 0.0),
		dominance = 1.0,
		handover = global_handover(),
		takeover = global_takeover(),
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "hsv(121, 70%, 45%)" }
		},
		vegetation = {
			{
				name = "oak",
				probability = 0.2,
				path = "resource/tundra_tree.xyz",
				scale = range(0.5, 2.0),
				hue = shift(100, 10), --0-360
				saturation = shift(0.7,0.1), --0-1
				value = shift(0.6,0.1), --0-1
			},
			--{
			--	name = "bush",
			--	probability = 0.2,
			--	path = "resource/tundra_bush.xyz",
			--	scale = range(0.5, 2.0),
			--	hue = shift(110, 30), --0-360
			--	saturation = shift(0.75,0.1), --0-1
			--	value = shift(0.75,0.1), --0-1
			--},
		}
	},
	{
		name = "rocks",
		coord = vec2f(1.0, 0.0),
		dominance = 1.0,
		handover = global_handover(),
		takeover = global_takeover(),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(240, 70%, 60%)" }
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
				model = "rock",

				probability = 0.8,
				path = "resource/tundra_rock.xyz",
				scale = range(0.5, 2.0),
				hue = shift(240, 10), --0-360
				saturation = shift(0.5,0.1), --0-1
				value = shift(0.9,0.1), --0-1
			}
		}
	},
	{
		name = "alps",
		coord = vec2f(0.4, 0.0),
		dominance = 1.0,
		handover = global_handover(),
		takeover = global_takeover(),
		colorGrad = {
			{ 0, "#f0f3f7" },
			{ 1, "hsv(40, 0%, 90%)" }
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
				model = "rock",
				probability = 0.8,
				path = "resource/tundra_rock.xyz",
				scale = range(0.5, 2.0),
				hue = shift(300, 10), --0-360
				saturation = shift(0.1,0.1), --0-1
				value = shift(0.4,0.1), --0-1
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
	nodes = pow {
		exponent = 2.0,
		--exponent = 1.0,
		add {
			warp_legacy {
				--value = 0.5,
				seed = 423,
				octaves = 6,
				amplitude = 3.5,
				frequency = 0.5,
				lacunarity = 2.0,
				persistence = 0.25,
				--simplex{
				--	seed = 126
				--}
				simplexFractal {
					seed = 600733745,
					octaves = 5,
					amplitude = 0.6,
					frequency = 0.09,
					lacunarity = 2.0,
					persistence = 0.5,
				},
			},
			fractal {
				seed = 2,
				octaves = 10,
				amplitude = 0.4,
				frequency = 0.2,
				lacunarity = 2.0,
				persistence = 0.5,
				simplex{
					seed=123
				}
			},
			--constant {
			--	value = 0.5,
			--},
		},
	},

	--nodes = pow {
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
	--			amplitude = 0.4,
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
	--			value = 0.2
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

temperature = {
	name = "temperature",
	heightSensitivity = 0.4, --//200m-kent 1 fok
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes = saturate {
		add {

			simplexFractal {
				seed = 1,
				octaves = 6,
				amplitude = 0.2,
				frequency = 0.2,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			constant {
				value = 0.7
			}
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
		--	amplitude = 0.2,
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
			value = 0.0
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
