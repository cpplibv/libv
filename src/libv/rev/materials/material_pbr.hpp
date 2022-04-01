// Project: libv.rev, File: src/libv/rev/materials/material_pbr.hpp
//
//#pragma once
//
//// libv
//#include <libv/glr/fwd.hpp>
//// pro
//#include <libv/rev/fwd.hpp>
//#include <libv/rev/resource/material.hpp>
//#include <libv/rev/materials/block_matrices.hpp>
//#include <libv/rev/resource/texture.hpp>
//#include <libv/rev/shader.hpp>
//
//
//
//namespace libv {
//namespace rev {
//
//// -------------------------------------------------------------------------------------------------
//
//struct UniformsPBR {
////	libv::glr::Uniform_vec4f base_color;
//
//	template <typename Access> void access_uniforms(Access&) {
////		access(base_color, "base_color");
//	}
//
//	template <typename Access> void access_blocks(Access& access) {
//		access(uniformBlock_matrices);
//	}
//};
//
//using ShaderPBR = libv::rev::Shader<UniformsPBR>;
//
//// -------------------------------------------------------------------------------------------------
//
//class MaterialPBR : public BaseMaterial {
//public:
//	ShaderPBR shader;
//
//	Texture diffuse;
//	//Texture albedo;
//	//Texture normal;
//	//Texture roughness;
//	//Texture metallic;
//
//public:
//	inline MaterialPBR(ShaderPBR shader, Texture diffuse) :
//		shader(std::move(shader)),
//		diffuse(std::move(diffuse)) {}
//
//public:
//	static Material create(const MaterialScanner& scan);
//
//public:
//	virtual void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override;
//	// virtual bool utilizes(ModelVertexAttribute attribute) override;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace rev
//} // namespace libv
