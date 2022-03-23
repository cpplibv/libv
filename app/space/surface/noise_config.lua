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

Config = {
	mode = Mode._3d,
	size = 1024,
	amplitude = 0.5,

	--{
	--	--fractal
	--	octaves = 4,
	--	amplitude = 0.5,
	--	frequency = 0.2,
	--	lacunarity = 2.0,
	--	persistence = 0.5,
	--	--noise
	--	seed = 0x5EED,
	--	noiseType = NoiseType.perlin,
	--	----cellular
	--	--distanceFn =,
	--	--returnType =,
	--	--jitter = 1.0f
	--},
	--{
	--	--fractal
	--	octaves = 8,
	--	amplitude = 0.5,
	--	frequency = 0.2,
	--	lacunarity = 2.0,
	--	persistence = 0.5,
	--	--noise
	--	seed = 0x5EED,
	--	noiseType = NoiseType.simplex,
	--	----cellular
	--	--distanceFn =,
	--	--returnType =,
	--	--jitter = 1.0f
	--}
}

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
	colorGrad = {
		{-1.0 , "hsv(240, 80%, 20%)"},
		{-0.01 , "hsv(240, 80%, 60%)"},
		--{0.0 , "hsv(240, 70%, 99%)"},
		{0.0 , "hsv(220, 80%, 60%)"},
		{0.01 , "hsv(60, 80%, 90%)"},
		{0.05 , "hsv(120, 70%, 75%)"},
		{0.1 , "hsv(120, 100%, 50%)"},
		{0.5 , "hsv(120, 100%, 60%)"},
		{0.6 , "hsv(120, 60%, 60%)"},
		{1.0 , "hsv(220, 5%, 95%)"},
		--{key = -2.0 ,value =  "blue"},
		--{key = 0.0 , value = "yellow"},
		--{key = 1.0 , value = "green"},
		--{key = 2.0 , value = "grey"},
		--[-2.0] = "blue",
		--[0.0] = "yellow",
		--[1.0] = "green",
		--[2.0] = "grey",
	},
	mode = Mode._3d,
	visualization = Visualization.spheres,
	resolution = 2,
	amplitude = 0.5,
	--treeSize = 0.003,
	plantDistribution = PlantDistribution.random,
	circleNumber = 50,
	circleSize = 0.01,
	objects = {
		--asd = 12,
		{
			type = "tree",
			size = 0.02,
			count = 20,
			color = "hsv(120, 70%, 75%)",
		},
		{
			type = "bush",
			size = 0.012,
			count = 40,
			color= "hsv(160, 70%, 75%)",
		},
		{
			type = "rock",
			size = 0.008,
			count = 60,
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

nodes =
	pow{
		exponent = 2.0,
		--	}
		--}
		add{
			--perlin{},
			warp{
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
					amplitude = 1,
					frequency = 0.5,
					lacunarity = 2.0,
					persistence = 0.5,
				},
			},
			--simplexFractal {
			--	seed = 423,
			--	octaves = 6,
			--	amplitude = 0.2,
			--	frequency = 0.2,
			--	lacunarity = 2.0,
			--	persistence = 0.5,
			--},
			constant{
				value = 0.5
			},
			--perlin{}

		}
	}

heatMaps = {
	temperature = {
		heightSensitivity = 0.2,
		simplexFractal {
			seed = 810,
			octaves = 6,
			amplitude = 1,
			frequency = 0.0015,
			lacunarity = 2.0,
			persistence = 0.5,
		}
	},
	humidity = {
		heightSensitivity = 0.2,
		simplexFractal {
			seed = 810,
			octaves = 6,
			amplitude = 1,
			frequency = 0.0015,
			lacunarity = 2.0,
			persistence = 0.5,
		}
	},
	fertility = {
		heightSensitivity = 0.2,
		simplexFractal {
			seed = 810,
			octaves = 6,
			amplitude = 1,
			frequency = 0.0015,
			lacunarity = 2.0,
			persistence = 0.5,
		}
	},
	hardness = {
		...
	}
}

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




