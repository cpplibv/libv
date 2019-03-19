-- random.lua

local class = require "lib.util.class"

--

local RandomGenerator = class(function (self, seed)
	self.seed = seed
	math.randomseed(seed)
end)

function RandomGenerator:real(val_min, val_max)
	val_min = val_min or 0
	val_max = val_max or 1
	return (val_max - val_min) * math.random() + val_min
--	return math.random(val_min, val_max)
end

function RandomGenerator:vec3f(val_min, val_max)
	return vec3f(
		self:real(val_min, val_max),
		self:real(val_min, val_max),
		self:real(val_min, val_max))
end

--

local rng = {}
rng.RandomGenerator = RandomGenerator;

return rng
