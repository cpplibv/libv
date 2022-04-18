// Project: libv.rev, File: src/libv/rev/materials/material_phong.hpp
//
//#pragma once
//
//// libv
//#include <libv/glr/fwd.hpp>
//// pro
//#include <libv/rev/fwd.hpp>
//#include <libv/rev/materials/block_matrices.hpp>
//#include <libv/rev/resource/material.hpp>
//#include <libv/rev/resource/shader.hpp>
//#include <libv/rev/resource/texture.hpp>
//
//
//namespace libv {
//namespace rev {
//
//// -------------------------------------------------------------------------------------------------
//
//struct UniformsPhong {
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
//using ShaderPhong = libv::rev::Shader<UniformsPhong>;
//
//// -------------------------------------------------------------------------------------------------
//
//class MaterialPhong : public BaseMaterial {
//public:
//	ShaderPhong shader;
//
//	Texture diffuse;
//	//Texture albedo;
//	//Texture normal;
//	//Texture roughness;
//	//Texture metalic;
//
//public:
//	inline MaterialPhong(ShaderPhong shader, Texture diffuse) :
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
////	const auto shader = scan.shader<ShaderSpriteBaker>("builtin/sprite_baker.vs", "builtin/sprite_baker.fs");
////	const auto shader = scan.shader<UniformsSpriteBaker>("builtin/sprite_baker.vs", "builtin/sprite_baker.fs");
////
////	const auto tex_diffuse = scan.texture_or(libv::vm4::TextureType::diffuse, "builtin:white");
////	const auto tex_specular = scan.texture_or(libv::vm4::TextureType::specular, "builtin:black");
////	const auto tex_normal = scan.texture_or(libv::vm4::TextureType::normals, "builtin:up");
////
////	const auto color_diffuse = scan.vec4f_or("color_diffuse", {1, 1, 1, 1});
////	const auto color_ambient = scan.vec4f_or("color_ambient", {0, 0, 0, 1});
////	const auto color_emissive = scan.vec4f_or("color_emissive", {0, 0, 0, 1});
////	const auto color_reflective = scan.vec4f_or("color_reflective", {0, 0, 0, 1});
////	const auto color_specular = scan.vec4f_or("color_specular", {1, 1, 1, 1});
////
////	const auto opacity = scan.float_or("opacity", 1.0f);
////	const auto shininess = scan.float_or("shininess", 16.0f);
////	const auto shininess_strength = scan.float_or("shininess_strength", 1.0f);
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace rev
//} // namespace libv
