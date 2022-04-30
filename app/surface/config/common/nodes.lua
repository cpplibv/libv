-- Common config

-- -------------------------------------------------------------------------------------------------


NodeType = {
	constant = "constant",
	value = "value",
	perlin = "perlin",
	simplex = "simplex",
	cellular = "cellular",
	simplexFractal = "simplexFractal",
	warp = "warp",
	fractal = "fractal",
	coord = "coord",
	add = "add",
	mul = "mul",
	pow = "pow",
	mix = "mix",
	clamp = "clamp",
	saturate = "saturate",
	min = "min",
	max = "max",
	sin = "sin",
	smoothstep = "smoothstep",
}

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

-- -------------------------------------------------------------------------------------------------


constant = function(args)
	args = args or {}
	args.nodeType = NodeType.constant
	return args
end

value = function(args)
	args = args or {}
	args.nodeType = NodeType.value
	return args
end

perlin = function(args)
	args = args or {}
	args.nodeType = NodeType.perlin
	return args
end

simplex = function(args)
	args = args or {}
	args.nodeType = NodeType.simplex
	return args
end

cellular = function(args)
	args = args or {}
	args.nodeType = NodeType.cellular
	return args
end

simplexFractal = function(args)
	args = args or {}
	args.nodeType = NodeType.simplexFractal
	return args
end

warp = function(args)
	args = args or {}
	args.nodeType = NodeType.warp
	return args
end

fractal = function(args)
	args = args or {}
	args.nodeType = NodeType.fractal
	return args
end

coord = function(args_or_x, none_or_y)
	if type(args_or_x) == "number" and type(none_or_y) == "number" then
		local node = {}
		node.nodeType = NodeType.coord
		node.weights = vec2f(args_or_x, none_or_y)
		return node
	else
		local node = args_or_x or {}
		node.nodeType = NodeType.coord
		return node
	end
end

add = function(args)
	args = args or {}
	args.nodeType = NodeType.add
	return args
end

mul = function(args)
	args = args or {}
	args.nodeType = NodeType.mul
	return args
end

pow = function(args)
	args = args or {}
	args.nodeType = NodeType.pow
	return args
end

mix = function(args)
	args = args or {}
	args.nodeType = NodeType.mix
	return args
end

clamp = function(args)
	args = args or {}
	args.nodeType = NodeType.clamp
	return args
end

saturate = function(args)
	args = args or {}
	args.nodeType = NodeType.saturate
	return args
end

min = function(args)
	args = args or {}
	args.nodeType = NodeType.min
	return args
end

max = function(args)
	args = args or {}
	args.nodeType = NodeType.max
	return args
end

sin = function(args)
	args = args or {}
	args.nodeType = NodeType.sin
	return args
end

smoothstep = function(args)
	args = args or {}
	args.nodeType = NodeType.smoothstep
	return args
end

-- -------------------------------------------------------------------------------------------------

range = function(min, max)
	return {
		min = min,
		max = max
	}
end

shift = function(offset, radius)
	return {
		offset = offset,
		radius = radius
	}
end

full_hue = function()
	return {
		offset = 180,
		radius = 180
	}
end


full_saturation = function()
	return {
		offset = 0.5,
		radius = 0.5
	}
end

full_value = function()
	return {
		offset = 0.5,
		radius = 0.5
	}
end
