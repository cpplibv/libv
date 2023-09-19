
local uniform_block = plugin("uniform_block")

namespace("libv::rev")

uniform_block.generate(0, "Camera200", "res/shader/block/camera200.glsl", {
	{"mat4", "matP"},
	{"mat4", "matV"},
	{"vec3", "eye"},
	{"vec3", "cameraForwardW"},
	{"vec3", "cameraRightW"},
	{"vec3", "cameraUpW"},
	--{"float", "far"},
	--{"float", "near"},
})
