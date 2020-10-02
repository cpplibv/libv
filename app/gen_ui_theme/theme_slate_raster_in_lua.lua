

data = {}

data_color = data
data_mask = {}

--tex_size_x = 512
--tex_size_y = 512

local abs = math.abs
local cos = math.cos
local max = math.max
local min = math.min
local pi = math.pi
local sin = math.sin
local sqrt = math.sqrt

-- -------------------------------------------------------------------------------------------------

local set = function(x, y, color, mask)
    data_color[tex_size_x * (y - 1) + x] = color
    data_mask[tex_size_x * (y - 1) + x] = mask
end

-- -------------------------------------------------------------------------------------------------

local blend_rgb_a = function(src_rgb, src_a, dst_rgb, dst_a)
    local out_a = src_a + dst_a * (1 - src_a)
    local out_rgb = (src_rgb * src_a + dst_rgb * dst_a * (1 - src_a)) / out_a

    if (abs(out_a) < 0.00001) then
        return src_rgb, src_a
    else
        return out_rgb, out_a
    end
end

local blend_rgba = function(src_rgba, dst_rgba)
    local out_rgb, out_a = blend_rgb_a(
            vec3f(src_rgba.x, src_rgba.y, src_rgba.z), src_rgba.w,
            vec3f(dst_rgba.x, dst_rgba.y, dst_rgba.z), dst_rgba.w
    )
    return vec4f(out_rgb.x, out_rgb.y, out_rgb.z, out_a);
end

-- -------------------------------------------------------------------------------------------------

local clamp = function (value, minv, maxv)
    if value < minv then
        return minv
    elseif value > maxv then
        return maxv
    else
        return value
    end
end

local saturate = function (value)
    if value < 0 then
        return 0
    elseif value > 1 then
        return 1
    else
        return value
    end
end

local mix = function(v0, v1, t)
    return (1 - t) * v0 + t * v1;
end

local vec2_abs = function(v)
    return vec2f(abs(v.x), abs(v.y))
end

local vec2_max = function(a, b)
    if type(b) == "number" then
        return vec2f(max(a.x, b), max(a.y, b))
    else
        return vec2f(max(a.x, b.x), max(a.y, b.y))
    end
end

local dist_round_box = function(pos, size, r)
    size = size * 0.5
    pos = pos - size
    size = size - r
    -- pos : vec2f
    -- size : vec2f
    -- rounding : float

    local q = vec2_abs(pos) - size;
    return (vec2_max(q, 0.0)):length() + min(max(q.x, q.y), 0.0) - r;
end

-- -------------------------------------------------------------------------------------------------

local comp_pos = vec2f(16, 16)
local comp_size = vec2f(96, 96)

local corner_size = 5.5
local corner_sharpness = 16


local effects = {}

local add_effect = function(effects, size, falloff, color)
    table.insert(effects, {
        inward = size < 0,
        size = abs(size),
        falloff = falloff,
        color = color,
    })
end

---- outer_shadow
--add_effect(effects, 3.5, 3, vec4f(0, 0, 0, 0.5))
---- inner edge
--add_effect(effects, -1.5, 3, vec4f(0.8, 0.8, 0.8, 0.8))

-- outer_shadow
add_effect(effects, 6.5, 3, vec4f(0.2, 0.2, 0.5, 0.5))
-- inner edge
add_effect(effects, -1.5, 1, vec4f(0.8, 0.8, 1, 0.8))

-- -------------------------------------------------------------------------------------------------

local add_layer = function(output, color, shape)
    return blend_rgba(vec4f(color.x, color.y, color.z, color.w * shape), output)
end

for x = 1, tex_size_x do
    for y = 1, tex_size_y do
        local pos = vec2f(x, y)
        local output = vec4f(0, 0, 0, 0)

        local signed_dist = dist_round_box(pos - comp_pos, comp_size, corner_size)

        local box = mix(0, 1, -signed_dist) -- Sharpen the shape with mix
        local mask = saturate(box);
        -- <<< need signed_dist field recalculation after mask has been created, create a binary mask
        --local mask = saturate(box) ^ corner_sharpness;


        for k, effect in ipairs(effects) do
            local effect_shape = 0;

            if (effect.inward) then
                effect_shape = (signed_dist + 1 + effect.size) / effect.size
                effect_shape = saturate(saturate(effect_shape) - (1 - mask))
            else
                effect_shape = (-signed_dist + effect.size) / effect.size
                effect_shape = saturate(saturate(effect_shape) - mask)
            end

            effect_shape = effect_shape ^ effect.falloff
            output = add_layer(output, effect.color, effect_shape)
        end

        set(x, y, output, mask)
    end
end

-- -------------------------------------------------------------------------------------------------

return data
