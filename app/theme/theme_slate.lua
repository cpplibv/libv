local abs = math.abs
local cos = math.cos
local max = math.max
local min = math.min
local pi = math.pi
local sin = math.sin
local sqrt = math.sqrt

-- -------------------------------------------------------------------------------------------------

local rgba = function(r, g, b, a)
	return vec4f(r, g, b, a);
end

-- -------------------------------------------------------------------------------------------------

local box = function(pos, size)
	return { type = "rounded_box", pos = pos, size = size, corner = 0, sharpness = 1 }
end

local rounded_box = function(pos, size, corner, sharpness)
	return { type = "rounded_box", pos = pos, size = size, corner = corner, sharpness = sharpness }
end

local glow = function(size, falloff, color)
	return { type = "glow", size = size, falloff = falloff, color = color }
end

-- -------------------------------------------------------------------------------------------------

local merge = function(base, derived)
	local result = {}
	for k,v in pairs(base) do
		result[k] = v
	end
	for k,v in pairs(derived) do
		if (type(k) == "number") then
			table.insert(result, v)
		else
			result[k] = v
		end
	end
	return result
end

-- -------------------------------------------------------------------------------------------------

register_var("normal_os_size", 0, 8, 0.1, 3.5)
register_var("normal_is_size", 0, 8, 0.1, -3.5)
register_var("normal_oe_size", 0, 8, 0.1, 1.5)
register_var("normal_ie_size", 0, 8, 0.1, -1.5)

--edge_color = rgba()

-- -------------------------------------------------------------------------------------------------

main = function (theme)
	local button_base = {
		box(vec2f(8, 8), vec2f(48, 48)),
	}

	theme.texture_size(64, 64);

	theme.atlas("button.normal", merge(button_base, {
		--glow(-1, 1, rgba(0, 0, 0, 0.5)),
		glow(normal_os_size, 1, rgba(0, 0, 0, 0.5)),
	}))

	theme.atlas("button.pressed", merge(button_base, {
		glow( 2.5, 1, rgba(1, 0, 0, 1)),
		glow(-2.5, 1, rgba(0, 0, 1, 1)),
	}))

	theme.atlas("button.hover", merge(button_base, {
		glow( 3.5, 3, rgba(0, 1, 0, 0.5)),
		glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
	}))

	theme.atlas("button.inactive", merge(button_base, {
		glow( 3.5, 3, rgba(0, 0, 1, 0.5)),
		glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
	}))
end

-- =================================================================================================
