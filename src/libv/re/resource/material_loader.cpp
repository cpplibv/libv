// Project: libv.re, File: src/libv/re/resource/material_loader.cpp

#include <libv/container/flat_map.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/material_loader.hpp>
#include <libv/re/resource/material_scanner.hpp>
#include <libv/vm4/material.hpp>

#include <mutex>

// [group]
#include <libv/re/material/material_solid.hpp>
#include <libv/re/material/material_test0.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

struct ImplMaterialLoader {
	std::mutex mutex;
	libv::flat_map<std::string, MaterialScanFunc> materialTypes;

	Material_ptr fallback;
};

// -------------------------------------------------------------------------------------------------

MaterialLoader::MaterialLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
	self(std::make_unique<ImplMaterialLoader>()) {
	(void) settings;
	(void) nexus;
	(void) gl;
}

void MaterialLoader::init() {
	self->fallback = MaterialSolid::create(libv::vec4f{1.f, 0.f, 0.f, 1.f});

	// As create is an overload set and the return type is the derived type a wrapper lambda is necessary
	registerMaterial("solid", [](const MaterialScanner& scan) { return MaterialSolid::create(scan); });
	registerMaterial("test0", [](const MaterialScanner& scan) { return MaterialTest0::create(scan); });

	// TODO P3: Implement the basic materials: phong, lambert, pbr
	registerMaterial("phong", [](const MaterialScanner& scan) { return MaterialTest0::create(scan); });
	// registerMaterial("phong", [](const MaterialScanner& scan) { return MaterialPhong::create(scan); });
	// registerMaterial("sprite_baker", MaterialSpriteBaker::create);
}

void MaterialLoader::reset() {
	self.reset();
}

MaterialLoader::~MaterialLoader() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void MaterialLoader::registerMaterial(std::string name, MaterialScanFunc func) {
	auto lock = std::unique_lock(self->mutex);
	self->materialTypes.emplace(std::move(name), std::move(func));
}

Material_ptr MaterialLoader::load(const libv::vm4::Material& vm4Material) {
	auto lock = std::unique_lock(self->mutex);
	MaterialScanner scanner{vm4Material};
	const auto it = self->materialTypes.find(vm4Material.shader);
	if (it == self->materialTypes.end()) {
		log_re.error("Failed to load material type \"{}\": Type not registered. Falling back to solid red", vm4Material.name);
		return self->fallback;
	}
	return it->second(scanner);
}

Material_ptr MaterialLoader::fallback() const {
	auto lock = std::unique_lock(self->mutex);
	return self->fallback;
}

MaterialScanner MaterialLoader::scanner(const libv::vm4::Material& vm4Material) const {
	// auto lock = std::unique_lock(self->mutex);, No need to lock, no member is accessed
	return MaterialScanner{vm4Material};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
