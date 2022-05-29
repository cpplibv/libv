-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 0,
	resolution = 256,
	numVeggie = 100,

	fogIntensity = 0.0125,
	fogColor = "rgb(0.95, 0.8, 0.65)";

	sunColor = "hsv(0, 5%, 100%)";
	sunDirection = vec3f(0.8, 0.2, 0.6),
}


biomes = {
	{
		name = "desert",
		coord = vec2f(0, 0),
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
				probability = 0.2,
			},
			{
				name = "rock",
				model = "cactus_a",
				scale = range(1.5, 3.5),
				probability = 0.3,
			},
			{
				name = "rock",
				model = "cactus_b",
				scale = range(1.0, 1.5),
				probability = 0.5,
			}
		}
	},
	{
		name = "desert_rocky",
		coord = vec2f(-1, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(34, 30%, 35%)" }
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

-- -------------------------------------------------------------------------------------------------

local mask_claw =
			clamp {
				add {
					constant(0.5),
					simplexFractal {
						seed = 10000,
						octaves = 3,
						amplitude = 2.0,
						frequency = 0.018,
						lacunarity = 2.0,
						persistence = 0.5,
					},
				},
				constant(0),
				constant(1),
			}


-- -------------------------------------------------------------------------------------------------

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
			add {
				mix {
					constant(0),
					warp {
						seed = 5555555,
						octaves = 5,
						amplitude = 30.0,
						frequency = 0.02,
						lacunarity = 2.0,
						persistence = 0.35,

						simplexFractal {
							seed = 2222222,
							octaves = 1,
							amplitude = 0.05,
							frequency = 0.81,
							lacunarity = 2.0,
							persistence = 0.5,
						},
					},
					mask_claw,
				},
				simplexFractal {
					seed = 111111,
					octaves = 4,
					amplitude = 1.75,
					frequency = 0.01,
					lacunarity = 2.0,
					persistence = 0.5,
				},
				warp {
					seed = 44444,
					octaves = 6,
					amplitude = 50.0,
					frequency = 0.03,
					lacunarity = 2.0,
					persistence = 0.25,
					clamp {
						pow {
							exponent = 2.0,
							min {
								add {
									constant(0),
									simplexFractal {
										seed = 111111,
										octaves = 4,
										amplitude = 1.75,
										frequency = 0.01,
										lacunarity = 2.0,
										persistence = 0.5,
									},
								},
								simplexFractal {
									seed = 33333,
									octaves = 4,
									amplitude = 1.75,
									frequency = 0.01,
									lacunarity = 2.0,
									persistence = 0.5,
								},
							},
						},
						constant (-0.0),
						constant (4),
					},
				},
			},
}

temperature = {
	name = "temperature",
	heightSensitivity = 0.4,
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "red" }
	},
	nodes = add {
		simplexFractal {
			seed = 1,
			octaves = 4,
			amplitude = 0.1,
			frequency = 0.1,
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
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "blue" }
	},
	nodes = constant(0.5),
}

fertility = {
	name = "fertility",
	heightSensitivity = 0.2,
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "green" }
	},
	nodes = constant(0.5),
}
