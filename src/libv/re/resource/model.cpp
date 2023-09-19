// Project: libv.re, File: src/libv/re/resource/model.cpp

#include <libv/re/resource/model.hpp>

#include <libv/algo/sort.hpp>
#include <libv/math/mat.hpp>
#include <libv/re/common/sub_object_index.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/indirect_comand_buffer_mapped.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/resource/basic_resource.hxx>
#include <libv/re/resource/model_loader.hpp>
#include <libv/utility/hash.hpp>
#include <libv/vm4/model.hpp>

#include <mutex>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Model* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Model* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

struct ColdModelData {
	std::mutex mutex;
	Model_ptr fallback = nullptr;
	// bool created = false;
	// bool dirty... = false;

	bool cached = false;

	// Immutable key
	const std::string identifier;
	const VariantID variant;

	// Loading state
	LoadingState loadingState = LoadingState::standby;
	int currentVersion = -1; /// Value -1 means loading never succeeded, using fallback model instead
	int loadVersion = -1;

	// Properties
	MaterialLoaderFunc materialLoaderFunc;

public:
	ColdModelData(bool cached, LoadingState initState, std::string&& identifier, VariantID variant, MaterialLoaderFunc&& materialLoaderFunc, const Model_ptr& fallback) :
		fallback(fallback),
		cached(cached),
		identifier(std::move(identifier)),
		variant(variant),
		loadingState(initState),
		materialLoaderFunc(std::move(materialLoaderFunc)) { }
};

// -------------------------------------------------------------------------------------------------

Model::Model(std::string identifier, VariantID variant) :
	cold(std::make_unique<ColdModelData>(false, LoadingState::standby, std::move(identifier), variant, nullptr, nullptr)) { }

Model::Model(std::string_view identifier, VariantID variant, MaterialLoaderFunc&& materialLoaderFunc, CachedTag, bool delayed, const Model_ptr& fallback) :
	cold(std::make_unique<ColdModelData>(
			true,
			delayed ? LoadingState::delayed : LoadingState::standby,
			std::string(identifier),
			variant,
			std::move(materialLoaderFunc),
			fallback)) {
}

void Model::ref_count_one() {
	BasicResource::impl_ref_count_one(this, r.model);
}

Model::~Model() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

const std::string& Model::identifier() const noexcept {
	return cold->identifier;
}
VariantID Model::variant() const noexcept {
	return cold->variant;
}
int Model::currentVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->currentVersion;
}
int Model::loadVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->loadVersion;
}

// -------------------------------------------------------------------------------------------------

std::size_t Model::hash() const noexcept {
	// NOTE: This hash function MUST BE kept in sync with ModelKey.hash()
	return libv::hash_combine(cold->identifier, cold->variant);
}

bool Model::operator==(const Model& rhs) const noexcept {
	const auto& lhs = *this;
	return lhs.cold->identifier == rhs.cold->identifier &&
			lhs.cold->variant == rhs.cold->variant;
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] MaterialLoaderFunc& Model::materialLoaderFunc() const noexcept {
	assert(cold->materialLoaderFunc);
	return cold->materialLoaderFunc;
}

// -------------------------------------------------------------------------------------------------

void Model::requestLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestLoad(this, r.model, mode);
}
void Model::requestActivatingLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestActivatingLoad(this, r.model, mode);
}
void Model::recordLoadSuccess() noexcept {
	BasicResource::implRecordLoadSuccess(this, r.model);
}
void Model::recordLoadFailure() noexcept {
	BasicResource::implRecordLoadFailure(this, r.model);
}
void Model::recordUnloadFromCache() noexcept {
	BasicResource::implRecordUnloadFromCache(this);
}

namespace { // -------------------------------------------------------------------------------------

template <typename T>
void load_node(T& output, const libv::vm4::Model& model, const libv::vm4::Node& node, libv::mat4f parentTransform) {
	const auto transform = parentTransform * node.transform;
	for (const auto& meshID : node.meshIDs)
		output.emplace_back(
				model.meshes[meshID].materialID,
				meshID,
				libv::mat4x3f::from_columns(
					// Rows are encoded as columns for packing
					transform.row(0),
					transform.row(1),
					transform.row(2)));
	for (const auto& childID : node.childrenIDs)
		load_node(output, model, model.nodes[childID], transform);
}

} // namespace -------------------------------------------------------------------------------------

