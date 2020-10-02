

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


shape(EffectRoundedBox, vec2f(16, 16), vec2f(96, 96), 5.5, 16);

define_flex_point(vec2f(16, 16), vec2f(96, 96))


-- outer_shadow
add_effect(effects, 3.5, 3, vec4f(0, 0, 0, 0.5))
-- inner edge
add_effect(effects, -1.5, 3, vec4f(0.8, 0.8, 0.8, 0.8))

-- outer_shadow
add_effect(effects, 6.5, 3, vec4f(0.2, 0.2, 0.5, 0.5))
-- inner edge
add_effect(effects, -1.5, 1, vec4f(0.8, 0.8, 1, 0.8))

-- -------------------------------------------------------------------------------------------------

