-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 1,
	resolution = 256,
	numVeggie = 50,

	fogIntensity = 0.01,
	fogColor = "rgb(0.1, 0.1, 0.1)";

	sunColor = "hsv(0, 0%, 100%)";
	sunDirection = vec3f(0.8, 0.2, 0.6),

	skyboxTexture = "cube_nebula_green_0001.dds",
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
			{ 1, "hsv(34, 10%, 92%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "rock",
				probability = 1.0,
			},
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
			{ 1, "hsv(34, 5%, 35%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "rock",
				probability = 0.2,
			}
		}
	},
}

-- -------------------------------------------------------------------------------------------------

local mask_claw =
			clamp {
				add {
					constant(2.0),
					simplexFractal {
						seed = 20000,
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

local claws =
			mix {
				constant(0),
				warp_legacy {
					seed = 21,
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
			}

local spots_small =
		warp_legacy {
			seed = 42,
			octaves = 5,
			amplitude = 1.0,
			frequency = 0.15,
			lacunarity = 2.0,
			persistence = 0.5,
			add {
				constant(-0.35),
				min {
					constant(0.35),
					pow {
						exponent = 2.0,
						mul {
							constant(3.0),
							add {
								constant(1.0),
								fractal {
									seed = 0,
									octaves = 1,
									amplitude = 1.0,
									frequency = 0.3,
									lacunarity = 2.0,
									persistence = 0.5,

									cellular {
										seed = 3330000,
										distanceFn = DistFun.euclidean,
										returnType = ReturnType.distance,
										jitter = 1,
									},
								},
							},
						},
					},
				},
			},
		}

local spots_big =
		warp_legacy {
			seed = 0,
			octaves = 5,
			amplitude = 1.0,
			frequency = 0.15,
			lacunarity = 2.0,
			persistence = 0.5,

			add {
				constant(-1.5),
				min {
					constant(2.0),
					pow {
						exponent = 2.0,
						mul {
							constant(4.0),
							add {
								constant(1.0),
								fractal {
									seed = 0,
									octaves = 1,
									amplitude = 1.0,
									frequency = 0.075,
									lacunarity = 2.0,
									persistence = 0.5,

									cellular {
										seed = 2220000,
										distanceFn = DistFun.euclidean,
										returnType = ReturnType.distance,
										jitter = 1,
									},
								},
							},
						},
					},
				},
			}
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
				simplexFractal {
					seed = 7777777,
					octaves = 3,
					amplitude = 0.05,
					frequency = 0.3,
					lacunarity = 2.0,
					persistence = 0.5,
				},
				simplexFractal {
					seed = 7777777,
					octaves = 3,
					amplitude = 1.0,
					frequency = 0.02,
					lacunarity = 2.0,
					persistence = 0.5,
				},

				min {
					add {
						claws,
						spots_big,
					},
					add {
						spots_small,
						spots_big,
					}
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
