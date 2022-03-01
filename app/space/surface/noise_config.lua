Mode = {
	texture = 0,
	_3d = 1
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

result =
	pow{
		mode = Mode._3d,
		--mode = Mode.texture,
		size = 1024,
		amplitude = 0.5,

		exponent = 2.0,
		--exponent = 0.5,
		--exponent = 1.0,

		--warp{
		--	--value = 0.5,
		--	seed = 423,
		--	octaves = 5,
		--	amplitude = 100,
		--	frequency = 0.01,
		--	lacunarity = 2.0,
		--	persistence = 0.5,
		--	simplex{
		--		seed = 500
		--	}
		--}
		add{
			--perlin{},
			warp{
				--value = 0.5,
				seed = 423,
				octaves = 5,
				amplitude = 1,
				frequency = 0.25,
				lacunarity = 2.0,
				persistence = 0.5,
				--simplex{
				--	seed = 126
				--}
				simplexFractal {
					seed = 423,
					octaves = 6,
					amplitude = 0.5,
					frequency = 0.2,
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
				value = 0.25
			},
			--perlin{}

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


NodeTree = {
	node_type = "add",
	[1] = {...},
	[2] = {...}
}