void Model::gl_assignModel(const libv::vm4::Model& model, std::span<Material_ptr> materials) noexcept {
	assert(materials.size() == model.materials.size());

	auto lock = std::unique_lock(cold->mutex);

	auto newMesh = libv::re::MeshModelStatic::create(model);

	// TODO P5: LOD support, process every lod, but only 'queue'/'render' the lod that should be used
	const auto& rootNode = model.nodes[model.lods[0].rootNodeID];

	// Gather entries from node hierarchy
	struct Entry {
		uint32_t materialID = 0;
		uint32_t meshID = 0;
		libv::mat4x3f transformTransposed; // Rows are transposed for packing
	};
	std::vector<Entry> entries;
	entries.reserve(model.nodes.size()); // Upper bound
	load_node(entries, model, rootNode, libv::mat4f::identity());

	// Sort into materials -> meshes -> transforms order
	libv::sort_unstable(entries, [](const Entry& lhs, const Entry& rhs) {
		if (lhs.materialID < rhs.materialID)
			return true;
		if (lhs.materialID > rhs.materialID)
			return false;
		return lhs.meshID < rhs.meshID;
	});

	// Process entries into sub-objects
	std::vector<libv::mat4x3f> transformsTransposed;
	transformsTransposed.reserve(model.materials.size());

	auto it = entries.begin();
	const auto end = entries.end();
	uint32_t instanceBase = 0;
	while (it != end) {
		const auto currentMaterialID = it->materialID;
		const auto indirectCommandIndex = static_cast<uint32_t>(subObjectCommands.size());
		uint32_t indirectCommandCount = 0;

		do {
			const auto currentMeshID = it->meshID;

			uint32_t instanceCount = 0;
			do {
				transformsTransposed.emplace_back(it->transformTransposed);
				++it;
				++instanceCount;
			} while (it != end && currentMaterialID == it->materialID && currentMeshID == it->meshID);

			subObjectCommands.push_back(libv::gl::IndirectCommandDrawElements{
					.count = model.meshes[currentMeshID].numIndices,
					.instanceCount = instanceCount,
					.firstIndex = model.meshes[currentMeshID].baseIndex,
					.baseVertex = static_cast<int32_t>(model.meshes[currentMeshID].baseVertex),
					.baseInstance = instanceBase,
			});

			++indirectCommandCount;
			instanceBase += instanceCount;
		} while (it != end && currentMaterialID == it->materialID);

		subObjects.push_back(ModelStaticSubObject{
				.material = std::move(materials[currentMaterialID]),
				.indirectCommandIndex = indirectCommandIndex,
				.indirectCommandCount = indirectCommandCount
		});
	}

	newMesh->instanceMatricesRow012(transformsTransposed);

	// Finalize
	mesh_ = std::move(newMesh);
	cold->fallback = nullptr;
}

void Model::gl_assignMeshMaterial(MeshModelStatic_ptr&& mesh, Material_ptr&& material) noexcept {
	assert(mesh);
	assert(mesh->indexCount() != -1);
	assert(mesh->vertexCount() != -1);
	assert(material);

	auto lock = std::unique_lock(cold->mutex);

	subObjects.push_back(ModelStaticSubObject{
			.material = std::move(material),
			.indirectCommandIndex = 0,
			.indirectCommandCount = 1
	});
	subObjectCommands.push_back(libv::gl::IndirectCommandDrawElements{
			.count = static_cast<uint32_t>(mesh->indexCount()),
			.instanceCount = 1,
			.firstIndex = 0,
			.baseVertex = 0,
			.baseInstance = 0,
	});

	// Finalize
	mesh_ = std::move(mesh);
	cold->fallback = nullptr;
}

BoundingSphere Model::boundingSphere() const {
	if (!mesh_)
		return cold->fallback->boundingSphere();
	return mesh_->boundingSphere();
}

void Model::foreachSubObject(const function_ref<void(Mesh*, Material*, SubObjectIndex)>& func) {
	if (!mesh_) {
		assert(cold->fallback);
		cold->fallback->foreachSubObject(func);
		return;
	}

	uint32_t i = 0;
	for (auto& subObject : subObjects)
		func(mesh_.get(), subObject.material.get(), SubObjectIndex{i++});
}

void Model::gl_update(ContextUpdate& ctx) {
	if (!mesh_) {
		assert(cold->fallback);
		cold->fallback->gl_update(ctx);
		return;
	}

	indirectCommandBufferOffset = ctx.frame.indirectCommandBuffer.write(subObjectCommands);
	// Currently for simplicity every subobject command is uploaded as a whole.
	// This could be optimized with only uploading parts that are actually visible,
	// but it most likely wouldn't save any performance
}

void Model::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	if (!mesh_) {
		assert(cold->fallback);
		cold->fallback->gl_render(ctx, subObjectIndex);
		return;
	}

	assert(+subObjectIndex < subObjects.size());
	const auto& subobject = subObjects[+subObjectIndex];
	assert(subobject.material != nullptr);

	if (subobject.indirectCommandCount == 1)
		// Not measured, but guessing glDrawElementsInstancedBaseVertexBaseInstance is cheaper than glMultiDrawElementsIndirect for a single draw call
		mesh_->gl_renderBaseVertexBaseInstance(ctx,
				subObjectCommands[subobject.indirectCommandIndex].baseVertex,
				subObjectCommands[subobject.indirectCommandIndex].firstIndex,
				subObjectCommands[subobject.indirectCommandIndex].count,
				subObjectCommands[subobject.indirectCommandIndex].baseInstance,
				subObjectCommands[subobject.indirectCommandIndex].instanceCount);
	else
		mesh_->gl_renderIndirect(ctx,
				static_cast<uint32_t>(indirectCommandBufferOffset + subobject.indirectCommandIndex * sizeof(libv::gl::IndirectCommandDrawElements)),
				static_cast<uint32_t>(subobject.indirectCommandCount));
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
