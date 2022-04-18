// Project: libv.rev, File: src/libv/rev/resource/resource_manager.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/gl/gl_fwd.hpp>
// pro
#include <libv/rev/resource/material_loader.hpp>
#include <libv/rev/resource/model_loader.hpp>
#include <libv/rev/resource/texture_loader.hpp>
#include <libv/rev/resource/shader_loader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ResourceManager {
private:
	std::shared_ptr<InternalResourceManager> self;

public:
	ShaderLoader shader;
	TextureLoader texture;
	MaterialLoader material;
	ModelLoader model;

public:
	ResourceManager();
	explicit ResourceManager(Settings settings);

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) & = delete;
	ResourceManager(ResourceManager&&) noexcept = delete;
	ResourceManager& operator=(ResourceManager&&) & noexcept = delete;

	~ResourceManager();

public:
	void update(libv::gl::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
