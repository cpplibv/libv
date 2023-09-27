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
	},
	nodes =

	--mix {
	simplexFractal {
		seed = 1,
		octaves = 1,
		amplitude = 1,
		frequency = 0.1,
		lacunarity = 2.0,
		persistence = 0.5,
	}
		--saturate {
		--	coord(0.1, 0)
		--}

	--}

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
