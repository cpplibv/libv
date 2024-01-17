// Project: libv.re, File: src/libv/re/node/skybox.cpp

#include <libv/re/node/skybox.hpp>

#include <libv/re/material/material_skybox.hpp>
#include <libv/re/mesh/mesh_skybox.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Skybox* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Skybox* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Skybox::Skybox(Texture_ptr skyTexture, SkyboxType skyboxType) :
	Object(libv::vec3f{}, MaterialSkybox::create(std::move(skyTexture), skyboxType), MeshSkybox::create()) {
}

// -------------------------------------------------------------------------------------------------

void Skybox::textureSky(Texture_ptr texture, SkyboxType type) noexcept {
	static_cast<MaterialSkybox&>(*material).textureSky = std::move(texture);
	static_cast<MaterialSkybox&>(*material).skyboxType = type;
}
Texture_ptr Skybox::texture() const noexcept {
	return static_cast<const MaterialSkybox&>(*material).textureSky.texture;
}
SkyboxType Skybox::type() const noexcept {
	return static_cast<const MaterialSkybox&>(*material).skyboxType.value;
}

void Skybox::intensity(float value) noexcept {
	static_cast<MaterialSkybox&>(*material).intensity = value;
}
float Skybox::intensity() const noexcept {
	return static_cast<const MaterialSkybox&>(*material).intensity.value;
}

void Skybox::rotateZ(float value) noexcept {
	static_cast<MaterialSkybox&>(*material).matRotateZ = libv::mat2f::create_rotate(libv::radian(value));
}

void Skybox::horizonOffset(float value) noexcept {
	static_cast<MaterialSkybox&>(*material).horizonOffset = value;
}
float Skybox::horizonOffset() const noexcept {
	return static_cast<const MaterialSkybox&>(*material).horizonOffset.value;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
