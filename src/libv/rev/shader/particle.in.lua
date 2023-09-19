
local shader = plugin("shader")

namespace("libv::rev")

include_hpp("libv/rev/shader/attribute.hpp")
include_hpp("libv/rev/shader/block/camera200.hpp")
include_hpp("libv/rev/shader/block/matrices200.hpp")

shader.generate{
	name = "Particle",
	uniforms = {
		{"texture", "texture0", "textureUnit_diffuse"},
	},
	blocks = {
		"Matrices200",
		"Camera200",
	},
}
