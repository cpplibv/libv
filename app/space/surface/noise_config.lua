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

--PlantDistribution = {
--	random = "random",
--	clustered = "clustered",
--}


--option 1
--height = perlin(seed) > warp(0.5) + cellular(cell.distance0);

--option 2
--function perlin()
--	return {
--		nodeType = "perlin"
--		--seed = seed
--	}
--end


--function warp(node)
--	return {
--		nodeType = "warp",
--		--warp_value = 0.5,
--		input = node
--	}
--end

--function add(...)
--	local args = {...}
--	--local x = table.unpack(arg)
--	args.node_type = "add"
--	return args
--end


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

local pow = function(args)
	args = args or {}
	args.nodeType = NodeType.pow
	return args
end

--base = perlin(seed, 3, 4, 5, 6, 2, 4);
--result =
--		add(
--			warp(
--				perlin(seed, 3, 4, 5, 6, 2, 4)
--			),
--			cellular(cell.distance0)
--		)

--local addKeyValue = function(key, value)
--
--	return args
--end

config = {

	--currentHeatMap = "temperature",
	visualization = Visualization.spheres,
	resolution = 64,
	numChunks = 81,
	amplitude = 0.5,
	--treeSize = 0.003,
	plantDistribution = PlantDistribution.random,
	circleNumber = 50,
	circleSize = 0.01,

--                      /\
--  COLD|BARREN        /  \ Polar
--                    /    \
--      ^            /      \ Subpolar
--      |           /        \
--      |          /          \ Temperate
--                /            \
--  HOT|FERTILE  /              \ Tropical
--               ================
--  HUMID|WET|FERTILE   -->   ARID|DRY|BARREN

	objects = {
		--asd = 12,
		{
			type = "tropical rain forest - tree",
			size = 0.02,
			count = 60,
			color = "hsv(120, 70%, 75%)",
		},
		{
			type = "bush",
			size = 0.012,
			count = 30,
			color= "hsv(160, 70%, 75%)",
		},
		{
			type = "rock",
			size = 0.008,
			count = 20,
			color = "grey",
		},
		--{
		--	--type = "boulder",
		--	type = "rock",
		--	size = 0.005,
		--	count = 300,
		--	color = "hsv(160, 10%, 60%)",
		--},
	},

}

--BiomeZone = {
--	tropical = 0,
--	temperate = 1,
--	subpolar = 2,
--	polar = 3,
--};

--not regions, biomes-forest types
biomes = {
	{
		name = "no-biome",
		coord = vec2f(0.25, 0.75),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.6),
		colorGrad = {
			{0, "black"},
			{1, "white"}
		}
	},
	{
		name = "polar",
		coord = vec2f(0.125, 0.125),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(191, 20%, 26%)"},
			{1, "hsv(191, 20%, 76%)"}
		}
	},
	{
		name = "subpolar-boreal",
		coord = vec2f(0.25, 0.25),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(41, 89%, 12%)"},
			{1, "hsv(41, 89%, 62%)"}
		}
	},
	{
		name = "temperate-rainforest",
		coord = vec2f(0.65, 0.65),
		--radius = 0.05
		--min cut off, max cut off
		--dominance
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(130, 50%, 27%)"},
			{1, "hsv(130, 60%, 50%)"}
		}
	},
	{
		name = "temperate-forest",
		coord = vec2f(0.55, 0.4),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(110, 20%, 27%)"},
			{1, "hsv(110, 50%, 40%)"}
		}
	},
	{
		name = "temperate-grassland",
		coord = vec2f(0.5, 0.125),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(140, 20%, 27%)"},
			{1, "hsv(140, 70%, 40%)"}
		}
	},
	{
		name = "tropical-rainforest",
		coord = vec2f(0.8, 0.75),
		--radius = 0.05
		--min cut off, max cut off
		cutOff = vec2f(0.1, 0.7),
		colorGrad = {
			{0, "hsv(100, 78%, 2%)"},
			{1, "hsv(100, 78%, 52%)"}
		}
	},
	--{
	--	name = "tropical-savanna",
	--	colorGrad = {
	--		{0, "hsv(100, 78%, 2%)"},
	--		{1, "hsv(100, 78%, 52%)"}
	--	}
	--},
	--{
	--	name = "subtropical-desert",
	--	colorGrad = {
	--		{0, "hsv(100, 78%, 2%)"},
	--		{1, "hsv(100, 78%, 52%)"}
	--	}
	--}
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
		--	}
		--}
		add {
			--perlin{},
			warp {
				--value = 0.5,
				seed = 423,
				octaves = 5,
				amplitude = 0.05,
				frequency = 0.25,
				lacunarity = 2.0,
				persistence = 0.5,
				--simplex{
				--	seed = 126
				--}
				simplexFractal {
					seed = 810,
					octaves = 6,
					amplitude = 0.7,
					frequency = 0.5,
					lacunarity = 2.0,
					persistence = 0.5,
				},
			},
			simplexFractal {
				seed = 1,
				octaves = 6,
				amplitude = 0.2,
				frequency = 0.2,
				lacunarity = 2.0,
				persistence = 0.5,
			},
			--simplexFractal {
			--	seed = 1000,
			--	octaves = 6,
			--	amplitude = 0.6,
			--	frequency = 0.2,
			--	lacunarity = 2.0,
			--	persistence = 0.5,
			--},
			constant {
				value = 0.5
			},
			--cellular {
			--	distanceFn = DistFun.manhattan,
			--	returnType = ReturnType.distance2Div,
			--	jitter = 1,
			--},

		}
	}
}


--heatMaps = {
temperature = {
	name = "temperature",
	heightSensitivity = 0.2,
	colorGrad = {
		{ 0.0, "black" },
		{ 1.0, "white" }
	},
	nodes =

 add {
	 simplexFractal {
		 seed = 1,
		 octaves = 6,
		 amplitude = 0.5,
		 frequency = 0.5,
		 lacunarity = 2.0,
		 persistence = 0.5,
	 },
	 warp{
		 seed = 423,
		 octaves = 5,
		 amplitude = 4,
		 frequency = 0.25,
		 lacunarity = 2.5,
		 persistence = 0.5,
		 cellular {
			 seed = 123,
			 distanceFn = DistFun.euclidean,
			 returnType = ReturnType.cellValue,
			 jitter = 1,
		 },
	 },

		constant {
			value = 0.8
		}
	}
}
humidity = {
	name = "humidity",
	heightSensitivity = 0.2,
	colorGrad = {
		{0.0 , "white"},
		{1.0 , "gray"}
	},
	nodes =

	add {
		simplexFractal {
			seed = 123,
			octaves = 6,
			amplitude = 0.5,
			frequency = 0.5,
			lacunarity = 2.0,
			persistence = 0.5,
		},
	--cellular {
	--	seed = 123,
	--	distanceFn = DistFun.euclidean,
	--	returnType = ReturnType.cellValue,
	--	jitter = 1,
	--},
		constant{
			value = 0.5
		}
	}


}

fertility =	{
	name = "fertility",
	heightSensitivity = 0.2,
	colorGrad = {
		{0.0 , "white"},
		{1.0 , "green"}
	},
	nodes =
	add{
		warp{
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
	--hardness = {
	--	...
	--}
--}

--option 3
--NodeTree = {
--	node_type = "add",
--	{
--		node_type = "warp",
--		warp_value = 0.5,
--		input = {
--			node_type = "perlin"
--			--seed,
--			--args,
--		}
--	},
--	{
--		node_type = "perlin"
--		--return_value = cell.distance0,
--	}
--}




