-- Test surface config example: Nova

require("config/common/nodes")

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
	resolution = 64,
	--numChunks = 9,
	--numChunks = 81,
	numChunks = 169,
	--numChunks = 369,
	numVeggie = 150,
	--numVeggie = 500,
	--numVeggie = 1000,

	fogIntensity = 0.03,
	fogColor = "hsv(120, 5%, 95%)";
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
		coord = vec2f(0.2, 0.8),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(100, 40%, 80%)" },
			{ 1, "hsv(120, 50%, 45%)" }
		},
		vegetation = {
			--{
			--	name = "tree",
			--	color = "hsv(150, 65%, 70%)",
			--	size = 0.05,
			--	probability = 0.2,
			--},
			{
				name = "bush",
				color = "hsv(145, 70%, 60%)",
				scale = range(2.0, 4.0),
				hue = shift(-60.0, 10.0),
				saturation = shift(0.2, 0.1),
				value = shift(0.1, 0.1),
				probability = 0.03,
			},
			{
				name = "bush",
				color = "hsv(145, 70%, 60%)",
				scale = range(2.0, 4.0),
				hue = shift(20.0, 10.0),
				saturation = shift(-0.1, 0.2),
				value = shift(0.1, 0.1),
				probability = 1.0,
			},
		}
	},
	{
		name = "desert",
		coord = vec2f(0.8, 0.2),
		dominance = 1.0,
		--handover = global_handover,
		--takeover = global_takeover,

		cutOff = vec2f(0.1, 0.4),
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
				probability = 0.002,
			}
		}
	},
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
			frequency = 0.05,
			lacunarity = 2.0,
			persistence = 0.5,
		},
	}
end

mountainRange = pow {
	exponent = 2.0,

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

-- =================================================================================================

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
			mountainRange,
			microRange,
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
