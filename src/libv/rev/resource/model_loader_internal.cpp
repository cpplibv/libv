// Project: libv.rev, File: src/libv/rev/resource/model_loader_internal.cpp

// hpp
#include <libv/rev/resource/model_loader_internal.hxx>
// libv
#include <libv/glr/procedural/cube.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/vm4/load.hpp>
#include <libv/vm4/model.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/resource/attribute.hpp>
#include <libv/rev/resource/material.hpp>
#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/resource/resource_manager_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

InternalModelLoader::InternalModelLoader(const std::shared_ptr<InternalResourceManager>& irm, const MaterialLoader& materialLoader) :
	irm(irm),
	includeDirectories(1, irm->settings.model.base_path),
	materialLoader(materialLoader),
	defaultNode(libv::mat4f::identity(), materialLoader.fallback(), 0, 0, 6 * 6) {

	initDefaultGLR();
	// NOTE: glr default has to be set before first before anyone could request a load (and copy a default inited glr objects)
}

InternalModelLoader::~InternalModelLoader() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

void InternalModelLoader::initDefaultGLR() {
	auto position = defaultGLRMesh.attribute(attribute_position);
	auto normal = defaultGLRMesh.attribute(attribute_normal);
//	auto tangent = defaultGLRMesh.attribute(attribute_tangent);
//	auto bitangent = defaultGLRMesh.attribute(attribute_bitangent);
	auto texture0 = defaultGLRMesh.attribute(attribute_texture0);
//	auto boneID = defaultGLRMesh.attribute(attribute_bondID);
//	auto boneWeight = defaultGLRMesh.attribute(attribute_bondWeight);

	auto index = defaultGLRMesh.index();

	libv::glr::generateCube(position, normal, texture0, index);

	fallback_ = libv::make_intrusive2_ptr<InternalModel>(
			std::weak_ptr<InternalModelLoader>{}, // Builtins are not tracked: loader can be null
			"<<fallback>>",
			defaultGLRMesh,
			defaultNode,
			0);
}

// -------------------------------------------------------------------------------------------------

InternalModel_ptr InternalModelLoader::load(std::string_view name) {
	auto lock = std::unique_lock(mutex);
	auto& ptr = cache.lookup(name, 0);


	if (!ptr) {
		auto mov = [this](const libv::vm4::Material& vm4Material) {
			return materialLoader.load(vm4Material);
		};
		ptr = create_resource(name, std::move(mov), 0);
	}

	return ptr;
}

InternalModel_ptr InternalModelLoader::load(std::string_view name, Material material, int64_t movID) {
	auto lock = std::unique_lock(mutex);
	auto& ptr = cache.lookup(name, movID);

	if (!ptr) {
		auto mov = [material = std::move(material)](const libv::vm4::Material&) {
			return material;
		};
		ptr = create_resource(name, std::move(mov), movID);
	}

	return ptr;
}

InternalModel_ptr InternalModelLoader::load(std::string_view name, MaterialScanFunc scanFn, int64_t movID) {
	auto lock = std::unique_lock(mutex);
	auto& ptr = cache.lookup(name, movID);

	if (!ptr) {
		auto mov = [this, scanFn = std::move(scanFn)](const libv::vm4::Material& vm4Material) {
			auto ms = materialLoader.scanner(vm4Material);
			return scanFn(ms);
		};
		ptr = create_resource(name, std::move(mov), movID);
	}

	return ptr;
}

InternalModel_ptr InternalModelLoader::load(std::string_view name, MaterialLoaderFunc loadFn, int64_t movID) {
	auto lock = std::unique_lock(mutex);
	auto& ptr = cache.lookup(name, movID);

	if (!ptr)
		ptr = create_resource(name, std::move(loadFn), movID);

	return ptr;
}

InternalModel_ptr InternalModelLoader::fallback() const {
	return fallback_;
}

void InternalModelLoader::unload(InternalModel* resource) {
	auto lock = std::unique_lock(mutex);

	if (resource->ref_count() > 1)
		return; // Resource got reacquired while this call was in flight

	cache.remove(resource);
}

// -------------------------------------------------------------------------------------------------

InternalModel_ptr InternalModelLoader::create_resource(std::string_view name, MaterialLoaderFunc&& mov, int64_t movID) {
	log_rev.trace("Create new model: {} (MOV:{})", name, movID);

	auto loader_sp = shared_from_this();

	// --- Start of real work

	auto res = libv::make_intrusive2_ptr<InternalModel>(loader_sp, std::string(name), defaultGLRMesh, defaultNode, movID);

	// --- End of real work

	irm->queue_fs([l = std::move(loader_sp), r = res, o = std::move(mov)]() mutable {
		l->process_fs(std::move(r), std::move(o));
	});

	return res;
}

void InternalModelLoader::process_fs(InternalModel_ptr&& res, MaterialLoaderFunc&& mov) {
	auto loader_sp = shared_from_this();

	// ---  Start of real work

	bool success = false;
	std::string data;

	auto lock = std::unique_lock(includeDirectories_m);
	for (const auto& includeDirectory : includeDirectories) {
		const auto resource_path = includeDirectory + res->name_;

//		std::filesystem::exists(resource_path);
//		irm->fsw.subscribe_file(resource_path, [] {
//		});

		const auto resource_data = libv::read_file_ec(resource_path);
		if (resource_data.ec) {
//			errors.emplace_back(resource_path, resource_data.ec);
			log_rev.warn("Failed to read file {} for {}: {}", resource_path, res->name_, resource_data.ec.message());
			continue;
		} else {
			success = true;
			data = std::move(resource_data.data);
			break;
		}
	}

	if (!success) {
		log_rev.error("Failed to load model {}: Failed to load the file", res->name_);
		return;
	}

	// --- End of real work

	irm->queue_cpu([l = std::move(loader_sp), r = std::move(res), o = std::move(mov), d = std::move(data)]() mutable {
		l->process_cpu(std::move(r), std::move(o), std::move(d));
	});
}

void InternalModelLoader::process_cpu(InternalModel_ptr&& res, MaterialLoaderFunc&& mov, std::string&& data) {
	auto loader_sp = shared_from_this();

	// ---  Start of real work

	libv::vm4::Model vm4Model;

	try {
		vm4Model = libv::vm4::load_or_throw(std::move(data));
	} catch (const std::exception& e) {
		log_rev.error("Failed to load model: {}", e.what());
		return;
	}

	std::vector<Material> materials;
	materials.reserve(vm4Model.materials.size());
	for (const auto& vm4Material : vm4Model.materials)
		materials.emplace_back(mov(vm4Material));

	// --- End of real work

	irm->queue_res([l = std::move(loader_sp), r = std::move(res), m = std::move(vm4Model), ma = std::move(materials)]() mutable {
		l->process_res(std::move(r), std::move(m), std::move(ma));
	});
}

void InternalModelLoader::process_res(InternalModel_ptr&& res, libv::vm4::Model&& model, std::vector<Material>&& materials) {
	// --- Start of real work

	res->load_mesh(model);
	res->load_node(model, materials);

	// --- End of real work
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv

