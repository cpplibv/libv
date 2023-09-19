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

Skybox::Skybox(Texture_ptr skyTexture) :
	Object(libv::vec3f{}, MaterialSkybox::create(std::move(skyTexture)), MeshSkybox::create()) {
	// material->textureType = CubemapType::cubemapXYZ;
	// material->textureType = CubemapType::cubemapZXY;
	// material->textureType = CubemapType::equirectangular;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
