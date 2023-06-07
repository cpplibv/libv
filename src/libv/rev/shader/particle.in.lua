
local shader = {}

local uniform_typenames = {
	texture = { glsl = "texture", cpp = "libv::glr::Uniform_texture"},

	bool = { glsl = "bool", cpp = "libv::glr::Uniform_bool"},
	int = { glsl = "int", cpp = "libv::glr::Uniform_int32"},
	int32 = { glsl = "int", cpp = "libv::glr::Uniform_int32"},
	uint = { glsl = "uint", cpp = "libv::glr::Uniform_uint32"},
	uint32 = { glsl = "uint", cpp = "libv::glr::Uniform_uint32"},
	int64 = { glsl = "int64", cpp = "libv::glr::Uniform_int64"},
	uint64 = { glsl = "uint64", cpp = "libv::glr::Uniform_uint64"},
	float = { glsl = "float", cpp = "libv::glr::Uniform_float"},
	double = { glsl = "double", cpp = "libv::glr::Uniform_double"},

	vec2b = { glsl = "vec2b", cpp = "libv::glr::Uniform_vec2b"},
	vec3b = { glsl = "vec3b", cpp = "libv::glr::Uniform_vec3b"},
	vec4b = { glsl = "vec4b", cpp = "libv::glr::Uniform_vec4b"},
	vec2i = { glsl = "vec2i", cpp = "libv::glr::Uniform_vec2i"},
	vec3i = { glsl = "vec3i", cpp = "libv::glr::Uniform_vec3i"},
	vec4i = { glsl = "vec4i", cpp = "libv::glr::Uniform_vec4i"},
	vec2ui = { glsl = "vec2ui", cpp = "libv::glr::Uniform_vec2ui"},
	vec3ui = { glsl = "vec3ui", cpp = "libv::glr::Uniform_vec3ui"},
	vec4ui = { glsl = "vec4ui", cpp = "libv::glr::Uniform_vec4ui"},
	vec2l = { glsl = "vec2l", cpp = "libv::glr::Uniform_vec2l"},
	vec3l = { glsl = "vec3l", cpp = "libv::glr::Uniform_vec3l"},
	vec4l = { glsl = "vec4l", cpp = "libv::glr::Uniform_vec4l"},
	vec2ul = { glsl = "vec2ul", cpp = "libv::glr::Uniform_vec2ul"},
	vec3ul = { glsl = "vec3ul", cpp = "libv::glr::Uniform_vec3ul"},
	vec4ul = { glsl = "vec4ul", cpp = "libv::glr::Uniform_vec4ul"},
	vec2 = { glsl = "vec2", cpp = "libv::glr::Uniform_vec2f"},
	vec3 = { glsl = "vec3", cpp = "libv::glr::Uniform_vec3f"},
	vec4 = { glsl = "vec4", cpp = "libv::glr::Uniform_vec4f"},
	vec2f = { glsl = "vec2", cpp = "libv::glr::Uniform_vec2f"},
	vec3f = { glsl = "vec3", cpp = "libv::glr::Uniform_vec3f"},
	vec4f = { glsl = "vec4", cpp = "libv::glr::Uniform_vec4f"},
	vec2d = { glsl = "vec2d", cpp = "libv::glr::Uniform_vec2d"},
	vec3d = { glsl = "vec3d", cpp = "libv::glr::Uniform_vec3d"},
	vec4d = { glsl = "vec4d", cpp = "libv::glr::Uniform_vec4d"},

	mat2 = { glsl = "mat2", cpp = "libv::glr::Uniform_mat2f"},
	mat3 = { glsl = "mat3", cpp = "libv::glr::Uniform_mat3f"},
	mat4 = { glsl = "mat4", cpp = "libv::glr::Uniform_mat4f"},
	mat2f = { glsl = "mat2", cpp = "libv::glr::Uniform_mat2f"},
	mat3f = { glsl = "mat3", cpp = "libv::glr::Uniform_mat3f"},
	mat4f = { glsl = "mat4", cpp = "libv::glr::Uniform_mat4f"},
	mat2d = { glsl = "mat2d", cpp = "libv::glr::Uniform_mat2d"},
	mat3d = { glsl = "mat3d", cpp = "libv::glr::Uniform_mat3d"},
	mat4d = { glsl = "mat4d", cpp = "libv::glr::Uniform_mat4d"},

	mat2x2 = { glsl = "mat2x2", cpp = "libv::glr::Uniform_mat2x2f"},
	mat2x3 = { glsl = "mat2x3", cpp = "libv::glr::Uniform_mat2x3f"},
	mat2x4 = { glsl = "mat2x4", cpp = "libv::glr::Uniform_mat2x4f"},
	mat3x2 = { glsl = "mat3x2", cpp = "libv::glr::Uniform_mat3x2f"},
	mat3x3 = { glsl = "mat3x3", cpp = "libv::glr::Uniform_mat3x3f"},
	mat3x4 = { glsl = "mat3x4", cpp = "libv::glr::Uniform_mat3x4f"},
	mat4x2 = { glsl = "mat4x2", cpp = "libv::glr::Uniform_mat4x2f"},
	mat4x3 = { glsl = "mat4x3", cpp = "libv::glr::Uniform_mat4x3f"},
	mat4x4 = { glsl = "mat4x4", cpp = "libv::glr::Uniform_mat4x4f"},
	mat2x2f = { glsl = "mat2x2", cpp = "libv::glr::Uniform_mat2x2f"},
	mat2x3f = { glsl = "mat2x3", cpp = "libv::glr::Uniform_mat2x3f"},
	mat2x4f = { glsl = "mat2x4", cpp = "libv::glr::Uniform_mat2x4f"},
	mat3x2f = { glsl = "mat3x2", cpp = "libv::glr::Uniform_mat3x2f"},
	mat3x3f = { glsl = "mat3x3", cpp = "libv::glr::Uniform_mat3x3f"},
	mat3x4f = { glsl = "mat3x4", cpp = "libv::glr::Uniform_mat3x4f"},
	mat4x2f = { glsl = "mat4x2", cpp = "libv::glr::Uniform_mat4x2f"},
	mat4x3f = { glsl = "mat4x3", cpp = "libv::glr::Uniform_mat4x3f"},
	mat4x4f = { glsl = "mat4x4", cpp = "libv::glr::Uniform_mat4x4f"},
	mat2x2d = { glsl = "mat2x2d", cpp = "libv::glr::Uniform_mat2x2d"},
	mat2x3d = { glsl = "mat2x3d", cpp = "libv::glr::Uniform_mat2x3d"},
	mat2x4d = { glsl = "mat2x4d", cpp = "libv::glr::Uniform_mat2x4d"},
	mat3x2d = { glsl = "mat3x2d", cpp = "libv::glr::Uniform_mat3x2d"},
	mat3x3d = { glsl = "mat3x3d", cpp = "libv::glr::Uniform_mat3x3d"},
	mat3x4d = { glsl = "mat3x4d", cpp = "libv::glr::Uniform_mat3x4d"},
	mat4x2d = { glsl = "mat4x2d", cpp = "libv::glr::Uniform_mat4x2d"},
	mat4x3d = { glsl = "mat4x3d", cpp = "libv::glr::Uniform_mat4x3d"},
	mat4x4d = { glsl = "mat4x4d", cpp = "libv::glr::Uniform_mat4x4d"},
}

