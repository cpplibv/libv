// Project: libv.re, File: src/libv/re/resource/model_loader.cpp

#include <libv/re/resource/model_loader.hpp>

#include <cppcoro/generator.hpp>

#include <libv/re/log.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/material/material_solid_model.hpp>
#include <libv/re/mesh/mesh_model_static.hpp>
#include <libv/re/resource/basic_resource_loader.hxx>
#include <libv/re/resource/material_loader.hpp>
#include <libv/re/resource/material_scanner.hpp>
#include <libv/re/resource/model_fmt.hpp>
#include <libv/re/resource/model_load_event.hpp>
#include <libv/re/settings.hpp>
#include <libv/utility/hash.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/vm4/load.hpp>
#include <libv/vm4/model.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

DelayedModelLoadGuard::~DelayedModelLoadGuard() {
	if (requestLoadOnDestroy)
		ResourceAccess::requestActivatingLoad(*resource, mode);
}

// -------------------------------------------------------------------------------------------------

struct ModelKey {
	std::string_view identifier;
	VariantID variant;

	ModelKey(const std::string_view& identifier, VariantID variant, MaterialLoaderFunc& materialLoaderFunc) :
		identifier(identifier),
		variant(variant) {
		(void) materialLoaderFunc; // materialLoaderFunc must be encoded as variantID, therefore it isn't part of the hash
	}

	[[nodiscard]] std::size_t hash() const noexcept {
		// NOTE: This hash function MUST BE kept in sync with Model.hash()
		return libv::hash_combine(identifier, variant);
	}

	[[nodiscard]] friend bool operator==(const Model& lhs, const ModelKey& rhs) {
		return lhs.identifier() == rhs.identifier && lhs.variant() == rhs.variant;
	}
	[[nodiscard]] friend bool operator==(const ModelKey& lhs, const Model& rhs) {
		return rhs == lhs;
	}
};

struct TraitsModelLoader {
	static constexpr std::string_view name_resource = "model";
	static constexpr std::string_view name_Resource = "Model";
	static constexpr std::string_view name_Loader = "ModelLoader";
	using Resource = Model;
	using ResourceID = ModelID;
	using ResourceKey = ModelKey;
	using DelayedLoadGuard = DelayedModelLoadGuard;
};

struct ImplModelLoader : BasicResourceLoader<ImplModelLoader, TraitsModelLoader> {
	Model_ptr fallback;
	std::unordered_map<std::string_view, Model_ptr> builtins;

	libv::Nexus nexus;

public:
	explicit ImplModelLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
		BasicResourceLoader<ImplModelLoader, TraitsModelLoader>(settings.model),
		nexus(nexus) {
		(void) gl;
	}

