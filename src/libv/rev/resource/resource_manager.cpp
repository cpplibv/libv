// Project: libv.rev, File: src/libv/rev/resource/resource_manager.cpp

// hpp
#include <libv/rev/resource/resource_manager.hpp>
// pro
#include <libv/rev/resource/resource_manager_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager() : ResourceManager(Settings{}) {
}

ResourceManager::ResourceManager(Settings settings) :
	self(std::make_shared<InternalResourceManager>(std::move(settings))),
	shader(self->settings.shader.base_path),
	texture(self),
	material(shader, texture),
	model(self, material) {
}

ResourceManager::~ResourceManager() {
	// For the sake of forward declared ptr
}

void ResourceManager::update(libv::gl::GL& gl) {
	self->update(gl);
	shader.update(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
