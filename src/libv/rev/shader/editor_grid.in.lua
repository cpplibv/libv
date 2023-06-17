
local shader = plugin("shader")

namespace("libv::rev")

include_hpp("libv/rev/shader/block/matrices200.hpp")
include_hpp("libv/rev/shader/block/camera200.hpp")

shader.generate{
	name = "EditorGrid",
	uniforms = {
	},
	blocks = {
		"Matrices200",
		"Camera200",
	},
}
