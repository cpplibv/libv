-- Common config

-- -------------------------------------------------------------------------------------------------


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


constant = function(args_or_number)
	if type(args_or_number) == "number" then
		return {
			nodeType = "constant",
			value = args_or_number,
		}
	else
		local node = args_or_number or {}
		node.nodeType = "constant"
		return node
	end
end

value = function(args)
	args = args or {}
	args.nodeType = "value"
	return args
end

perlin = function(args)
	args = args or {}
	args.nodeType = "perlin"
	return args
end

simplex = function(args)
	args = args or {}
	args.nodeType = "simplex"
	return args
end

cellular = function(args)
	args = args or {}
	args.nodeType = "cellular"
	return args
end

simplexFractal = function(args)
	args = args or {}
	args.nodeType = "simplexFractal"
	return args
end

warp = function(args)
	args = args or {}
	args.nodeType = "warp"
	return args
end

fractal = function(args)
	args = args or {}
	args.nodeType = "fractal"
	return args
end

coord = function(args_or_x, none_or_y)
	if type(args_or_x) == "number" and type(none_or_y) == "number" then
		local node = {}
		node.nodeType = "coord"
		node.weights = vec2f(args_or_x, none_or_y)
		return node
	else
		local node = args_or_x or {}
		node.nodeType = "coord"
		return node
	end
end

add = function(args)
	args = args or {}
	args.nodeType = "add"
	return args
end

mul = function(args)
	args = args or {}
	args.nodeType = "mul"
	return args
end

pow = function(args)
	args = args or {}
	args.nodeType = "pow"
	return args
end

mix = function(args)
	args = args or {}
	args.nodeType = "mix"
	return args
end

clamp = function(args)
	args = args or {}
	args.nodeType = "clamp"
	return args
end

saturate = function(args)
	args = args or {}
	args.nodeType = "saturate"
	return args
end

min = function(args)
	args = args or {}
	args.nodeType = "min"
	return args
end

max = function(args)
	args = args or {}
	args.nodeType = "max"
	return args
end

sin = function(args)
	args = args or {}
	args.nodeType = "sin"
	return args
end

smoothstep = function(args)
	args = args or {}
	args.nodeType = "smoothstep"
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

limit = function(soft, hard)
	return {
		soft = soft,
		hard = hard
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