	~ImplModelLoader() {
		builtins.clear();
		fallback = nullptr;
		auto lock = std::unique_lock(mutex);
		if (!cache.empty())
			log_re.error("ModelLoader is destroyed before every cached Model instance is destroyed. {} Model alive.", cache.size());
		if (!watchedFiles.empty())
			log_re.error("ModelLoader is destroyed before every watched fsw token is destroyed. {} File token alive.", watchedFiles.size());
	}

public:
	void initFallbacks() {
		auto mesh = MeshModelStatic::create();

		using F = float;
		using vec3 = libv::vec3_t<F>;
		using vec2 = libv::vec2_t<F>;
		using index_t = uint32_t;

		const auto numVertices = 6 * 4;
		const auto numIndices = 6 * 3 * 2;

		struct VertexShadeInfo {
			libv::vec2f uv = libv::uninitialized;
			libv::vec3f normal = libv::uninitialized;
			libv::vec3f tangent = libv::uninitialized;
		};

		auto position = mesh->addAttribute(libv::gl::BufferUsage::StaticDraw, attrPosition, numVertices);
		auto vertex = mesh->addBuffer<VertexShadeInfo>(libv::gl::BufferUsage::StaticDraw, numVertices);
		mesh->addAttribute(vertex, attrUV, &VertexShadeInfo::uv);
		mesh->addAttribute(vertex, attrNormal, &VertexShadeInfo::normal);
		mesh->addAttribute(vertex, attrTangent, &VertexShadeInfo::tangent);
		auto index = mesh->addIndex<index_t>(libv::gl::BufferUsage::StaticDraw, numIndices);

		// Inner cube generation:
		struct Face {
			vec3 origin;
			vec3 right;
			vec3 up;
			vec3 normal;
		};
		static const Face faces[6] = {
				//   origin                 'U'              'V'                normal
				Face{vec3(+.5f, +.5f, +.5f), vec3(0, 0, -1), vec3(0, -1, 0), vec3(+1, 0, 0)}, // Front
				Face{vec3(+.5f, +.5f, +.5f), vec3(-1, 0, 0), vec3(0, 0, -1), vec3(0, +1, 0)}, // Left
				Face{vec3(+.5f, +.5f, +.5f), vec3(0, -1, 0), vec3(-1, 0, 0), vec3(0, 0, +1)}, // Top

				Face{vec3(-.5f, -.5f, -.5f), vec3(0, +1, 0), vec3(0, 0, +1), vec3(-1, 0, 0)}, // Back
				Face{vec3(-.5f, -.5f, -.5f), vec3(0, 0, +1), vec3(+1, 0, 0), vec3(0, -1, 0)}, // Right
				Face{vec3(-.5f, -.5f, -.5f), vec3(+1, 0, 0), vec3(0, +1, 0), vec3(0, 0, -1)}, // Bottom
		};

		for (int i = 0; i < 6; ++i) {
			const auto& face = faces[i];

			position[i * 4 + 0] = face.origin + face.right;
			position[i * 4 + 1] = face.origin;
			position[i * 4 + 2] = face.origin + face.up;
			position[i * 4 + 3] = face.origin + face.right + face.up;
			vertex[i * 4 + 0].uv = vec2(0, 0);
			vertex[i * 4 + 1].uv = vec2(1, 0);
			vertex[i * 4 + 2].uv = vec2(1, 1);
			vertex[i * 4 + 3].uv = vec2(0, 1);
			vertex[i * 4 + 0].normal = face.normal;
			vertex[i * 4 + 1].normal = face.normal;
			vertex[i * 4 + 2].normal = face.normal;
			vertex[i * 4 + 3].normal = face.normal;
			vertex[i * 4 + 0].tangent = face.right;
			vertex[i * 4 + 1].tangent = face.right;
			vertex[i * 4 + 2].tangent = face.right;
			vertex[i * 4 + 3].tangent = face.right;
		}

		for (int i = 0; i < 6; ++i) {
			index[i * 6 + 0] = i * 4 + 0;
			index[i * 6 + 1] = i * 4 + 1;
			index[i * 6 + 2] = i * 4 + 2;
			index[i * 6 + 3] = i * 4 + 2;
			index[i * 6 + 4] = i * 4 + 3;
			index[i * 6 + 5] = i * 4 + 0;
		}

		libv::mat4x3f transformsTransposed[1] = {libv::mat4x3f::from_columns(
				libv::vec4f{1, 0, 0, 0},
				libv::vec4f{0, 1, 0, 0},
				libv::vec4f{0, 0, 1, 0}
		)};
		mesh->instanceMatricesRow012(std::span(transformsTransposed));

		auto material = MaterialSolidModel::create(libv::vec4f{1, 0, 1, 1}); // magenta
		// Consider a lighting enabled solid color material instead

		fallback = Model_ptr::make("fallback:cube");
		fallback->gl_assignMeshMaterial(std::move(mesh), std::move(material));
	}