shader.generate = function(rules)
	local fn_hpp = function(out)
		--struct UniformsParticle {
		--//	libv::glr::Uniform_mat4f matMVP;
		--//	libv::glr::Uniform_mat4f matM;
		--//	libv::glr::Uniform_vec3f color;
		--	libv::glr::Uniform_texture texture0;
		--
		--	template <typename Access> void access_uniforms(Access& access) {
		--		access(texture0, "texture0", textureChannel_diffuse);
		--	}
		--
		--	template <typename Access> void access_blocks(Access& access) {
		--		access(uniformBlock_matrices200);
		--		access(uniformBlock_camera200);
		--	}
		--};

		out("\n")
		out("struct Uniforms" .. rules.name .. " {\n")
		for _, member in ipairs(rules.uniforms) do
			out("\t" .. uniform_typenames[member[1]].cpp .. " " .. member[2] .. ";\n")
		end

		if (#rules.uniforms ~= 0) then
			out("\n")
		end
		out("\ttemplate <typename Access> void access_uniforms(Access& access) {\n")
		if (#rules.uniforms == 0) then
			out("\t\t(void) access;\n")
		end
		for _, member in ipairs(rules.uniforms) do
			out("\t\taccess(" .. member[2] .. ", \"" .. member[2] .. "\"")
			if member[3] ~= nil then
				out(", " .. member[3])
			end
			out(");\n")
		end
		out("\t}\n")
		out("\n")
		out("\ttemplate <typename Access> void access_blocks(Access& access) {\n")
		if (#rules.blocks == 0) then
			out("\t\t(void) access;\n")
		end
		for _, member in ipairs(rules.blocks) do
			out("\t\taccess(uniformBlock_" .. member .. ");\n")
		end
		out("\t}\n")
		out("};\n")
	end

	local fn_cpp = function(out)
		--out("\n")
	end

	include_hpp("libv/glr/layout_std140.hpp")
	include_hpp("libv/glr/uniform.hpp")
	include_hpp("libv/glr/uniform_block_binding.hpp")

	include_cpp(__codegen_filepath_hpp)

	generate_hpp(fn_hpp)
	generate_cpp(fn_cpp)
end

-- -------------------------------------------------------------------------------------------------

namespace("libv::rev")

include_hpp("libv/rev/shader/attribute.hpp")
include_hpp("libv/rev/shader/block/camera200.hpp")
include_hpp("libv/rev/shader/block/matrices200.hpp")

shader.generate{
	name = "Particle",
	uniforms = {
		{"texture", "texture0", "textureChannel_diffuse"},
	},
	blocks = {
		"Matrices200",
		"Camera200",
	},
}
