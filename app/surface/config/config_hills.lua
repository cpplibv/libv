-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 12,
	resolution = 128,
	--numChunks = 9,
	--numChunks = 81,
	numChunks = 169,
	numVeggie = 100,
	--numVeggie = 500,
	--numVeggie = 1000,

	fogIntensity = 0.01,
	fogColor = "#58a3fe";
}

biomes = {
	--{
	--	name = "mountain",
	--	coord = vec2f(0.2, 0.2),
	--	dominance = 1.0,
	--	cutOff = vec2f(0.1, 0.4),
	--	colorGrad = {
	--		{ 0, "hsv(210, 10%, 98%)" },
	--		{ 1, "hsv(215, 14%, 92%)" }
	--	},
	--	vegetation = {
	--	--	{
	--	--		name = "tree",
	--	--		color = "hsv(60, 80%, 100%)",
	--	--		size = 0.025,
	--	--		probability = 0.2,
	--	--	},
	--	--	{
	--	--		name = "bush",
	--	--		color = "hsv(69, 80%, 100%)",
	--	--		size = 0.022,
	--	--		probability = 0.2,
	--	--	}
	--	}
	--},
	{
		name = "grassland2",
		coord = vec2f(0.8, 0.8),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "#939f0b" }
		},
		vegetation = {
			{
				name = "tree",
				color = "#69750c",
				size = 0.05,
				probability = 0.2,
			},
			{
				name = "bush",
				color = "#69740c",
				size = 0.022,
				probability = 0.6,
			},
		}
	},
	{
		name = "green",
		coord = vec2f(0.5, 0.9),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "#415d08" }
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
				probability = 0.2,
			}
		}
	},
	{
		name = "black",
		coord = vec2f(0.2, 0.9),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "#1d2320" }
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
				probability = 0.2,
			}
		}
	},
	{
		name = "red",
		coord = vec2f(0.6, 0.0),
		dominance = 1,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "#553522" }
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
	nodes =
		pow {
			exponent = 2.0,
			--exponent = 1.0,
			add {
				warp {
					--value = 0.5,
					seed = 423,
					octaves = 6,
					amplitude = 4.5,
					frequency = 0.2,
					lacunarity = 2.0,
					persistence = 0.25,
					--simplex{
					--	seed = 126
					--}
					simplexFractal {
						seed = 600733745,
						octaves = 5,
						amplitude = 0.5,
						frequency = 0.08,
						lacunarity = 2.0,
						persistence = 0.5,
					},
				},
				--simplexFractal {
				--	seed = 2,
				--	octaves = 10,
				--	amplitude = 0.4,
				--	frequency = 0.2,
				--	lacunarity = 2.0,
				--	persistence = 0.5,
				--},
				constant {
					value = 0.5,
				},
			},
		},

	--nodes = pow {
	--	exponent = 2.0,
	--	--	}
	--	--}
	--	add {
	--		--perlin{},
	--		warp {
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
	heightSensitivity = 5, --//200m-kent 1 fok
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes = add {
		simplexFractal {
			seed = 476,
			octaves = 6,
			amplitude = 0.5,
			frequency = 0.5,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant {
			value = 0.5
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
		constant {
			value = 0.5
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
