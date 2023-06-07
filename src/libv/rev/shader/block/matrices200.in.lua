
local uniform_block = plugin("uniform_block")

namespace("libv::rev")

uniform_block.generate(1, "Matrices200", "res/shader/block/matrices200.glsl", {
	{"mat4", "matMVP"};
	{"mat4", "matM"};
})
