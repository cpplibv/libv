// Project: libv.rev, File: src/libv/rev/resource/material_loader.cpp

// hpp
#include <libv/rev/resource/material_loader.hpp>
// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/vm4/material.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/resource/material.hpp>
#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/resource/shader_loader.hpp>
#include <libv/rev/resource/texture_loader.hpp>

#include <libv/rev/material/material_red.hpp>
#include <libv/rev/material/material_sprite_baker.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct ImplMaterialLoader {
public:
	boost::container::flat_map<std::string, MaterialScanFunc> materialTypes;

	ShaderLoader shaderLoader;
	TextureLoader textureLoader;

	Material fallback;

public:
	inline ImplMaterialLoader(ShaderLoader&& shaderLoader, TextureLoader&& textureLoader) :
		shaderLoader(std::move(shaderLoader)),
		textureLoader(std::move(textureLoader)),
		fallback(libv::make_intrusive2_ptr<MaterialRed>(shaderLoader.load<UniformsRed>("builtin/red.vs", "builtin/red.fs"))) {
	}
};

// -------------------------------------------------------------------------------------------------

MaterialLoader::MaterialLoader(ShaderLoader shaderLoader, TextureLoader textureLoader) :
	self(std::make_shared<ImplMaterialLoader>(std::move(shaderLoader), std::move(textureLoader))) {

	register_material("sprite_baker", MaterialSpriteBaker::create);
	register_material("phong", MaterialSpriteBaker::create);
}

// -------------------------------------------------------------------------------------------------

void MaterialLoader::register_material(std::string name, MaterialScanFunc func) {
	self->materialTypes.emplace(std::move(name), std::move(func));
}

Material MaterialLoader::load(const libv::vm4::Material& vm4Material) {
	MaterialScanner scanner{self->shaderLoader, self->textureLoader, vm4Material};
	const auto it = self->materialTypes.find(vm4Material.shader);
	if (it == self->materialTypes.end()) {
		log_rev.error("Failed to load material type \"{}\": Type not registered. Falling back to builtin:red", vm4Material.name);
		return self->fallback;
	}
	return it->second(scanner);
}

Material MaterialLoader::fallback() const {
	return self->fallback;
}

MaterialScanner MaterialLoader::scanner(const libv::vm4::Material& vm4Material) const {
	MaterialScanner ms{self->shaderLoader, self->textureLoader, vm4Material};
	return ms;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
