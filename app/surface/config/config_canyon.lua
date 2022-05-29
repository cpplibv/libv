-- Test surface config example: Canyon

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 501,
	resolution = 256,
	numChunks = 169,
	numVeggie = 500,

	fogIntensity = 0.01,
	fogColor = "rgb(0.99, 0.7, 0.5)";

	sunColor = "rgb(0.9, 0.8, 0.6)";

	skyboxTexture = "cube_redDesertNight.dds",
}

biomes = {
	{
		name = "grassland2",
		coord = vec2f(-1.8, 0.8),
		dominance = 1.0,
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(50, 70%, 80%)" },
			{ 1, "#FB8848" }
		},
		vegetation = {
			{
				name = "tree",
				model = "cactus_a",
				scale = range(1.5, 3.5),
				size = 0.03,
				probability = 0.1,
			},
			{
				name = "bush",
				model = "cactus_b",
				scale = range(0.4, 0.9),
				size = 0.02,
				probability = 0.1,
			},
			{
				name = "rock",
				model = "rock",
				scale = range(1.0, 2.0),
				size = 0.02,
				probability = 0.3,
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
			{ 1, "hsv(25, 35%, 98%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "cactus_a",
				scale = range(1.5, 3.5),
				probability = 0.5,
			},
			{
				name = "rock",
				model = "cactus_b",
				scale = range(1.0, 1.5),
				probability = 0.5,
			}
		}
	},
}

canyon=min {

	mul {
		constant(5),
		pow {
			exponent = 2.0,
			--exponent = 1.0,
			add {
				--mix {
				mul {
					constant {
						value = 0.5
					},
					fractal {
						seed = 0,
						octaves = 1,
						amplitude = 1.0,
						frequency = 0.2,
						lacunarity = 2.0,
						persistence = 0.5,

						value {
							seed = 200,
						},
					},
				},
				mul {
					fractal {
						seed = 0,
						octaves = 1,
						amplitude = 1.0,
						frequency = 0.2,
						lacunarity = 2.0,
						persistence = 0.5,

						value {
							seed = 125,
						},
					},
					simplexFractal {
						seed = 345,
						octaves = 5,
						amplitude = 0.5,
						frequency = 0.05,
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
	},
	add{
		simplexFractal {
			seed = 345,
			octaves = 4,
			amplitude = 0.8,
			frequency = 0.03,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant {
			value = 0.7 * 4
		}
	}

}

mask=
clamp{

	add{
		simplexFractal {
			seed = 103421,

			octaves = 6,
			amplitude = 5,
			frequency = 0.01,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant(1),
	},
	constant(0),
	constant(1),
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
	mix{
		constant{
			value=0
		},
		canyon,
	mask

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
	nodes = constant(0)
}
