-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 0,
	resolution = 256,
	--numVeggie = 500,
	numVeggie = 20000,

	fogIntensity = 0.01,
	fogColor = "rgb(0.7, 0.8, 0.9)";

	sunColor = "hsv(0, 15%, 95%)";
	sunDirection = vec3f(0.8, 0.2, 0.6),
}

biomes = {
	{
		name = "snow",
		coord = vec2f(-2, 0),
		dominance = 1.0,
		--handover = { soft = 0.3, hard = 0.0 },
		--takeover = { soft = 0.8, hard = 0.9 },
		colorGrad = {
			{ 0, "hsv(210, 10%, 98%)" },
			{ 1, vec3f(2, 2, 2) }
		},
		vegetation = {
		}
	},
	{
		name = "mountain",
		coord = vec2f(-1, 0),
		dominance = 1.0,
		handover = { soft = 0.4, hard = 0.1 },
		takeover = { soft = 0.6, hard = 0.8 },
		colorGrad = {
			{ 0, "hsv(210, 10%, 98%)" },
			{ 1, "hsv(215, 14%, 92%)" }
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
		name = "grassland2",
		coord = vec2f(0.2, 0.5),
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
				value = shift(0.1, 0.05),
				probability = 1.0,
			},
		}
	},
	{
		name = "forest",
		coord = vec2f(-0.2, -0.5),
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
		name = "desert",
		coord = vec2f(1, 0),
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
				color = "hsv(69, 10%, 60%)",
				size = 0.022,
				probability = 0.1,
			}
		}
	},
}

height = {
	name = "height",
	heightSensitivity = 0.0,
	nodes =
			add {
				warp_legacy {
					seed = 98001,
					octaves = 4,
					amplitude = 50.0,
					frequency = 0.02,
					lacunarity = 2.0,
					persistence = 0.25,

					simplexFractal {
						seed = 433322,
						octaves = 4,
						amplitude = 0.75,
						frequency = 0.01,
						lacunarity = 2.0,
						persistence = 0.5,
					},
				},
				simplexFractal {
					seed = 4894558,
					octaves = 4,
					amplitude = 0.25,
					frequency = 0.035,
					lacunarity = 2.0,
					persistence = 0.5,
				},
				simplexFractal {
					seed = 4894558,
					octaves = 4,
					amplitude = 2.75,
					frequency = 0.0015,
					lacunarity = 2.0,
					persistence = 0.5,
				},
				warp_legacy {
					seed = 98001,
					octaves = 3,
					amplitude = 50.0,
					frequency = 0.02,
					lacunarity = 2.0,
					persistence = 0.25,

					clamp {
						mul {
							constant(4.0),
							pow {
								exponent = 2.0,
								add {
									simplexFractal {
										seed = 600733745,
										octaves = 4,
										amplitude = 0.75,
										frequency = 0.01,
										lacunarity = 2.0,
										persistence = 0.5,
									},
								},
							},
						},

						constant(0.0),
						add {
							constant(3.5),
							simplexFractal {
								seed = 455553745,
								octaves = 4,
								amplitude = 0.45,
								frequency = 0.1,
								lacunarity = 2.0,
								persistence = 0.5,
							},
						},
					},
				}
			}
}

temperature = {
	name = "temperature",
	heightSensitivity = 0.4, --//200m-kent 1 fok
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
			value = 0.9
		}
	}
}

humidity = {
	name = "humidity",
	--heightSensitivity = 0.2,
	nodes = --add {
		add {
			simplexFractal {
				seed = 143421,

				octaves = 3,
				amplitude = 0.45,
				frequency = 0.02,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			simplexFractal {
				seed = 143422,

				octaves = 3,
				amplitude = 0.45,
				frequency = 0.02,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			--constant {
			--	value = -0.5
			--},
		}
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
