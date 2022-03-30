// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/utility/memory/intrusive2_ptr.hpp>
// pro
#include <libv/rev/resource/settings.hpp>
#include <libv/rev/resource/texture_loader.hpp>
//#include <libv/rev/resource/material_loader.hpp>
//#include <libv/rev/resource/model_loader.hpp>
//#include <libv/rev/resource/shader_loader.hpp>
//#include <libv/rev/shader_loader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ResourceManager {
public:
	using Settings = Settings;

private:
	libv::intrusive2_ptr<InternalResourceManager> self;

public:
	TextureLoader texture;
//	ShaderLoader shader;
//	MaterialLoader material;
//	ModelLoader model;

public:
	explicit ResourceManager(Settings settings = {});
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) & = delete;
	ResourceManager(ResourceManager&&) noexcept = delete;
	ResourceManager& operator=(ResourceManager&&) & noexcept = delete;
	~ResourceManager();
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