	void initBuiltins() {
		builtins.emplace("builtin:cube", fallback);
	}

public:
	// NOTE: Events could broadcast inline or on any other thread, but for easier UI interactions they are switched to GL thread
	inline void broadcast(ModelLoadSuccess&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(ModelLoadFailure&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(ModelUnload&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcastResourceUnload(Model_ptr&& resource) const {
		broadcast(ModelUnload{resource->id(), std::move(resource)});
	}

	[[nodiscard]] Model_ptr _loadBuiltin(std::string_view identifier, VariantID variant, MaterialLoaderFunc& materialLoaderFunc) {
		(void) materialLoaderFunc;

		if (!identifier.starts_with("builtin:"))
			return nullptr;

		log_re.warn_if(variant != VariantID::Default, "Requested builtin model '{}' with non-default variant: {}", identifier, +variant);
		const auto it = builtins.find(identifier);
		if (it == builtins.end()) {
			log_re.error("Requested model is not a valid builtin: '{}'", identifier);
			return fallback;
		} else {
			return it->second;
		}
	}

	cppcoro::generator<ThreadAffinity> loadingRoutine(Model_ptr model, LoadingMode mode) {
		// NOTE: Owning ptr keeps alive the resource during the whole loading process
		log_re.trace("{} {} model: '{}'",
				model->loadVersion() == -1 ? "Loading" : "Reloading", to_string(mode), *model);

		std::vector<WatchedFile>* watchTokens = nullptr;
		std::vector<WatchedFile> oldTokens;
		{
			auto lock = std::unique_lock(mutex);
			if (trackFiles) {
				// NOTE: Taking the address relies on that std::unordered_map is stable
				watchTokens = &watchedFiles[model->id()];
				// Moving out old fsw tokens and keeping them alive until the new watches are created to avoid subscription changes in fsw
				std::swap(*watchTokens, oldTokens);
			}
		}

		co_yield ThreadAffinity::FS; // -------------------------------------------------------------------

		auto fileData = _fs_loadFile(
				watchTokens,
				r.settings.model.resourceMappings,
				model->identifier(),
				model->id());

		if (!fileData) {
			ResourceAccess::recordLoadFailure(*model);
			broadcast(ModelLoadFailure{
				model->id(),
				model,
				ModelLoadFailure::IOFailure{
						std::move(fileData.error().unmatchedMappings),
						std::move(fileData.error().mappingErrors),
						std::move(fileData.error().physicalErrors)
				},
				std::nullopt
			});
			co_return;
		}

		oldTokens.clear(); // Discard old fsw tokens, as the new tokens are already in place

		co_yield ThreadAffinity::CPU; // ------------------------------------------------------------------

		libv::vm4::Model vm4Model;

		try {
			vm4Model = libv::vm4::load_or_throw(*fileData);
		} catch (const std::exception& e) {
			log_re.error("Failed to load model: '{}'. {}", *model, "Invalid or unsupported model data or format.");
			ResourceAccess::recordLoadFailure(*model);
			broadcast(ModelLoadFailure{
				model->id(),
				model,
				std::nullopt,
				e.what()
			});
			co_return;
		}

		// Load materials as soon as possible (in CPU thread) to start the async fetch of shaders and textures
		auto& materialLoaderFunc = model->materialLoaderFunc();
		libv::small_vector<Material_ptr, 16> materials;
		materials.reserve(vm4Model.materials.size());
		for (const auto& vm4Material : vm4Model.materials)
			materials.emplace_back(materialLoaderFunc(vm4Material));

		// Remove duplicate/matching materials
		for (auto& lhs : materials)
			for (auto& rhs : materials)
				if (lhs != rhs && Material::equals(*lhs, *rhs))
					lhs = rhs;

		co_yield ThreadAffinity::GL; // --------------------------------------------------------------------

		model->gl_assignModel(vm4Model, std::span(materials));

		// The OpenGL parts of the model loading cannot fail (without internal errors), report success
		ResourceAccess::recordLoadSuccess(*model);
		broadcast(ModelLoadSuccess{model->id(), model});

		co_return;
	}
};

// -------------------------------------------------------------------------------------------------

ModelLoader::ModelLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
	self(std::make_unique<ImplModelLoader>(settings, nexus, gl)) { }

void ModelLoader::init() {
	self->initFallbacks();
	self->initBuiltins();
}

void ModelLoader::reset() {
	self.reset();
}

ModelLoader::~ModelLoader() {
	// For the sake of forward declared types
}

namespace {

MaterialLoaderFunc movMaterialLoader() {
	return [](const libv::vm4::Material& vm4Material) {
		return r.material.load(vm4Material);
	};
}

MaterialLoaderFunc movMaterial(Material_ptr&& material) {
	return [m = std::move(material)](const libv::vm4::Material& vm4Material) {
		(void) vm4Material;
		return m;
	};
}

MaterialLoaderFunc movScanner(MaterialScanFunc&& scan) {
	return [s = std::move(scan)](const libv::vm4::Material& vm4Material) {
		return s(r.material.scanner(vm4Material));
	};
}

} // namespace

/// @thread any
Model_ptr ModelLoader::load(std::string_view identifier, VariantID variant, LoadingMode mode) {
	return self->load(mode, self->fallback, identifier, variant, movMaterialLoader());
}

/// @thread any
Model_ptr ModelLoader::load(std::string_view identifier, Material_ptr mov, VariantID variant, LoadingMode mode) {
	return self->load(mode, self->fallback, identifier, variant, movMaterial(std::move(mov)));
}

/// @thread any
Model_ptr ModelLoader::load(std::string_view identifier, MaterialScanFunc mov, VariantID variant, LoadingMode mode) {
	return self->load(mode, self->fallback, identifier, variant, movScanner(std::move(mov)));
}

/// @thread any
Model_ptr ModelLoader::load(std::string_view identifier, MaterialLoaderFunc mov, VariantID variant, LoadingMode mode) {
	return self->load(mode, self->fallback, identifier, variant, std::move(mov));
}

/// @thread any
Model_ptr ModelLoader::load_async(std::string_view identifier, VariantID variant) {
	return self->load(LoadingMode::async, self->fallback, identifier, variant, movMaterialLoader());
}

/// @thread any
Model_ptr ModelLoader::load_async(std::string_view identifier, Material_ptr mov, VariantID variant) {
	return self->load(LoadingMode::async, self->fallback, identifier, variant, movMaterial(std::move(mov)));
}

/// @thread any
Model_ptr ModelLoader::load_async(std::string_view identifier, MaterialScanFunc mov, VariantID variant) {
	return self->load(LoadingMode::async, self->fallback, identifier, variant, movScanner(std::move(mov)));
}

/// @thread any
Model_ptr ModelLoader::load_async(std::string_view identifier, MaterialLoaderFunc mov, VariantID variant) {
	return self->load(LoadingMode::async, self->fallback, identifier, variant, std::move(mov));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed(std::string_view identifier, VariantID variant, LoadingMode mode) {
	return self->load_delayed(mode, self->fallback, identifier, variant, movMaterialLoader());
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed(std::string_view identifier, Material_ptr mov, VariantID variant, LoadingMode mode) {
	return self->load_delayed(mode, self->fallback, identifier, variant, movMaterial(std::move(mov)));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed(std::string_view identifier, MaterialScanFunc mov, VariantID variant, LoadingMode mode) {
	return self->load_delayed(mode, self->fallback, identifier, variant, movScanner(std::move(mov)));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed(std::string_view identifier, MaterialLoaderFunc mov, VariantID variant, LoadingMode mode) {
	return self->load_delayed(mode, self->fallback, identifier, variant, std::move(mov));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed_async(std::string_view identifier, VariantID variant) {
	return self->load_delayed(LoadingMode::async, self->fallback, identifier, variant, movMaterialLoader());
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed_async(std::string_view identifier, Material_ptr mov, VariantID variant) {
	return self->load_delayed(LoadingMode::async, self->fallback, identifier, variant, movMaterial(std::move(mov)));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed_async(std::string_view identifier, MaterialScanFunc mov, VariantID variant) {
	return self->load_delayed(LoadingMode::async, self->fallback, identifier, variant, movScanner(std::move(mov)));
}

/// @thread any
DelayedModelLoadGuard ModelLoader::load_delayed_async(std::string_view identifier, MaterialLoaderFunc mov, VariantID variant) {
	return self->load_delayed(LoadingMode::async, self->fallback, identifier, variant, std::move(mov));
}

Model_ptr ModelLoader::fallback() {
	return self->fallback;
}

void ModelLoader::executeLoad(Model_ptr resource, LoadingMode mode) {
	self->executeLoad(std::move(resource), mode);
}

void ModelLoader::unload(Model* resource) {
	self->requestUnload(resource);
}

void ModelLoader::foreach_model(libv::function_ref<void(const Model_ptr&)> func) {
	const auto lock = std::unique_lock(self->mutex);
	for (const auto& model : self->cache)
		func(model);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
