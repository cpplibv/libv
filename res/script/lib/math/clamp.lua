-- clamp.lua

math.clamp = function (value, minv, maxv)
	if value < minv then
		return minv
	elseif value > maxv then
		return maxv
	else
		return value
	end
end


return math
