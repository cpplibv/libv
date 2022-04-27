-- Test surface config example: Nova

-- -------------------------------------------------------------------------------------------------

Mode = {
	texture = 0,
	_3d = 1
}

Visualization = {
	model = 0,
	spheres = 1
}

NodeType = {
	constant = "constant",
	perlin = "perlin",
	simplex = "simplex",
	cellular = "cellular",
	simplexFractal = "simplexFractal",
	warp = "warp",
	add = "add",
	mul = "mul",
	pow = "pow",
}

NoiseType = {
	value = 0,
	perlin = 1,
	simplex = 2,
	simplex2S = 3,
	cellular = 4,
}

PlantDistribution = {
	random = 0,
	clustered = 1,
}

-- -------------------------------------------------------------------------------------------------

local constant = function(args)
	args = args or {}
	args.nodeType = NodeType.constant
	return args
end

local perlin = function(args)
	args = args or {}
	args.nodeType = NodeType.perlin
	return args
end

local simplex = function(args)
	args = args or {}
	args.nodeType = NodeType.simplex
	return args
end

DistFun = {
	euclidean = 0,
	euclideanSq = 1,
	manhattan = 2,
	hybrid = 3,
}

ReturnType = {
	cellValue = 0,
	distance = 1,
	distance2 = 2,
	distance2Add = 3,
	distance2Sub = 4,
	distance2Mul = 5,
	distance2Div = 6,
}

local cellular = function(args)
	args = args or {}
	args.nodeType = NodeType.cellular
	return args
end

local simplexFractal = function(args)
	args = args or {}
	args.nodeType = NodeType.simplexFractal
	return args
end

local warp = function(args)
	args = args or {}
	args.nodeType = NodeType.warp
	return args
end

local add = function(args)
	args = args or {}
	args.nodeType = NodeType.add
	return args
end

local mul = function(args)
	args = args or {}
	args.nodeType = NodeType.mul
	return args
end

local pow = function(args)
	args = args or {}
	args.nodeType = NodeType.pow
	return args
end

-- -------------------------------------------------------------------------------------------------

config = {
	seed = 0,
	visualization = Visualization.spheres,
	resolution = 64,
	--numChunks = 9,
	--numChunks = 81,
	numChunks = 169,
	plantDistribution = PlantDistribution.random,
	numVeggie = 100,
	--numVeggie = 500,
	--numVeggie = 1000,
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
		cutOff = vec2f(0.1, 0.4),
		colorGrad = {
			{ 0, "hsv(121, 70%, 80%)" },
			{ 1, "hsv(140, 90%, 70%)" }
		},
		vegetation = {
			{
				name = "tree",
				color = "hsv(150, 65%, 70%)",
				size = 0.05,
				probability = 0.2,
			},
			{
				name = "bush",
				color = "hsv(145, 70%, 60%)",
				size = 0.022,
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
	nodes = pow {
		exponent = 2.0,
		--exponent = 1.0,
		add {
			warp {
				--value = 0.5,
				seed = 423,
				octaves = 6,
				amplitude = 3.5,
				frequency = 0.5,
				lacunarity = 2.0,
				persistence = 0.25,
				--simplex{
				--	seed = 126
				--}
				simplexFractal {
					seed = 600733745,
					octaves = 5,
					amplitude = 0.5,
					frequency = 0.05,
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
			seed = 1,
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
