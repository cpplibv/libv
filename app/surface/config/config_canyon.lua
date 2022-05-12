-- Test surface config example: Canyon

require("config/common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 500,
	resolution = 256,
	numChunks = 169,
	numVeggie = 100,

	fogIntensity = 0.035,
	fogColor = "rgb(0.9, 0.8, 0.6)";
}

biomes = {
	{
		name = "grassland2",
		coord = vec2f(0.2, 0.8),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(50, 70%, 80%)" },
			{ 1, "#DB8848" }
		},
		vegetation = {
			{
				name = "tree",
				color = "#DB8840",
				size = 0.03,
				probability = 0.2,
			},
			{
				name = "bush",
				color = "#DB8860",
				size = 0.02,
				probability = 0.6,
			},
		}
	},
	{
		name = "desert",
		coord = vec2f(0.8, 0.2),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "#F2D77C" }
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
				color = "#F2D77C",
				size = 0.02,
				probability = 0.8,
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
		{ 0.6, "hsv(120, 60%, 60%)" },
		{ 1.0, "hsv(220, 5%, 95%)" },
	},
	nodes =
	min {

		pow {
			exponent = 2.0,
			--exponent = 1.0,
			add {
				--mix {
					mul {
						constant {
							value = 0.5
						},
						value {
							seed = 200,
						},
					},
					mul {
						value {
							seed = 125
						},
						simplexFractal {
							seed = 345,
							octaves = 5,
							amplitude = 0.5,
							frequency = 0.25,
							lacunarity = 2.0,
							persistence = 0.5,
						},
					},
					--add {
					--	coord(0.05, 0),
					--	constant {
					--		value = 0.5,
					--	}
					--}
				--},

			},
		},
		add{
			simplexFractal {
				seed = 345,
				octaves = 5,
				amplitude = 0.2,
				frequency = 0.2,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			constant {
				value = 0.9
			}
		}

	}
}


temperature = {
	name = "temperature",
	heightSensitivity = 5, --//200m-kent 1 fok
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes = add {
		value {
			seed = 100,
		},
		constant {
			value = 0.0
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
