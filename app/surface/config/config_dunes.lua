-- Test surface config example: Nova

require("common/nodes")

-- -------------------------------------------------------------------------------------------------

local global_handover = {
	soft = 0.2,
	hard = 0.1
}

local global_takeover = {
	soft = 0.35,
	hard = 0.6
}

config = {
	seed = 0,
	resolution = 256,
	--numChunks = 169,
	--numChunks = 441,
	--numVeggie = 150,
	--numVeggie = 500,
	--numVeggie = 1601,
	numVeggie = 20000,

	fogColor = "hsv(120, 5%, 95%)";
	fogIntensity = 0.02,

	sunColor = "hsv(0, 10%, 95%)";
	--sunDirection = vec3f(0.891186, 0.0560718, 0.450159),
	sunDirection = vec3f(0.9, 0.06, 0.7),

	skyboxTexture = "cube_fluffball.dds",
	--skyboxTexture = "cube_redDesertNight.dds",
	--skyboxTexture = "cube_AmbienceExposure4k.dds",
}

biomes = {
	{
		name = "snow",
		coord = vec2f(-8, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, vec3f(2, 2, 2) }
		},
		vegetation = {
		}
	},
	{
		name = "mountain",
		coord = vec2f(-6, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(0, 0%, 50%)" }
		},
		vegetation = {
			{
				name = "tree",
				model = "pine",
				scale = range(1.0, 2.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.0, 0.0),
				probability = 0.01,
			},
		}
	},
	{
		name = "grassland",
		coord = vec2f(0.2, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(100, 40%, 80%)" },
			--{ 1, "hsv(0, 30%, 80%)" }
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
				name = "treeRed",
				model = "oak",
				scale = range(2.0, 4.0),
				hue = shift(-60.0, 10.0),
				saturation = shift(0.3, 0.1),
				value = shift(0.25, 0.1),
				probability = 0.01,
			},
			{
				name = "tree",
				model = "oak",
				scale = range(1.0, 3.0),
				--hue = shift(20.0, 5.0),
				--saturation = shift(0.0, 0.0),
				--value = shift(0.0, 0.04),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.2, 0.1),
				probability = 1.0,
			},
		}
	},
	{
		name = "sandbed",
		coord = vec2f(0.9, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(36, 35%, 98%)" },
			{ 1, "hsv(34, 14%, 100%)" }
		},
		vegetation = {
			{
				name = "rock",
				model = "rock",
				probability = 0.02,
				scale = range(0.5, 1.5),
				hue = shift(0.0, 0.0),
				saturation = shift(0.0, 0.0),
				value = shift(0.2, 0.2),
			}
		}
	},
	--{
	--	name = "sandbed-darl",
	--	coord = vec2f(3.0, 0.2),
	--	dominance = 1.0,
	--	handover = global_handover,
	--	takeover = global_takeover,
	--
	--	cutOff = vec2f(0.1, 0.4),
	--	colorGrad = {
	--		{ 0, "hsv(36, 35%, 98%)" },
	--		{ 1, "hsv(34, 14%, 50%)" }
	--	},
	--	vegetation = {
	--		{
	--			name = "rock",
	--			model = "rock",
	--			probability = 0.02,
	--			scale = range(0.5, 1.5),
	--			hue = shift(0.0, 0.0),
	--			saturation = shift(0.0, 0.0),
	--			value = shift(0.2, 0.2),
	--		}
	--	}
	--},
}

-- =================================================================================================

base = function(seed)
	return add{
		constant {
			value = 1,
		},
		simplexFractal {
			seed = seed,
			octaves = 2,
			amplitude = 1,
			frequency = 0.025,
			lacunarity = 2.0,
			persistence = 0.5,
		},
	}
end

mountainRange = mul {
	constant(1.4),
	pow {
		exponent = 1.5,

		min {
			base(222),
			min {
				base(333),
				min {
					base(555),
					base(666),
				},
			},
		},
	},
}

baseMicro = function(seed)
	return add{
		constant {
			value = 0.5,
		},
		simplexFractal {
			seed = seed,
			octaves = 1,
			amplitude = 0.5,
			frequency = 0.2,
			lacunarity = 2.0,
			persistence = 0.5,
		},
	}
end

microRange = mul {
	constant {
		value = -0.5,
	},
	pow {
		exponent = 0.2,

		min {
			baseMicro(2222),
			min {
				--baseMicro(3333),
				--min {
					baseMicro(4444),
					min {
						baseMicro(5555),
						baseMicro(6666),
					},
				--},
			},
		},
	},
}

bumps =
	max {
		constant(-0.2),
		simplexFractal {
			seed = 601,
			octaves = 5,
			amplitude = 0.8,
			frequency = 0.02,
			lacunarity = 2.0,
			persistence = 0.55,
		},
	}

-- =================================================================================================

height = {
	name = "height",
	nodes =
		add {
			mul { constant(2), mountainRange },
			mul { constant(1), microRange },
			mul { constant(2), bumps },
		}
	--nodes = pow {
	--	exponent = 2.0,
	--
	--	min {
	--		simplexFractal {
	--			seed = 33543745,
	--			octaves = 5,
	--			amplitude = 0.5,
	--			frequency = 0.1,
	--			lacunarity = 2.0,
	--			persistence = 0.5,
	--		},
	--		mix {
	--			constant {
	--				value = 0,
	--			},
	--			add {
	--				simplexFractal {
	--					seed = 600733745,
	--					octaves = 5,
	--					amplitude = 0.5,
	--					frequency = 0.1,
	--					lacunarity = 2.0,
	--					persistence = 0.5,
	--				},
	--				constant {
	--					value = 0.5,
	--				},
	--			},
	--			--},
	--			saturate {
	--				add {
	--					simplexFractal {
	--						seed = 602364345,
	--						octaves = 5,
	--						amplitude = 1.0,
	--						frequency = 0.10,
	--						lacunarity = 1.5,
	--						persistence = 0.5,
	--					},
	--					constant {
	--						value = 0.5,
	--					},
	--				},
	--			},
	--		},
	--	},
	--},
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
			octaves = 6,
			amplitude = 0.3,
			frequency = 0.25,
			lacunarity = 2.0,
			persistence = 0.5,
		},
		constant {
			value = 0.1
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
