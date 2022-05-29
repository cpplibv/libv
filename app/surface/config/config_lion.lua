-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 0,
	resolution = 256,
	numVeggie = 500,
	--numVeggie = 20000,

	fogIntensity = 0.015,
	fogColor = "rgb(0.7, 0.8, 0.9)";

	sunColor = "hsv(0, 15%, 95%)";
	sunDirection = vec3f(0.8, 0.2, 0.6),
}

biomes = {
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
		--	{
		--		name = "tree",
		--		color = "hsv(60, 80%, 100%)",
		--		size = 0.025,
		--		probability = 0.2,
		--	},
		--	{
		--		name = "bush",
		--		color = "hsv(69, 80%, 100%)",
		--		size = 0.022,
		--		probability = 0.2,
		--	}
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
				color = "hsv(150, 65%, 70%)",
				size = 0.05,
				probability = 0.3,
			},
			{
				name = "bush",
				color = "hsv(145, 70%, 60%)",
				size = 0.022,
				probability = 0.7,
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
				color = "hsv(150, 65%, 70%)",
				size = 0.05,
				probability = 0.3,
			},
			{
				name = "bush",
				color = "hsv(145, 70%, 60%)",
				size = 0.022,
				probability = 0.7,
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
				probability = 0.1,
			}
		}
	},
	--{
	--	name = "water",
	--	coord = vec2f(1.5, 0),
	--	dominance = 1.0,
	--	cutOff = vec2f(0.1, 0.4),
	--	colorGrad = {
	--		{ 0, "hsv(36, 35%, 98%)" },
	--		{ 1, "hsv(180, 90%, 92%)" }
	--	},
	--	vegetation = {
	--		--{
	--		--	name = "tree",
	--		--	color = "hsv(60, 80%, 100%)",
	--		--	size = 0.05,
	--		--	probability = 0.2,
	--		--	--fertilityRange = vec2f{0.6, 1.0},
	--		--	--color = "hsv(120, 60%, 100%)",
	--		--},
	--		{
	--			name = "rock",
	--			color = "hsv(69, 10%, 60%)",
	--			size = 0.022,
	--			probability = 0.1,
	--		}
	--	}
	--},
}

--local flexSimplex = function (args)
--	return
--		simplexFractal(args);
--end

--mountain ----------> grassland ---> desert
--		   \--> forest >--/

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
		warp {
			seed = 98001,
			octaves = 3,
			amplitude = 50.0,
			frequency = 0.02,
			lacunarity = 2.0,
			persistence = 0.25,
			--frequency = 0.015,
			--persistence = 0.5,

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
		}
}

temperature = {
	name = "temperature",
	heightSensitivity = 0.4, --//200m-kent 1 fok
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
	--heightSensitivity = 0.2,
	colorGrad = {
		{ 0.0, "white" },
		{ 1.0, "blue" }
	},
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
