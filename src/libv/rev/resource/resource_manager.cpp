// Project: libv.rev, File: src/libv/rev/resource/resource_manager.cpp

// hpp
#include <libv/rev/resource/resource_manager.hpp>
// pro
#include <libv/rev/resource/resource_manager_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager(libv::Nexus& nexus) : ResourceManager(Settings{}, nexus) {
}

ResourceManager::ResourceManager(Settings settings, libv::Nexus& nexus) :
	self(std::make_shared<InternalResourceManager>(std::move(settings))),
	shader(nexus, self->settings.shader.base_path),
	texture(self),
	material(shader, texture),
	model(self, material) {
}

ResourceManager::~ResourceManager() {
	// For the sake of forward declared types
}

void ResourceManager::update(libv::GL& gl) {
	self->update(gl);
	shader.update(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
